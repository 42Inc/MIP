#!/usr/bin/env python3

import random
import matplotlib.pyplot as plt

max = 26
for a in range(1, 50):
	c1_mid = 0
	c2_mid = 0
	for i in range(1, 1000):
		c1 = 0
		c2 = 0
		rand = 1
		while(rand != 0):
			rand = random.randint(0, 100)
			if(1 <= rand < a and c1+1 <= max):
				c1 += 1
			elif(a <= rand < (2*a) and c2+1 <= max):
				c2 += 1
			else:
				continue
		c1_mid += c1
		c2_mid += c2
	print(c1_mid/1000,'\t', c2_mid/1000)
