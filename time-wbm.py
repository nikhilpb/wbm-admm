#!/usr/bin/python

from subprocess import call
import time, sys

rho = 5.0

args = ["./wbm-admm.out", "-n10","-w","-b10","-l0.000001","-m0.000001"]
for tc in [1,2,5,10,20,100]:
	args.append("-t" + str(tc))
	start_time = time.time()
	call(args)
	print "execution took ",time.time() - start_time, "seconds"
	args.pop()
