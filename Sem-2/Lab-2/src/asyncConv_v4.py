#!/usr/bin/env python3

MIN_TIME = 1
MAX_TIME = 50
inputData = 100000

def syncPipeCalc(pipeSize, pipesTiming, slowestPipe):
	syncPipeTime = slowestPipe * (pipeSize + inputData - 1)
	return syncPipeTime

def asyncPipeCalc(pipeSize, pipesTiming, slowestPipe):
	asyncPipeTime = 0
	for i in range(pipeSize):
		asyncPipeTime += pipesTiming[i]
	asyncPipeTime += slowestPipe * (inputData - 1)
	return asyncPipeTime

def drawGraph(y_arr):
	x_arr = [cnt for cnt in range(2,22,2)]
	plt.plot(x_arr, y_arr[0], label="Sync Pipeline")
	plt.plot(x_arr, y_arr[1], label="Async Pipeline")
	# plt.axis([1, 11, 3000000, 7000000])
	plt.ylabel("Pipeline's tacts")
	plt.xlabel("Pipeline's stages")
	plt.legend()
	plt.show()

def main():
	seed(dt.now())
	flg = 0
	slowestPipe = 0
	pipesTiming = [0 for cnt in range(30)]
	pipeTimeArr = [[0 for cnt in range(10)] for cnt in range(2)]
	for i in range(30):
		pipesTiming[i] = rnd(MIN_TIME, MAX_TIME)

	for pipeSize in range(2,22,2):
		for i in range(pipeSize):
			if pipesTiming[i] > slowestPipe:
				slowestPipe = pipesTiming[i]

		pipeTimeArr[0][flg] = syncPipeCalc(pipeSize, pipesTiming, slowestPipe)
		pipeTimeArr[1][flg] = asyncPipeCalc(pipeSize, pipesTiming, slowestPipe)
		print(pipeSize, "\t", pipeTimeArr[0][flg], "\t", pipeTimeArr[1][flg])
		flg += 1
	drawGraph(pipeTimeArr)

if __name__ == '__main__':
	try:
		import sys
		import numpy as np
		from random import randint as rnd, seed
		from datetime import datetime as dt
		from matplotlib import pyplot as plt
	except Exception as err:
		print("Error while loading dependencies:", err)
		exit(-1)
	main()
