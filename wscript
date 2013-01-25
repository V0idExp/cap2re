import os

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
	ctx.load('check_pylon', tooldir=extras)

def configure(ctx):
	plainctx = copy.copy(ctx)

	ctx.load('compiler_c')
	ctx.load('compiler_cxx')
	ctx.env.PREFIX = '/usr'

	# check for Boost.RegEx, needed for ConfigOptsTree
	ctx.check_cxx(
		header_name = 'boost/regex.hpp',
		lib = 'boost_regex',
		uselib_store = 'BOOST_REGEX',
		msg = "Checking for Boost.RegEx"
	)

	# check for Boost.Thread, needed for CameraManager multithreaded capture
	ctx.check_cxx(
		header_name = 'boost/thread.hpp',
		lib = 'boost_thread',
		uselib_store = 'BOOST_THREAD',
		msg = "Checking for Boost.Thread"
	)

	# check for Python headers
	for prog in ['python-config', 'python2-config']:
		if ctx.find_program(prog, var='PYTHON_CONFIG', mandatory=False):
			break

	if not ctx.env.PYTHON_CONFIG:
		ctx.fatal('python-config is missing')

	ctx.check_cfg(
		path=ctx.env.PYTHON_CONFIG,
		package = '',
		args = '--include',
		uselib_store = 'PYTHON2',
		msg = "Checking for Python headers"
	)

	# check for SWIG
	ctx.find_program('swig')

	# look for libgphoto2
	ctx.check_cfg(
		package = 'libgphoto2',
		args = '--cflags --libs',
		uselib_store = 'GPHOTO2',
		msg = "Checking for libgphoto2",
		mandatory = False,
	)

	# look for Pylon SDK
	ctx.load('check_pylon', tooldir=top)

	# check for libjpeg, needed for basler backend
	if ctx.env.HAVE_PYLON:
		ctx.check_cxx(
			lib='jpeg',
			uselib_store = 'JPEG',
			msg = "Checking for libjpeg",
		)

	ctx.env.CXXFLAGS += ['-fPIC']

	# summary printout
	print
	print "Available backends:"
	ctx.msg("   libgphoto2", *yesnocolor(ctx.env.HAVE_GPHOTO2))
	ctx.msg("   Basler GiGE", *yesnocolor(ctx.env.HAVE_PYLON))

	ctx.env.DEFINES = []
	ctx.define('HAVE_PYLON', ctx.env.HAVE_PYLON, False)
	ctx.define('HAVE_GPHOTO2', ctx.env.HAVE_GPHOTO2, False)
	ctx.define('PYLON_ROOT', ctx.env.PYLON_ROOT, True)
	ctx.define('GENICAM_ROOT', ctx.env.GENICAM_ROOT, True)

	ctx.write_config_header('../src/config.h', remove=False)

def build(ctx):
	# Build platform-independent code
	baselib = ['ConfigOptsTree', 'Camera', 'CameraManager', 'Cap2re']

	ctx.objects(
		source = 'src/Node.cpp src/ConfigOptsTree.cpp',
		target = 'ConfigOptsTree',
		uselib = ['BOOST_REGEX'],
	)

	ctx.objects(
		source = 'src/Camera.cpp',
		target = 'Camera'
	)

	ctx.objects(
		source = 'src/CameraManager.cpp',
		target = 'CameraManager',
		uselib = ['BOOST_THREAD'],
	)

	ctx.objects(
		source = 'src/Cap2re.cpp',
		target = 'Cap2re',
	)

	# Build backends
	backends = []
	libs = []

	if ctx.get_define('HAVE_GPHOTO2') == "1":
		target = 'backend_gphoto2'
		libs.append('GPHOTO2')

		ctx.objects(
			source = 'src/Gphoto2Camera.cpp src/Gphoto2CameraManager.cpp',
			target = target,
			uselib = ['BOOST_THREAD']
		)

		backends.append(target)

	if ctx.get_define('HAVE_PYLON') == "1":
		backends.append('backend_basler')
		libs.append('PYLON')

		ctx.objects(
			source = ['src/BaslerCamera.cpp', 'src/BaslerCameraManager.cpp'],
			target = 'backend_basler',
			includes = ctx.env.INCLUDES_PYLON,
			uselib = ['JPEG'],
		)

	# Invoke SWIG and generate wrapping source file
	ctx(
		rule = "swig -c++ -python -I../src -o wrapper.cpp ${SRC}",
		source = 'swig_export.i',
		target = ['cap2re.py', 'wrapper.cpp']
	)

	# Build python wrapper
	ctx.shlib(
		source = 'wrapper.cpp',
		target = 'cap2re',
		includes = ['INCLUDES_PYTHON2', './src'],
		uselib = libs + ['PYTHON2'],
		use = baselib + backends
	)

	# Build multithreading test
	ctx.program(
		source = 'tests/mt-test.cpp',
		target = 'mt-test',
		includes = './src',
		uselib = libs,
		use = baselib + backends
	)

	# Build single-thread capture test
	ctx.program(
		source = 'tests/test.cpp',
		target = 'test',
		includes = './src',
		uselib = libs,
		use = baselib + backends
	)
