#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int MIN_TIME = 1;
const int MAX_TIME = 70;
const int inputData = 1000000;
int slowestPipe = 0;
int pipeSize;

void syncPipeCalc(int *syncPipeTime, int *pipesTiming) {
	for (int i = 0; i < pipeSize; ++i)
		slowestPipe = pipesTiming[i] > slowestPipe ? pipesTiming[i] : slowestPipe;
	*syncPipeTime = slowestPipe * (pipeSize + inputData - 1);
}

void asyncPipeCalc(int *asyncPipeTime, int *pipesTiming) {
	for (int i = 0; i < pipeSize; ++i)
		*asyncPipeTime += pipesTiming[i];
	*asyncPipeTime += slowestPipe * (inputData - 1);
}

int main(int argc, char const *argv[]) {
	srand(time(NULL));
  pipeSize = argv[1] ? atoi(argv[1]) : 5;
  if (pipeSize <= 0) return 253;

	int syncPipeTime = 0;
  int asyncPipeTime = 0;

  int *pipesTiming = (int*)malloc(pipeSize * sizeof(int));
	for (int i = 0; i < pipeSize; ++i) {
		pipesTiming[i] = rand() % (MAX_TIME - MIN_TIME) + MIN_TIME;
    slowestPipe = pipesTiming[i] > slowestPipe
	                              ? pipesTiming[i]
	                              : slowestPipe;
	}
	syncPipeCalc(&syncPipeTime, pipesTiming);
	asyncPipeCalc(&asyncPipeTime, pipesTiming);

  printf("%d\t%d\t%d\n", pipeSize, syncPipeTime, asyncPipeTime);

  return EXIT_SUCCESS;
}
