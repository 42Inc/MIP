#!/usr/bin/env python3

import random
from matplotlib import pyplot as plt

limit = 26
plt_x_vec = [cnt for cnt in range(50)]
stream_1_result = [0 for i in range(50)]
stream_2_result = [0 for j in range(50)]

for alfa in range(1, 51):
	stream_1_mean = 0
	stream_2_mean = 0

	for i in range(1, 1001):
		stream_1 = 0
		stream_2 = 0
		prob = 1

		while (prob != 0):
			prob = random.randint(0, 100)

			if (1 <= prob < alfa and stream_1 + 1 <= limit):
				stream_1 += 1
			elif (alfa <= prob < (2 * alfa) and stream_2 + 1 <= limit):
				stream_2 += 1
			else:
				continue

		stream_1_mean += stream_1
		stream_2_mean += stream_2

	stream_1_result[alfa-1] = stream_1_mean/1000
	stream_2_result[alfa-1] = stream_2_mean/1000

plt.plot(plt_x_vec, stream_1_result, label="Первый поток чтения")
plt.plot(plt_x_vec, stream_2_result, label="Второй поток чтения")
plt.ylabel("Среднее кол-во считанных символов")
plt.xlabel("Массив символов")
plt.legend()
plt.show()
