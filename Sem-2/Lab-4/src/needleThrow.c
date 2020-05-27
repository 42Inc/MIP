#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

const double PI = 3.14159265358979323846;
const int n = 100000;

double wtime() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

double getrand() { return (double)rand() / RAND_MAX; }

int main(int argc, char **argv) {
  int in = 0;
  int i = 0;
  double s = 0;
  double distance = 4;
  double length = 2;

  double t = wtime();

  for (i = 0; i < n; ++i) {
    double x = getrand() * 2 * distance;
    double alpha = getrand();
    if (length * sin(alpha) < x) {
      ++in;
    }
  }

  t = wtime() - t;
  double theor = 2 * length / (distance * PI);
  double pract = (double)(in) / n;
  double res = fabs(theor - pract);

  printf("Result: %.12f, n: %d\ntime = %.6lf\n", res, n,
         t);
  return EXIT_SUCCESS;
}
