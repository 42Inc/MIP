#!/usr/bin/env python3

routes = []

def load_graph():
	filePath='data/graph_13v.dat'
	with open(filePath, 'r', encoding='utf-8') as graphFile:
		raw_data = graphFile.read().split("\n")
		for row in range(len(raw_data)):
			raw_data[row] = raw_data[row].split("\t")
			raw_data[row] = list(filter(None, raw_data[row]))
			for value in range(len(raw_data[row])):
				raw_data[row][value] = int(raw_data[row][value])
		raw_data = list(filter(None, raw_data))
	data = {city: {node: raw_data[city][node] for node in range(len(raw_data))}\
					for city in range(len(raw_data))}

	for i in range(len(data)):
		for j in range(len(data)):
			if i == j:
				data[i].pop(j)
	return data

def find_paths(node, cities, path, distance):
	# Add way point
	path.append(node)
	# Calculate path length from current to last node
	if len(path) > 1:
		distance += cities[path[-2]][node]

	# If path contains all cities and is not a dead end,
	# add path from last to first city and return.
	if (len(cities) == len(path)) and (path[0] in cities[path[-1]]):
		global routes
		path.append(path[0])
		distance += cities[path[-2]][path[0]]
		print(path, distance)
		routes.append([distance, path])
		return

    # Fork paths for all possible cities not yet used
	for city in cities:
		if (city not in path) and (node in cities[city]):
			find_paths(city, dict(cities), list(path), distance)

def main():
	cities = load_graph()
	stime = t()
	find_paths(0, cities, [], 0)
	routes.sort()
	etime = t() - stime
	if len(routes) != 0:
		print("Shortest route:", routes[0])
		print("Elapsed Time:", etime, "Verticies:",len(cities))
	else:
		print("FAIL!")

if __name__ == '__main__':
	try:
		import sys
		from time import time as t
	except Exception as e:
		exit(e)
	main()

# Elapsed Time: 0.0005338191986083984 Verticies: 5
# Elapsed Time: 9.694021701812744 Verticies: 10
