#!/usr/bin/python

from subprocess import call
import time, sys


args = ["./wbm-admm.out","-w","-r10.0","-m0.0001","-l0.01","-b5"]
for n in [300,400,500]:
	args.append("-n"+str(n))
	for t in [40]:#[1,2,5,10,20,40]:
		args.append("-t"+str(t))
		start_time = time.time()
		call(args)
		print "execution took ",time.time() - start_time, "seconds"
		args.pop()
	args.pop()
