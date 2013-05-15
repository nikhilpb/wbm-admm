#!/usr/bin/python

from subprocess import call
import time

start_time = time.time()
call(["./wbm.out"])
print "execution took ",time.time() - start_time, "seconds"
