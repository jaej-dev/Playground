#!/usr/bin/env python

import os
import sys

if len(sys.argv) != 3:
    print "usage: gen_src.py source-path output-cmake-path"
    sys.exit(1)

try:
    f = open((sys.argv[2] + '/gen_src.cmake'), 'w')
except:
    print "open failure: ", sys.exc_info()[0]
    sys.exit(1)

filter_out_dirnames = [
    'build',
    'cmake',
    '.git',
    'x86',
    '.setting',
    'build',
]

filter_in_extensions = [
    '.cpp',
    '.cc',
    '.cxx',
    '.C',
    '.h',
    '.hpp',
    '.hh',
    '.H'
]

def is_target_dirname(dirname):
    for f in filter_out_dirnames:
        if f in dirname:
            return False
    return True

def is_target_extension(filename):
    for f in filter_in_extensions:
        if filename.endswith(f):
            return True
    return False

f.write("set (GEN_INCLUDES\n")
line = "\t{}\n".format(sys.argv[1])
f.write(line)
for dirname, dirnames, filenames in os.walk(sys.argv[1]):
    for subdirname in dirnames:
        if (is_target_dirname(dirname) and
            is_target_dirname(subdirname)):
            line = "\t{}\n".format(os.path.join(dirname, subdirname))
            f.write(line)
f.write(")\n")

f.write("set (GEN_SOURCES\n")
for dirname, dirnames, filenames in os.walk(sys.argv[1]):
    for filename in filenames:
        if (is_target_dirname(dirname) and
            is_target_extension(filename)):
            line = "\t{}\n".format(os.path.join(dirname, filename))
            f.write(line)
f.write(")\n")

line = "-I{}".format(sys.argv[1])
for dirname, dirnames, filenames in os.walk(sys.argv[1]):
    for subdirname in dirnames:
        if (is_target_dirname(dirname) and
            is_target_dirname(subdirname)):
            line += "\\n-I{}".format(os.path.join(dirname, subdirname))
f.write("set (CLANG_INCLUDES\n")
f.write(line)
f.write(")\n")

f.close()
