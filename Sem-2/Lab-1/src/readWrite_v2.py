#!/usr/bin/env python3

experiment = 50					# Amount of tries in one experiment
measuring = 1000				# Amount of measurings in one try
alphabet = 26					# Length of the given alphabet (Default: Eng - 26 letters)

def genRnd():
	readersData = [[0 for cnt in range(experiment)] for cnt in range(2)]
	for attempt in range(1, experiment + 1):
		firstReaderAvg = 0		# Average amount of read letters for the first reader's stream
		secondReaderAvg = 0		# Average amount of read letters for the second reader's stream
		for cnt in range(1, measuring + 1):
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
		readersData[0][attempt - 1] = firstReaderAvg / 1000
		readersData[1][attempt - 1] = secondReaderAvg / 1000
	return readersData

def drawGraph(y_arr):
	x_arr = [cnt for cnt in range(experiment)]			# Array of values similar to alphabet. Made for better graph performance
	plt.plot(x_arr, y_arr[0], label="First reader's stream")
	plt.plot(x_arr, y_arr[1], label="Second reader's stream")
	plt.ylabel("Average amount of read letters")
	plt.xlabel("Alphabet")
	plt.legend()
	plt.show()

def main():
	data = genRnd()
	drawGraph(data)

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
