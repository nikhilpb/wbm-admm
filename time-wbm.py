#!/usr/bin/python

from subprocess import call
import sys
import time

start_time = time.time()
args = sys.argv
args[0] = "./wbm.out"
call(args)
print "execution took ",time.time() - start_time, "seconds"
