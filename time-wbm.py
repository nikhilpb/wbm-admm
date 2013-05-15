#!/usr/bin/python

from subprocess import call
import time, sys

args = sys.argv
args[0] = "./lp"
start_time = time.time()
call(args)
print "execution took ",time.time() - start_time, "seconds"
