import os
import glob
import shutil

### PROJECT CONFIGURATION ###
APPNAME = 'cap2re'
VERSION = '1.0'
top = os.getcwd()
extras = top + '/waf-extras'
out = top + '/build'
#############################

import waflib.Logs as Logs
import copy

def yesno(b):
	if b:
		return "yes"
	return "no"

def yesnocolor(b):
	res = yesno(b)

	if res == "yes":
		col = 'GREEN'
	else:
		col = 'RED'

	return res, col

def options(ctx):
	ctx.load('compiler_c')
	ctx.load('compiler_cxx')
	ctx.add_option(
		'--with-backends',
		action='store',
		type='str',
		default="basler,libgphoto2",
		dest="backends",
		help="Comma-separated names of backends to compile. By default, all	available backends are built (basler, libgphoto2)."
	)
	ctx.load('check_pylon', tooldir=extras)

	ctx.add_option(
		'--disable-python',
		action='store_false',
		default=True,
		dest='buildPythonWrapper',
		help="Do not build Python wrapper for the library"
	)

	ctx.add_option(
		'--disable-mt',
		action='store_false',
		default=True,
		dest='useMultiThreading',
		help="Disable the support for parallel (multithreaded) captures"
	)

def configure(ctx):
	backends = ctx.options.backends.split(',') or ['basler, libgphoto2']

	# check if valid names are given
	for b in backends:
		if b not in ['basler', 'libgphoto2']:
			ctx.fatal("Unknown backend '%s' given!" % b)

	ctx.load('compiler_c')
	ctx.load('compiler_cxx')
	ctx.env.PREFIX = '/usr'

	# -- check for libraries supporting libgphoto2 backend --
	if 'libgphoto2' in backends:
		# check for Boost.RegEx, needed for configuration tree
		ctx.check_cxx(
			header_name = 'boost/regex.hpp',
			lib = 'boost_regex',
			uselib_store = 'BOOST_REGEX',
			msg = "Checking for Boost.RegEx"
		)

		# look for libgphoto2
		ctx.check_cfg(
			package = 'libgphoto2',
			args = '--cflags --libs',
			uselib_store = 'GPHOTO2',
			msg = "Checking for libgphoto2",
			mandatory = False,
		)

	# -- check for multithreaded captures feature supporting libraries --
	if ctx.options.useMultiThreading:
		ctx.check_cxx(
			header_name = 'boost/thread.hpp',
			lib = 'boost_thread',
			uselib_store = 'BOOST_THREAD',
			msg = "Checking for Boost.Thread"
		)

	# -- check for python wrapper supporting libraries --
	if ctx.options.buildPythonWrapper:
		# we need Python headers
		for prog in ['python-config', 'PYTHON-config']:
			if ctx.find_program(prog, var='PYTHON_CONFIG', mandatory=False):
				break

		if not ctx.env.PYTHON_CONFIG:
			ctx.fatal('python-config is missing')

		ctx.check_cfg(
			path=ctx.env.PYTHON_CONFIG,
			package = '',
			args = '--include',
			uselib_store = 'PYTHON',
			msg = "Checking for Python headers"
		)

	# -- basler backend supporting libraries search --
	if 'basler' in backends:
		# look for Pylon SDK
		ctx.load('check_pylon', tooldir=top)

		# check for libjpeg, needed for basler backend
		ctx.check_cxx(
			lib='jpeg',
			uselib_store = 'LIBJPEG',
			msg = "Checking for libjpeg",
		)

	ctx.env.CXXFLAGS += ['-fPIC']

	# summary printout
	print
	print "Available backends:"
	ctx.msg("   libgphoto2", *yesnocolor(ctx.env.HAVE_GPHOTO2))
	ctx.msg("   Basler GiGE", *yesnocolor(ctx.env.HAVE_PYLON))
	print
	print "Features:"
	ctx.msg("   Parallel captures:", *yesnocolor(ctx.env.HAVE_BOOST_THREAD_HPP))
	ctx.msg("   Python wrapper:", *yesnocolor(ctx.env.HAVE_PYTHON))

	ctx.env.DEFINES = []
	ctx.define('HAVE_PYLON', ctx.env.HAVE_PYLON, False)
	ctx.define('HAVE_GPHOTO2', ctx.env.HAVE_GPHOTO2 or 0, False)
	ctx.define('HAVE_BOOST_THREAD', ctx.env.HAVE_BOOST_THREAD_HPP or 0, False)
	ctx.define('PYLON_ROOT', ctx.env.PYLON_ROOT, True)
	ctx.define('GENICAM_ROOT', ctx.env.GENICAM_ROOT, True)

	ctx.write_config_header('../src/config.h', remove=False)

def build(ctx):
	components = ['Core']
	libs = []

	# -- Build core library --
	sources = [
		'src/Camera.cpp',
		'src/CameraManager.cpp',
	]

	if ctx.get_define('HAVE_BOOST_THREAD') == "1":
		libs.append('BOOST_THREAD')

	ctx.objects(
		source = sources,
		target = 'Core',
		uselib = libs,
	)

	# -- Build libgphoto2 backend, if we can --
	if ctx.get_define('HAVE_GPHOTO2') == "1":
		sources = [
			'src/Node.cpp',
			'src/ConfigOptsTree.cpp',
			'src/Gphoto2Camera.cpp',
			'src/Gphoto2CameraManager.cpp',
		]

		ctx.objects(
			source = sources,
			target = 'Gphoto2Backend',
			uselib = ['BOOST_REGEX', 'GPHOTO2'],
		)

		libs += ['BOOST_REGEX', 'GPHOTO2']
		components.append('Gphoto2Backend')

	# -- Build basler backend, possibly --
	if ctx.get_define('HAVE_PYLON') == "1":
		sources = [
			'src/BaslerCamera.cpp',
			'src/BaslerCameraManager.cpp',
		]

		ctx.objects(
			source = sources,
			target = 'BaslerBackend',
			uselib = ['PYLON', 'LIBJPEG'],
		)

		libs += ['PYLON', 'LIBJPEG']
		components.append('BaslerBackend')

	# -- Build the C++ library --
	ctx.shlib(
		source = 'src/Cap2re.cpp',
		target = 'cap2re',
		uselib = libs,
		use = components
	)

	# -- Build Python wrapper for library, if needed --
	if ctx.env.HAVE_PYTHON:
		ctx.shlib(
			source = 'src/PythonWrapper.cpp',
			target = 'pycap2re',
			uselib = ['PYTHON'],
			use = 'cap2re'
		)