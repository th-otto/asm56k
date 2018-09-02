#! /usr/bin/env python

from fnmatch import fnmatch
import os
import subprocess
from os.path import join, getsize

def print_fnmatches(pattern, dir, files):
    for filename in files:
        if fnmatch(filename, pattern):

			ret = os.system( "./asm56k " + "-I../../test " + "-o "+ filename.replace('.asm','.lod') + " -p "+ filename.replace('.asm','.p56') + " " + os.path.join(dir, filename)  + " >> log.txt" )
			
			if filename.rfind("fail") == -1:
				if ret == 0:
					ret = 1
				else:
					ret = 0
				
			if ret == 0:
				print "----------------------------------------------------------------"
				print "FAILED: " + os.path.join(dir, filename)
				print "----------------------------------------------------------------"		
			else:		
				print "PASSED: " + os.path.join(dir, filename)	
				
if os.system( "../build") == 0:
	os.path.walk('../../test', print_fnmatches, '*.asm')
