#! /usr/bin/env python
# encoding: utf-8

from waflib.Configure import conf

def options(opt):
	opt.load('compiler_cxx')
	opt.add_option('--pylon-root', action='store', default='', dest='pylon_root')

@conf
def check_pylon(ctx):
	ctx.load('compiler_cxx')

	hasPylon = True

	root = ctx.options.pylon_root or ctx.env.PREFIX

	libpath = [
		root + '/lib64',
		root + '/genicam/bin/Linux64_x64',
	]

	includes = [
		root + '/include',
		root + '/genicam/library/CPP/include',
	]

	libs = [
		'pylonbase',
		'GenApi_gcc40_v2_1',
		'GCBase_gcc40_v2_1',
		'Log_gcc40_v2_1',
		'log4cpp_gcc40_v2_1',
		'MathParser_gcc40_v2_1',
	]

	for l in libs:
		hasPylon = hasPylon and bool(ctx.check_cxx(lib=l, libpath=libpath, mandatory=False, env=ctx.env.derive()))

	headers = [
		'pylon/PylonIncludes.h',
		'pylon/gige/BaslerGigECamera.h',
	]

	for h in headers:
		hasPylon = hasPylon and bool(ctx.check_cxx(header_name=h, includes=includes, mandatory=False, env=ctx.env.derive()))

	ctx.env.HAVE_PYLON = int(hasPylon)
	ctx.env.PYLON_ROOT = root
	ctx.env.GENICAM_ROOT = root + '/genicam'
	ctx.env.LIBPATH_PYLON = libpath
	ctx.env.INCLUDES_PYLON = includes
	ctx.env.LIB_PYLON = libs


def configure(ctx):
	ctx.start_msg('Checking for Pylon 2.0')
	ctx.check_pylon()
	if ctx.env.HAVE_PYLON:
		ctx.end_msg('yes')
	else:
		ctx.end_msg('not found')