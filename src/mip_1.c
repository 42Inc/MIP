#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CHECK(val,min,max) (min <= val && val < max)

int main(int argc, char **argv) {
  const double P_write = 0.01;
  const double P_read = P_write;
  int P_read_count = argv[1] ? atoi(argv[1]) : 2;
  double a = 1.0;
  long int size = 33;
  long int i = 0;
  long int j = 0;
  double gen = 0.0;
  long int read = 0;
  double mean = 0.0;
  long int repeat = 1000000;
  double max_a = 20;
  long int success = 0;
  srand(time(NULL));
  for (a = 1; a <= max_a; a+=0.1) {
    mean = 0.0;
    success = 0;
    if ((P_write + a * P_read_count * P_read) > 1)
      break;
    for (i = 0; i < repeat; ++i) {
      read = 0;
      for (j = 0; j < size; ++j) {
        gen = ((double)(rand() % 10000)) / 10000;
//        fprintf(stdout, "[1][gen: %lf | a: %lf | P_write: %lf | Max: %lf | check: %ld]\n", gen, a, P_write, (P_write + P_read_count * P_read * a), CHECK(gen, P_write, (P_write + P_read_count * P_read * a)));
        if (CHECK(gen, 0, P_write)) {
          break;
        } else if (CHECK(gen, P_write, (P_write + P_read_count * P_read * a))) {
          ++read;
          continue;
        }
      }
      if (j == size)
        ++success;
      mean += read;
//      fprintf(stdout, "[1][P_write: %lf | P_read: %lf | a: %lf | read: %ld | mean: %lf]\n", P_write, P_read, a, read, mean);
    }
    mean = ((double)mean) / repeat;
    fprintf(stdout, "[P_write: %lf | P_read: %lf | P_read_count: %d | a: %lf | state: %ld/%ld | mean: %lf]\n", P_write, P_read, P_read_count, a, success, repeat, mean);
    fprintf(stderr, "%lf\t%ld\n",a,success);
  }
  return 0;
}
