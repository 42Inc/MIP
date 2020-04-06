#!/usr/bin/env python3

shortestPath = 1e10					# Shortest path for TSP.
									# Must be as much as possible at start.
filePath = "data/graph_10v.dat"		# Path to the file with
									# graph generated by genTSP.py

def loadGraph():
	with open(filePath, 'r', encoding='utf-8') as graphFile:
		rawData = graphFile.read().split("\n")
		for row in range(len(rawData)):
			rawData[row] = rawData[row].split("\t")
			rawData[row] = list(filter(None, rawData[row]))
			for value in range(len(rawData[row])):
				rawData[row][value] = int(rawData[row][value])
		rawData = list(filter(None, rawData))
	return rawData


def main():
	graphData = loadGraph()

if __name__ == '__main__':
	try:
		import sys
		import numpy as np
		from random import randint as rnd, seed
		from datetime import datetime as dt
	except Exception as err:
		print("Error while loading dependencies:", err)
		exit(-1)
	main()
