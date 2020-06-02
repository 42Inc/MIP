//gcc -Wall -g3 -O0 src/readWrite_v4.c -o bin/v4 && ./bin/v4 && ./42graph.sh

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int exp_size = 50;
const int symbols = 26;

void readersCall(float readersData[2][exp_size]) {
	float readerFthread_mean;
	float readerSthread_mean;
	int fthread_local;
	int sthread_local;
	int ps_random;

	for (int attempt = 1; attempt <= exp_size+1; attempt++) {
		readerFthread_mean = 0;
		readerSthread_mean = 0;
		for (int cnt = 1; cnt <= 1001; cnt++) {
			fthread_local = 0;
			sthread_local = 0;
			ps_random = 1;
			while (ps_random != 0) {
				ps_random = rand() % 80;
				if (((1 <= ps_random) && (ps_random < attempt)) && (fthread_local + 1 <= symbols)) {
					fthread_local += 1;
				} else if (((attempt <= ps_random) && (ps_random < (2 * attempt))) && (sthread_local + 1 <= symbols)) {
					sthread_local += 1;
				}
			}
			readerFthread_mean += fthread_local;
			readerSthread_mean += sthread_local;
		}
		readersData[0][attempt - 1] = readerFthread_mean / 1000;
		readersData[1][attempt - 1] = readerSthread_mean / 1000;
	}
}

void dataSave(float data[2][exp_size]) {
	FILE* FReader;
	FILE* SReader;
	char* FFile = "data/fr.dat";
	char* SFile = "data/sr.dat";

	FReader = fopen(FFile	,"w+");
	if (FReader == NULL) {
		exit(EXIT_FAILURE);
	}
	for (int i = 1; i <= exp_size; ++i)
		fprintf(FReader, "%d\t%f\n", i, data[0][i]);
	fclose(FReader);

	SReader = fopen(SFile,"w+");
	if (SReader == NULL) {
		exit(EXIT_FAILURE);
	}
	for (int i = 1; i <= exp_size; ++i)
		fprintf(SReader, "%d\t%f\n", i, data[1][i]);
	fclose(SReader);
}

int main(int argc, char const *argv[]) {
	srand(time(NULL));
	float data[2][exp_size];
	readersCall(data);
	dataSave(data);
	return EXIT_SUCCESS;
}
