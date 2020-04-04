#!/usr/bin/env python3
import random
import datetime
import matplotlib.pyplot as plt

min_time = 1
max_time = 5
items_count = 20
j = 0

random.seed(datetime.datetime.now)
stages = [cnt for cnt in range(2,22,2)]
pipeline_times = [0 for cnt in range(30)]
pipeTimeArr = [[0 for cnt in range(10)] for cnt in range(2)]

for pipeline_length in range(2, 22, 2):
	tact = 1
	last_tact = tact
	items_complete = 0
	async_pipeline_time = 0
	last_items_complete = 0
	items_left = items_count
	slowly_pipeline_step_time = 0

	for i in range(pipeline_length):
		pipeline_times[i] = random.randint(min_time, max_time)
		if pipeline_times[i] > slowly_pipeline_step_time:
			slowly_pipeline_step_time = pipeline_times[i]

	sync_pipeline_time = slowly_pipeline_step_time * (pipeline_length + items_count - 1)
	pipeline_status = [[0 for cnt in range(pipeline_length)] for cnt in range(2)]

	for i in range(pipeline_length):
		pipeline_status[0][i] = -1
		pipeline_status[1][i] = 0

	while (True):
		if (last_items_complete != items_complete):
			async_pipeline_time += tact - last_tact
			last_tact = tact
			last_items_complete = items_complete


		if (items_complete == items_count):
			break
		if (pipeline_length == 1):
			if (pipeline_status[0][0] <= 0 and not pipeline_status[1][0] and items_left):
				--items_left
				pipeline_status[0][0] = pipeline_times[0] - 1
				pipeline_status[1][0] = 1
			if (pipeline_status[0][0] <= 0 and pipeline_status[1][0]):
				++items_complete
				pipeline_status[1][0] = 0
		else:
			for i in range(pipeline_length - 1, -1, -1):
				if (i == 0):
					if (pipeline_status[0][i] <= 0 and not pipeline_status[1][i] and items_left):
						--items_left
						pipeline_status[0][i] = pipeline_times[i] - 1
						pipeline_status[1][i] = 1
				elif (i == pipeline_length - 1):

					if (pipeline_status[0][i] <= 0 and pipeline_status[1][i]):
						items_complete += 1
						pipeline_status[1][i] = 0

					if (not pipeline_status[1][i]):
						if (pipeline_status[0][i - 1] <= 0 and pipeline_status[1][i - 1]):
							pipeline_status[1][i - 1] = 0
							pipeline_status[1][i] = 1
							pipeline_status[0][i] = pipeline_times[i]
				else:
					if (not pipeline_status[1][i]):
						if (pipeline_status[0][i - 1] <= 0 and pipeline_status[1][i - 1]):
							pipeline_status[1][i - 1] = 0
							pipeline_status[1][i] = 1
							pipeline_status[0][i] = pipeline_times[i]

		for i in range(pipeline_length):
			pipeline_status[0][i]-=1
		tact+=1

	print(pipeline_length, "\t", sync_pipeline_time, "\t", async_pipeline_time)
	pipeTimeArr[0][j] = sync_pipeline_time
	pipeTimeArr[1][j] = async_pipeline_time
	j += 1

plt.plot(stages, pipeTimeArr[0], label="Sync Pipeline")
plt.plot(stages, pipeTimeArr[1], label="Async Pipeline")
plt.ylabel("Pipeline's tacts")
plt.xlabel("Pipeline's stages")
plt.legend()
plt.show()
