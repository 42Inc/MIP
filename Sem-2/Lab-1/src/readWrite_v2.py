#!/usr/bin/env python3

experiment = 50					# Amount of tries in one experiment
measuring = 1000				# Amount of measurings in one try
alphabet = 26					# Length of the given alphabet (Default: Eng - 26 letters)

def genRnd():
	readersData = [[0 for cnt in range(2)] for cnt in range(experiment - 1)]
	for attempt in range(1, experiment):
		firstReaderAvg = 0		# Average amount of read letters for the first reader's stream
		secondReaderAvg = 0		# Average amount of read letters for the second reader's stream
		for cnt in range(1, measuring):
			firstReader = 0		# Amount of read letters for the first reader's stream on the current measuring
			secondReader = 0	# Amount of read letters for the second reader's stream on the current measuring
			GodBlessRNG = 1		# RND number for deciding when we need to end current measuring
			while (GodBlessRNG != 0):
				GodBlessRNG = rnd.randint(0, 100)
				if (1 <= GodBlessRNG < attempt and firstReader + 1 <= alphabet):
					firstReader += 1
				elif (attempt <= GodBlessRNG < (2 * attempt) and secondReader + 1 <= alphabet):
					secondReader += 1
				else:
					continue
			firstReaderAvg += firstReader
			secondReaderAvg += secondReader
		readersData[attempt - 1][0] = firstReaderAvg / 1000
		readersData[attempt - 1][1] = secondReaderAvg / 1000
	return readersData

def drawGraph():
	pass

def main():
	data = genRnd()
	print(data)
	drawGraph()

if __name__ == '__main__':
	try:
		import sys
		import numpy as np
		import random as rnd
		import matplotlib.pyplot as plt
	except Exception as err:
		print("Error while loading dependencies:", err)
		exit(-1)
	main()
