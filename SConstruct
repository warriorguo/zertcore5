import SCons.Action
import SCons.Builder
import SCons.Scanner.C
import SCons.Util
import SCons.Script
import os
import functools

extra_ccflags = '-Wall -std=c++11'

env = Environment(CXX = '/opt/rh/devtoolset-2/root/usr/bin/gcc',
	CPPPATH = ['./','../../header','/opt/mongodb/mongo-cxx-driver-nightly/src'],
	LIBPATH = [""],
	LIBS = ['z', 'boost_regex', 'boost_locale', 'boost_thread', 'boost_filesystem', 'boost_program_options', 'cryptopp', 'mongoclient', 'pthread', 'rt', 'curl'],
	CCFLAGS = extra_ccflags)

src_files = []

for root, dirs, files in os.walk('./'):
	"""
	if (os.path.exists(root + "/SConstruct")):
		SConscript(root + "/SConstruct")
	"""
	if (root.find("test") != -1):
		continue
	if (root.find("save") != -1):
		continue
		
	src_files += Glob(root + '/*.cpp')

env.StaticLibrary('zertcore', src_files)
