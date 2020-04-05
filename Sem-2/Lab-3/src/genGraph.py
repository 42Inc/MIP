#!/usr/bin/env python3

filePath = "data/graph_10v.dat"		# Output file for generated graph
citiesNum = 10						# Amount of cities (vertices) to generate paths among them
conPerCity = 2						# Number of connections for single city
isCon = 50							# Probability to generate a connection between two cities.
									# The higher this variable then more variants for
									# TSP will be calculated

def genDist():
	probCon = 0
	pathArray = [[-255 for cnt in range(citiesNum)]\
				for cnt in range(citiesNum)]

	# Generating distances for pathArray
	for i in range(citiesNum):
		for j in range(citiesNum):
			if (pathArray[i][j] == -255) and (i != j):
				probCon = rnd(1, 100)
				if probCon <= isCon:
					pathArray[i][j] = rnd(1,9)
					pathArray[j][i] = pathArray[i][j]
				else:
					pathArray[i][j] = 0
					pathArray[j][i] = pathArray[i][j]

	# Looking for the dead cities without any connection
	for i in range(citiesNum):
		isoCity = 0
		for j in range(citiesNum):
			if pathArray[i][j] > 0:
				isoCity = 1
				break
		if isoCity == 0:
			print("Isolated city detected: City -", i)
	saveData(pathArray)

def saveData(pathData):
	with open(filePath, 'w', encoding='utf-8') as graphFile:
		for row in pathData:
			for path in row:
				graphFile.write("%d\t" % path)
			graphFile.write("\n")


if __name__ == '__main__':
	try:
		import sys
		import numpy as np
		from random import randint as rnd, seed
		from datetime import datetime as dt
	except Exception as err:
		print("Error while loading dependencies:", err)
		exit(-1)
	genDist()
