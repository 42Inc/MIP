#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

const int l = 2, a = 4;
const double M_PI = 3.14;

double gen_rand_fi(double min, double max) {
	double f = (double)rand() / RAND_MAX;
	return min + f * (max - min);
}

double buffon_task(int cnt_drops) {
	int cnt_success_drops = 0;
	for (int i = 0; i < cnt_drops; i++) {
		int x = rand() % a;
		double fi = gen_rand_fi(0, M_PI);
		double func_value = l * sin(fi);
		if (x <= func_value) {
			cnt_success_drops++;
		}
	}
	return (double) cnt_success_drops / cnt_drops;
}

int main() {
	double theory_prop = (2.0 * l) / (a * M_PI);
	for (int i = 1; i <= 100000; i = i*10) {
		double practical_prop = buffon_task(i);
		double delta = fabs(theory_prop - practical_prop) / theory_prop;
		printf("%d\t%f\n", i, delta);
	}
	return EXIT_SUCCESS;
}
