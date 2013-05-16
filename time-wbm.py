#!/usr/bin/python

from subprocess import call
import time, sys


args = ["./wbm-admm.out","-w","-m0.0001","-l0.01","-n200"]
for b in [1,2,4,10,20]:
	args.append("-b"+str(b))
	for t in [1,2,5,10,20]:
		args.append("-t"+str(t))
		start_time = time.time()
		call(args)
		print "execution took ",time.time() - start_time, "seconds"
		args.pop()
	args.pop()
