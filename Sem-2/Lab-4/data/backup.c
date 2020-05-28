#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

const double PI = 3.14159265358979323846;
int n = 1000000;

double wtime() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

double getrand() { return (double)rand() / RAND_MAX; }

int main(int argc, char **argv) {
  int in = 0;
  int i = 0;
  n = argv[1] ? atoi(argv[1]) : n;
  double s = 0;
  double distance = 4;
  double length = 2;
  srand(time(NULL));
  for (i = 0; i < n; ++i) {
    double x = getrand() * distance;
    double alpha = getrand() * PI;

    if (x <= length * sin(alpha)) {
      ++in;
    }
  }

  double theor = 2 * length / (distance * PI);
  double pract = (double)(in) / n;
  double res = fabs(theor - pract) / theor;

  printf("%.12f\t%d\t%lf\t%lf\n", res, n, theor, pract);
  return EXIT_SUCCESS;
}
