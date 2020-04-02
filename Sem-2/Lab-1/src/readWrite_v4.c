//gcc -Wall -g3 -O0 src/readWrite_v4.c -o bin/v4 && ./bin/v4 && ./42graph.sh

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int experiment = 50;					// Amount of tries in one experiment
const int measuring = 1000;					// Amount of measurings in one try
const int alphabet = 26;						// Length of the given alphabet (Default: Eng - 26 letters)

void readersCall(float readersData[2][experiment]) {
	int firstReaderAvg;
	int secondReaderAvg;
	int firstReader;
	int secondReader;
	int GodBlessRNG;

	for (int attempt = 1; attempt <= experiment+1; attempt++) {
		firstReaderAvg = 0;						// Average amount of read letters for the first reader's stream
		secondReaderAvg = 0;					// Average amount of read letters for the second reader's stream
		for (int cnt = 1; cnt <= measuring+1; cnt++) {
			firstReader = 0;						// Amount of read letters for the first reader's stream on the current measuring
			secondReader = 0;						// Amount of read letters for the second reader's stream on the current measuring
			GodBlessRNG = 1;						// RND number for deciding when we need to end current measuring
			while (GodBlessRNG != 0) {
				GodBlessRNG = rand() % 100;
				if (((1 <= GodBlessRNG) && (GodBlessRNG < attempt)) && (firstReader + 1 <= alphabet)) {
					firstReader += 1;
				} else if (((attempt <= GodBlessRNG) && (GodBlessRNG < (2 * attempt))) && (secondReader + 1 <= alphabet)) {
					secondReader += 1;
				}
			}
			firstReaderAvg += firstReader;
			secondReaderAvg += secondReader;
		}
		readersData[0][attempt - 1] = firstReaderAvg / 1000;
		readersData[1][attempt - 1] = secondReaderAvg / 1000;
	}
}

void dataSave(float data[2][experiment]) {
	FILE* firstReader;
	FILE* secondReader;
	char* firstFile = "data/fr.dat";
	char* secondFile = "data/sr.dat";

	firstReader = fopen(firstFile	,"w+");
	if (firstReader == NULL) {
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < experiment; ++i)
		fprintf(firstReader, "%d\t%f\n", i, data[0][i]);
	fclose(firstReader);

	secondReader = fopen(secondFile,"w+");
	if (secondReader == NULL) {
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < experiment; ++i)
		fprintf(secondReader, "%d\t%f\n", i, data[1][i]);
	fclose(secondReader);
}

int main(int argc, char const *argv[]) {
	srand(time(NULL));
	float data[2][experiment];
	readersCall(data);
	dataSave(data);
	return EXIT_SUCCESS;
}
