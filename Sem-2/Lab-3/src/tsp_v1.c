#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <algorithm>

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

struct Coords {
	int x;
	int y;
} T;

int linearSearch(int* c, int l, int T) {
	for (int i = 0; i < l; ++i) {
		if (c[i] == T) {
			return i;
		}
	}
	return l;
}

float distance(struct Coords T1, struct Coords T2) {
	return sqrt(pow((T1.x - T2.x), 2) + pow((T1.y - T2.y), 2));
}

int main() {
	double t1, t2;
	int l;
	float min = 0;
	float summ = 0;
	printf(" Amount of points: ");
	scanf("%d\n", &l);
	t1 = wtime();
	int *c = malloc(l * sizeof(int));
	int *I, *R;
	int *I = malloc((l - 1) * sizeof(int));
	int *R = malloc((l - 1) * sizeof(int));
	for (int i = 0; i < l - 1; ++i) {
		I[i] = i + 1;
	}
	for (int i = 0; i < l; ++i) {
		T.x = rand() % 100;
		T.y = rand() % 100;

		int p;

		p = linearSearch(c, l, T);
		for (; p != c.end();) {
			T.x = rand() % 100;
			T.y = rand() % 100;
			p = linearSearch(c, l, T);
		}
		c[i] = T;
	}
	for (int i = 0; i < l - 1; ++i) {
		R[i] = I[i];
		if (i != l - 2) {
			summ += distance(c[I[i]], c[I[i + 1]]);
		}
	}
	summ += distance(c[0], c[I[0]]);
	summ += distance(c[0], c[I[l - 2]]);
	printf("summ = %.2f\n", summ);
	min = summ;
	// I
	// &(I[l-1])
	for (; std::next_permutation(I, I + l - 1);) {
		summ = 0;
		for (int i = 0; i < l - 1; ++i)
			if (i != l - 2)
				summ += distance(c[I[i]], c[I[i + 1]]);
		summ += distance(c[0], c[I[0]]);
		summ += distance(c[0], c[I[l - 2]]);
		if (summ < min) {
			min = summ;
			for (int i = 0; i < l - 1; ++i) {
				R[i] = I[i];
			}
		}
	}

	printf("min = %f\n", min);
	t2 = wtime() - t1;
	printf("time = %.5f\n", t2);
}
