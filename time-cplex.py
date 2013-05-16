#!/usr/bin/python

from subprocess import call
import time, sys


args = ["./wbm-cplex.out"]
for n in [100,200,300,400,500]:
	args.append(str(n))
	start_time = time.time()
	call(args)
	print "execution took ",time.time() - start_time, "seconds"
	args.pop()
