#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

/*
 * Варианты:
 * Метод ветвей и границ
 * Метод ближайших соседей
 *
 * Допущения:
 * Граф считать полносвязным
 */

#define LOG_FILE 0
#define PRINT_WAYS 0

#define TRUE 0
#define FALSE 1

void SWAP(int* x, int* y) {
  int tmp = *x;
  *x = *y;
  *y = tmp;
}

static FILE* logger = NULL;
char* log_filename = "v2.log";

double wtime() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

long int factorial(long int a) {
  long int i = 0;
  long int k = 1;
  if (a == 0 || a == 1) {
    return k;
  } else if (a > 0) {
    for (i = a; i > 0; --i) {
      k = k * i;
    }
    return k;
  }
  return 0;
}

long int get_way_len(int** matrix, int* way, int n) {
  int i = 0;
  int sum = 0;
  for (i = 1; i <= n; ++i) {
    sum += matrix[way[i - 1]][way[i]];
  }
  return sum;
}

int get_row_min(int** matrix, int n, int row, int greater_or_equal_zero) {
  int i = 0;
  int min = __INT32_MAX__;
  for (i = 0; i < n; ++i) {
    if ((greater_or_equal_zero && matrix[row][i] > 0) ||
        (!greater_or_equal_zero && matrix[row][i] >= 0)) {
      if (matrix[row][i] < min) min = matrix[row][i];
    }
  }
  return min;
}

int get_col_min(int** matrix, int n, int col, int greater_or_equal_zero) {
  int i = 0;
  int min = __INT32_MAX__;
  for (i = 0; i < n; ++i) {
    if ((greater_or_equal_zero && matrix[i][col] > 0) ||
        (!greater_or_equal_zero && matrix[i][col] >= 0)) {
      if (matrix[i][col] < min) min = matrix[i][col];
    }
  }
  return min;
}

/* Алгоритм Нарайаны для получения следующей перестановки */
int get_next_permutation(int* array, int start, int end) {
  int i = 0;
  int j = 0;
  int size = end - start + 1;
  int* a = NULL;
  int tmp = 0;
  if (size <= 0 || start < 0) return 1;
  a = array + start;
  i = size;
  do {
    /* Условие завершения перебора - обратно упорядоченная последовательность*/
    if (i < 2) return 1;
    --i;
  } while (!(a[i - 1] < a[i]));
  for (j = size; j > i && !(a[i - 1] < a[--j]);)
    ;
  SWAP(&a[i - 1], &a[j]);

  for (j = size; i < --j;) {
    SWAP(&a[i++], &a[j]);
  }
  return 0;
}

/* http://www.lib.tpu.ru/fulltext/v/Bulletin_TPU/2004/v307/i6/03.pdf */
/* Или "Как получить лексикографическую перестановку по её индексу" */
int get_permutation_by_index(int* array, int start, int end, long int index,
                             int* res) {
  long int k = 0;
  long int t = 1;
  long int i = 0;
  int size = end - start + 1;
  int* a = NULL;
  long int tmp = 0;
  if (size <= 0 || start < 0 || index < 0) return 1;
  a = malloc(size * sizeof(int));
  if (!a) return 1;
  memcpy(a, array + start, size * sizeof(int));
  ++index;
  for (t = 1; t <= size; ++t) {
    tmp = factorial(size - t);
    k = (index + tmp - 1) / tmp;
    res[t - 1 + start] = a[k - 1];
    for (i = k - 1; i < size; ++i) {
      a[i] = a[i + 1];
    }
    index = index - (k - 1) * tmp;
  }
  return 0;
}

/* Метод полного перебора. Возвращает путь и индекс */
int* brute_force(int** matrix, int n, long int* id) {
  int i = 0;
  int j = 0;
  /* Номер пути */
  long int way_index = 0;
  long int min_way_index = 0;
  /* Длина пути */
  long int way_len = 0;
  long int min_way_len = __INT64_MAX__;

  /* Путь */
  int* way = NULL;
  int* start_way = NULL;
  int* res_way = NULL;
  /*
   * Если размерность матрицы меньше или 1 - ошибка.
   * Меньше единицы, потому что Хоббит - Туда и Обратно
   * Если указатель - NULL, тоже.
   */
  if (n <= 1 || !matrix) return NULL;

  for (i = 0; i < n; ++i)
    if (!matrix[i]) return NULL;

  /* Выделение памяти под массив хранения пути */
  way = (int*)malloc((n + 1) * sizeof(int));
  start_way = (int*)malloc((n + 1) * sizeof(int));
  res_way = (int*)malloc((n + 1) * sizeof(int));
  /* Не выделилась память */
  if (!way || !start_way || !res_way) return NULL;

  for (i = 0; i <= n; ++i) {
    start_way[i] = i % n;
  }

  /* Сохраняем стартовый путь. Просто чтоб был */
  memcpy(way, start_way, (n + 1) * sizeof(int));
  memcpy(res_way, start_way, (n + 1) * sizeof(int));

#if PRINT_WAYS == 1
  fprintf(logger, "Start way:\n");
#endif
  for (i = 0; i <= n; ++i) {
    start_way[i] = i % n;
#if PRINT_WAYS == 1
    fprintf(logger, "%d ", start_way[i]);
#endif
  }
#if PRINT_WAYS == 1
  fprintf(logger, "\n");
#endif

  if (n == 2) return way;

  /*
   * Перебор всех перестановок для индексов [1:n - 1]
   * по индексу перестановки
   * Количество перестановок = (n-1)!
   * Вывод минимальной перестановки по её индексу
   */
  do {
#if PRINT_WAYS == 1
    fprintf(logger, "Way %ld [ ", way_index);
    for (i = 0; i <= n; ++i) {
      fprintf(logger, "%d ", way[i]);
    }
    fprintf(logger, "]\n");
#endif
    way_len = get_way_len(matrix, way, n);
#if PRINT_WAYS == 1
    fprintf(logger, "Way Len : %ld", way_len);
    fprintf(logger, "\n");
#endif
    if (way_len < min_way_len) {
      min_way_len = way_len;
      min_way_index = way_index;
    }
    ++way_index;
  } while (!get_next_permutation(way, 1, n - 1));

  if (get_permutation_by_index(start_way, 1, n - 1, min_way_index, res_way)) {
    return NULL;
  }
  *id = min_way_index;
#if PRINT_WAYS == 1
  fprintf(logger, "Min Way %ld [ ", min_way_index);
  for (i = 0; i <= n; ++i) {
    fprintf(logger, "%d ", res_way[i]);
  }
  fprintf(logger, "]\n");
  fprintf(logger, "Min Way Len : %ld", min_way_len);
  fprintf(logger, "\n");
#endif
  free(start_way);
  free(way);
  return res_way;
}

/* Метод ветвей и границ */
int* branch_and_bound(int** matrix, int size) {
  int i = 0;
  int j = 0;
  int k = 0;
  int col_min = __INT32_MAX__;
  int row_min = __INT32_MAX__;

  /* Копируем размер матрицы, потому что он будет изменяться */
  int n = size;
  /* Индекс заполненности пути */
  int ways_index = 0;
  /* Нижняя граница */
  int low_border = 0;

  /* Копия исходной матрицы */
  int** matrix_c = NULL;
  /* Матрица оценок */
  int** matrix_temp = NULL;
  /* Матрица путей src->dst */
  int** ways = NULL;
  /* Минимум по строкам */
  int* alpha = NULL;
  /* Минимум по столбцам */
  int* beta = NULL;

  /*
   * Если размерность матрицы меньше или 1 - ошибка.
   * Меньше единицы, потому что Хоббит - Туда и Обратно
   * Если указатель - NULL, тоже.
   */
  if (n <= 1 || !matrix) return NULL;

  /* Выделение памяти под матрицу и вектора */
  matrix_c = (int**)malloc(n * sizeof(int*));
  matrix_temp = (int**)calloc(n, sizeof(int*));
  ways = (int**)calloc(n, sizeof(int*));

  alpha = (int*)malloc(n * sizeof(int));
  beta = (int*)malloc(n * sizeof(int));

  /* Не выделилась память */
  if (!matrix_c || !alpha || !beta || !matrix_temp || !ways) return NULL;

  for (i = 0; i < n; ++i) {
    matrix_c[i] = (int*)malloc(n * sizeof(int));
    /* Не выделилась память */
    if (!matrix_c[i]) {
      for (j = 0; j < i; ++j) free(matrix_c[j]);
      free(matrix_c);
      return NULL;
    }
  }

  for (i = 0; i < n; ++i) {
    matrix_temp[i] = (int*)calloc(n, sizeof(int));
    /* Не выделилась память */
    if (!matrix_temp[i]) {
      for (j = 0; j < i; ++j) free(matrix_temp[j]);
      free(matrix_temp);
      return NULL;
    }
  }

  for (i = 0; i < n; ++i) {
    ways[i] = (int*)calloc(2, sizeof(int));
    /* Не выделилась память */
    if (!ways[i]) {
      for (j = 0; j < i; ++j) free(ways[j]);
      free(ways);
      return NULL;
    }
  }

  /* Копируем матрицу. Из-за специфики хранения - построчно */
  for (i = 0; i < n; ++i) {
    memcpy(matrix_c[i], matrix[i], n * sizeof(int));
  }

  low_border = 0;
  /* Массив минимальных элементов в строке */
  for (i = 0; i < n; ++i) {
    alpha[i] = get_row_min(matrix_c, n, i, 0);
  }

  /* Вычитаем минимальный элемент строки из каждой строки */
  for (i = 0; i < n; ++i) {
    for (j = 0; j < n; ++j) {
      if (j != i) matrix_c[i][j] = matrix_c[i][j] - alpha[i];
    }
  }

  /* Массив минимальных элементов в столбце */
  for (i = 0; i < n; ++i) {
    beta[i] = get_col_min(matrix_c, n, i, 0);
  }

  /* Вычитаем минимальный элемент столбца из каждого столбца */
  for (i = 0; i < n; ++i) {
    for (j = 0; j < n; ++j) {
      if (j != i) matrix_c[i][j] = matrix_c[i][j] - beta[j];
    }
  }

  /* Считаем нижнюю границу */
  for (i = 0; i < n; ++i) low_border = low_border + alpha[i] + beta[i];
  /* Строим матрицу оценок */
  for (i = 0; i < n; ++i) {
    for (j = 0; j < n; ++j) {
      if (matrix_c[i][j] == 0) {
        for (k = 0; k < n; ++k) {
          if ((j != k) && (matrix_c[i][k] >= 0) && (matrix_c[i][k] < row_min))
            row_min = matrix_c[i][k];
        }
        for (k = 0; k < n; ++k) {
          if ((i != k) && (matrix_c[k][j] >= 0) && (matrix_c[k][j] < col_min))
            col_min = matrix_c[k][j];
        }
        matrix_temp[i][j] = row_min + col_min;
      } else {
        matrix_temp[i][j] = 0;
      }
    }
  }

  /* Печать матрицы, alpha, beta, lb */
  for (i = 0; i < n; ++i) {
    for (j = 0; j < n; ++j) fprintf(stderr, "%d\t", matrix_c[i][j]);
    fprintf(stderr, "\n");
  }
  fprintf(stderr, "\n");
  for (i = 0; i < n; ++i) {
    for (j = 0; j < n; ++j) fprintf(stderr, "%d\t", matrix_temp[i][j]);
    fprintf(stderr, "\n");
  }
  fprintf(logger, "alpha [ ");
  for (i = 0; i < n; ++i) {
    fprintf(logger, "%d ", alpha[i]);
  }
  fprintf(stderr, "]\n");
  fprintf(logger, "beta [ ");
  for (i = 0; i < n; ++i) {
    fprintf(logger, "%d ", beta[i]);
  }
  fprintf(logger, "]\n");
  fprintf(logger, "lb : %d\n", low_border);

  free(alpha);
  free(beta);
  for (i = 0; i < n; ++i) {
    free(matrix_c[i]);
  }
  free(matrix_c);
  for (i = 0; i < n; ++i) {
    free(matrix_temp[i]);
  }
  free(matrix_temp);
  return NULL;
}

int main(int argc, char** argv) {
  int i = 0;
  int j = 0;

  long int way_index = 0;

  char* filename = "matrix.dat";
  FILE* fd = NULL;

  double time = 0;

  /* Размер матрицы - по умолчанию 5 */
  int n = argv[1] ? atoi(argv[1]) : 5;
  /* Если передан файл */
  if (argv[2]) filename = argv[2];

  /* Матрица */
  int** matrix = NULL;
  int* brute_way = NULL;

/* Инициализация логгера */
#if LOG_FILE == 1
  if (!(logger = fopen(log_filename, "w"))) {
    logger = stderr;
  }
#else
  logger = stderr;
#endif

  /* Выделение памяти под матрицу */
  matrix = (int**)malloc(n * sizeof(int*));
  /* Не выделилась память */
  if (!matrix) return 255;

  for (i = 0; i < n; ++i) {
    matrix[i] = (int*)malloc(n * sizeof(int));
    /* Не выделилась память */
    if (!matrix[i]) {
      for (j = 0; j < i; ++j) free(matrix[j]);
      free(matrix);
      return 254;
    }
  }

  /* Чтение из файла */
  /* Ошибка открытия файла */
  if (!(fd = fopen(filename, "r"))) {
    return 253;
  }

  /* Читаем матрицу */
  for (i = 0; i < n; ++i) {
    for (j = 0; j < n; ++j) {
      fscanf(fd, "%d", &matrix[i][j]);
    }
  }

  /* Печать матрицы */
  for (i = 0; i < n; ++i) {
    for (j = 0; j < n; ++j) fprintf(stderr, "%d\t", matrix[i][j]);
    fprintf(stderr, "\n");
  }
  time = wtime();
  brute_way = brute_force(matrix, n, &way_index);
  time = wtime() - time;
  if (!brute_way) {
    return 252;
  }

  fprintf(stderr, "Res brute Min Way %ld [ ", way_index);
  for (i = 0; i <= n; ++i) {
    fprintf(stderr, "%d ", brute_way[i]);
  }
  fprintf(stderr, "]\n");
  fprintf(stderr, "Res brute Way Len : %ld", get_way_len(matrix, brute_way, n));
  fprintf(stderr, "\n");
  fprintf(stderr, "Brute time : %lf\n", time);
  free(brute_way);

  branch_and_bound(matrix, n);

  for (i = 0; i < n; ++i) {
    free(matrix[i]);
  }
  free(matrix);
  return 0;
}