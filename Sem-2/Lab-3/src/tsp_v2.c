#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

/*
 * Варианты:
 * Полный перебор
 * Метод ветвей и границ
 *
 * Допущения:
 * Граф считать полносвязным
 */

#define LOG_FILE 0
#define PRINT_WAYS 0
#define SAVE_INDEX 0

#define TRUE 0
#define FALSE 1

#define IND(i, j, s) (i * s + j)

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

#if SAVE_INDEX == 1
unsigned long long int factorial(long long int a) {
  unsigned long long int i = 0;
  unsigned long long int k = 1;
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
#endif

long int get_way_len(int* matrix, int* way, int n) {
  int i = 0;
  int sum = 0;
  for (i = 1; i <= n; ++i) {
    sum += matrix[IND(way[i - 1], way[i], n)];
  }
  return sum;
}

int get_row_min(int* matrix, int n, int row, int greater_or_equal_zero) {
  int i = 0;
  int min = __INT32_MAX__;
  for (i = 0; i < n; ++i) {
    if ((greater_or_equal_zero && matrix[IND(row, i, n)] > 0) ||
        (!greater_or_equal_zero && matrix[IND(row, i, n)] >= 0)) {
      if (matrix[IND(row, i, n)] < min) min = matrix[IND(row, i, n)];
    }
  }
  return min;
}

int get_col_min(int* matrix, int n, int col, int greater_or_equal_zero) {
  int i = 0;
  int min = __INT32_MAX__;
  for (i = 0; i < n; ++i) {
    if ((greater_or_equal_zero && matrix[IND(i, col, n)] > 0) ||
        (!greater_or_equal_zero && matrix[IND(i, col, n)] >= 0)) {
      if (matrix[IND(i, col, n)] < min) min = matrix[IND(i, col, n)];
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
  if (size <= 0 || start < 0) return 1;
  a = array + start;
  i = size;
  do {
    /* Условие завершения перебора - обратно упорядоченная последовательность */
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

#if SAVE_INDEX == 1
/* http://www.lib.tpu.ru/fulltext/v/Bulletin_TPU/2004/v307/i6/03.pdf */
/* Или "Как получить лексикографическую перестановку по её индексу" */
int get_permutation_by_index(int* array, int start, int end, long int index,
                             int* res) {
  int size = end - start + 1;
  int* a = NULL;
  unsigned long long int k = 0;
  unsigned long long int t = 1;
  unsigned long long int i = 0;
  unsigned long long int tmp = 0;
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
#endif

/* Метод полного перебора. Возвращает путь и индекс */
int* brute_force(int* matrix, int n, long int* id) {
  int i = 0;
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
#if SAVE_INDEX == 1
      min_way_index = way_index;
#else
      memcpy(res_way, way, (n + 1) * sizeof(int));
#endif
    }
    ++way_index;
  } while (!get_next_permutation(way, 1, n - 1));

#if SAVE_INDEX == 1
  if (get_permutation_by_index(start_way, 1, n - 1, min_way_index, res_way)) {
    return NULL;
  }
  *id = min_way_index;
#else
  *id = -1;
#endif
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

/* Редуцирование матрицы */
int reduction_matrix(int* matrix, int n) {
  int i = 0;
  int j = 0;
  /* Минимум по строкам */
  int* alpha = NULL;
  /* Минимум по столбцам */
  int* beta = NULL;
  /* Нижняя граница */
  int low_border = 0;

  alpha = (int*)malloc(n * sizeof(int));
  beta = (int*)malloc(n * sizeof(int));

  /* Не выделилась память */
  if (!alpha || !beta) return -1;

  /* Массив минимальных элементов в строке */
  for (i = 0; i < n; ++i) {
    alpha[i] = get_row_min(matrix, n, i, 0);
    if (alpha[i] == __INT32_MAX__) alpha[i] = 0;
  }

  /* Вычитаем минимальный элемент строки из каждой строки */
  for (i = 0; i < n; ++i) {
    for (j = 0; j < n; ++j) {
      if (matrix[IND(i, j, n)] >= 0)
        matrix[IND(i, j, n)] = matrix[IND(i, j, n)] - alpha[i];
    }
  }

  /* Массив минимальных элементов в столбце */
  for (i = 0; i < n; ++i) {
    beta[i] = get_col_min(matrix, n, i, 0);
    if (beta[i] == __INT32_MAX__) beta[i] = 0;
  }

  /* Вычитаем минимальный элемент столбца из каждого столбца */
  for (i = 0; i < n; ++i) {
    for (j = 0; j < n; ++j) {
      if (matrix[IND(i, j, n)] >= 0)
        matrix[IND(i, j, n)] = matrix[IND(i, j, n)] - beta[j];
    }
  }

  /* Считаем нижнюю границу */
  for (i = 0; i < n; ++i) low_border = low_border + alpha[i] + beta[i];
  return low_border;
}

/* Ищем замыкания в пути */
int loop_check(int* way, int step, int n) {
  int i = 0;
  int j = 0;
  int k = 0;
  int start = 0;
  int cursor = 0;
  k = 0;
  start = k;
  cursor = start;
  for (i = 0; i < step; ++i) {
    for (j = 0; j < step; ++j) {
      if (cursor == way[IND(j, 0, 2)]) {
        cursor = way[IND(j, 1, 2)];
        ++k;
      }
      if ((i > 0) & (cursor == start)) {
        if (k == n) return 0;
        return 1;
      }
    }
  }
  return 0;
}

/* Рекурсия метода ветвей и границ */
int* recursive_branch_and_bound(int* matrix, int size, int lb, int level,
                                int step, int* way) {
  int i = 0;
  int j = 0;
  int k = 0;
  int col_min = __INT32_MAX__;
  int row_min = __INT32_MAX__;
  int rate_max = -255;
  int rate_max_i = 0;
  int rate_max_j = 0;

  /* Копируем размер матрицы*/
  int n = size;
  /* Нижняя граница */
  int low_border = lb;
  int lb_left = 0;
  int lb_right = 0;

  /* Копия исходной матрицы */
  int* matrix_c = NULL;
  /* Матрица оценок */
  int* matrix_temp = NULL;
  /* Матрицы ветвлений */
  int* matrix_left = NULL;
  int* matrix_right = NULL;

  matrix_c = (int*)malloc(n * n * sizeof(int));
  matrix_left = (int*)malloc(n * n * sizeof(int));
  matrix_right = (int*)malloc(n * n * sizeof(int));
  matrix_temp = (int*)malloc(n * n * sizeof(int));
  /* Не выделилась память */
  if (!matrix_c || !matrix_temp || !matrix_left || !matrix_right) return NULL;

  /* Копируем матрицу */
  for (i = 0; i < n; ++i) {
    memcpy(matrix_c, matrix, n * n * sizeof(int));
  }

  if (level == 0) {
    low_border = reduction_matrix(matrix_c, n);
    if (low_border < 0) {
      free(matrix_right);
      free(matrix_left);
      free(matrix_temp);
      free(matrix_c);
      return NULL;
    }
  }

  /* Поиск преждевременных замыканий марщрута */
  for (i = 0; i < n; ++i) {
    for (j = 0; j < n; ++j) {
      if (matrix_c[IND(i, j, n)] == 0) {
        way[IND(step, 0, 2)] = i;
        way[IND(step, 1, 2)] = j;
        if (loop_check(way, step + 1, n)) {
          matrix_temp[IND(i, j, n)] = -255;
        }
        way[IND(step, 0, 2)] = -1;
        way[IND(step, 1, 2)] = -1;
      }
    }
  }

  /* Строим матрицу оценок */
  for (i = 0; i < n; ++i) {
    for (j = 0; j < n; ++j) {
      if (matrix_c[IND(i, j, n)] == 0) {
        col_min = __INT32_MAX__;
        row_min = __INT32_MAX__;
        for (k = 0; k < n; ++k) {
          if ((j != k) && (matrix_c[IND(i, k, n)] >= 0) &&
              (matrix_c[IND(i, k, n)] < row_min)) {
            row_min = matrix_c[IND(i, k, n)];
          }

          if ((i != k) && (matrix_c[IND(k, j, n)] >= 0) &&
              (matrix_c[IND(k, j, n)] < col_min)) {
            col_min = matrix_c[IND(k, j, n)];
          }
        }
        if (col_min == __INT32_MAX__) col_min = 0;
        if (row_min == __INT32_MAX__) row_min = 0;
        if (matrix_temp[IND(i, j, n)] >= 0) {
          matrix_temp[IND(i, j, n)] = row_min + col_min;
        }
        if (matrix_temp[IND(i, j, n)] > rate_max) {
          rate_max = matrix_temp[IND(i, j, n)];
          rate_max_i = i;
          rate_max_j = j;
        }
      } else {
        matrix_temp[IND(i, j, n)] = 0;
      }
    }
  }

  if (rate_max_i == rate_max_j || rate_max > lb) {
    free(matrix_right);
    free(matrix_left);
    free(matrix_temp);
    free(matrix_c);
    return NULL;
  }

  if (step + 1 >= n) {
    way[IND(step, 0, 2)] = rate_max_i;
    way[IND(step, 1, 2)] = rate_max_j;
    free(matrix_right);
    free(matrix_left);
    free(matrix_temp);
    free(matrix_c);
    return matrix;
  }

  /* Формирование матриц ветвлений */
  /* Левая - включает путь */
  for (i = 0; i < n; ++i) {
    for (j = 0; j < n; ++j) {
      if (i == rate_max_i || j == rate_max_j) {
        matrix_left[IND(i, j, n)] = -255;
      } else {
        matrix_left[IND(i, j, n)] = matrix_c[IND(i, j, n)];
      }
    }
  }

  matrix_left[IND(rate_max_j, rate_max_i, n)] = -255;

  /* Правая - исключает путь */
  memcpy(matrix_right, matrix_c, n * n * sizeof(int));
  matrix_right[IND(rate_max_i, rate_max_j, n)] = -255;

  lb_left = reduction_matrix(matrix_left, n);
  lb_right = reduction_matrix(matrix_right, n);

  if (lb_left <= lb_right) {
    way[IND(step, 0, 2)] = rate_max_i;
    way[IND(step, 1, 2)] = rate_max_j;
    if (!recursive_branch_and_bound(matrix_left, n, low_border + lb_left,
                                         level + 1, step + 1, way)) {
      way[IND(step, 0, 2)] = -1;
      way[IND(step, 1, 2)] = -1;
      if (!recursive_branch_and_bound(matrix_right, n, low_border + lb_right,
                                      level + 1, step, way)) {
        fprintf(logger, "Fail\n");
        return NULL;
      }
    }
  } else {
    if (!recursive_branch_and_bound(matrix_right, n, low_border + lb_right,
                                    level + 1, step, way)) {
      way[IND(step, 0, 2)] = rate_max_i;
      way[IND(step, 1, 2)] = rate_max_j;
      if (!recursive_branch_and_bound(matrix_left, n, low_border + lb_left,
                                           level + 1, step + 1, way)) {
        fprintf(logger, "Fail\n");
        return NULL;
      }
    }
  }
  free(matrix_right);
  free(matrix_left);
  free(matrix_temp);
  free(matrix_c);
  return matrix;
}

/* Метод ветвей и границ */
int* branch_and_bound(int* matrix, int size) {
  int i = 0;
  int j = 0;

  /* Копируем размер матрицы, потому что он будет изменяться */
  int n = size;
  /* Верхняя граница */
  int up_border = 0;

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

  /* Выделение памяти под массив хранения пути */
  start_way = (int*)malloc((n + 1) * sizeof(int));
  res_way = (int*)calloc((n + 1), sizeof(int));
  way = (int*)calloc(2 * n, sizeof(int));

  /* Не выделилась память */
  if (!start_way || !res_way || !way) return NULL;

  for (i = 0; i <= n; ++i) {
    start_way[i] = i % n;
  }
  /* Считаем верхнюю границу */
  up_border = 0;
  up_border = get_way_len(matrix, start_way, size);
  if (!recursive_branch_and_bound(matrix, n, up_border, 0, 0, way)) {
    return NULL;
  }

  for (i = 1; i < n; ++i) {
    for (j = 0; j < n; ++j) {
      if (i == 1) {
        if (start_way[0] == way[IND(j, 0, 2)]) {
          res_way[0] = way[IND(j, 0, 2)];
          res_way[n] = way[IND(j, 0, 2)];
          res_way[1] = way[IND(j, 1, 2)];
        }
      } else {
        if (res_way[i - 1] == way[IND(j, 0, 2)]) {
          res_way[i] = way[IND(j, 1, 2)];
        }
      }
    }
  }
  return res_way;
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
  int* matrix = NULL;
  int* brute_force_way = NULL;
  int* branch_and_bound_way = NULL;

/* Инициализация логгера */
#if LOG_FILE == 1
  if (!(logger = fopen(log_filename, "w"))) {
    logger = stderr;
  }
#else
  logger = stderr;
#endif

  /* Выделение памяти под матрицу */
  matrix = (int*)malloc(n * n * sizeof(int));
  /* Не выделилась память */
  if (!matrix) return 255;

  /* Чтение из файла */
  /* Ошибка открытия файла */
  if (!(fd = fopen(filename, "r"))) {
    free(matrix);
    return 253;
  }

  /* Читаем матрицу */
  for (i = 0; i < n; ++i) {
    for (j = 0; j < n; ++j) {
      fscanf(fd, "%d", &matrix[IND(i, j, n)]);
    }
  }

  /* Печать матрицы */
  for (i = 0; i < n; ++i) {
    for (j = 0; j < n; ++j) fprintf(stdout, "%d\t", matrix[IND(i, j, n)]);
    fprintf(stdout, "\n");
  }
  time = wtime();
  brute_force_way = brute_force(matrix, n, &way_index);
  time = wtime() - time;
  if (!brute_force_way) {
    free(brute_force_way);
    return 252;
  }

  fprintf(stdout, "Res brute force Min Way %ld [ ", way_index);
  for (i = 0; i <= n; ++i) {
    fprintf(stdout, "%d ", brute_force_way[i]);
  }
  fprintf(stdout, "]\n");
  fprintf(stdout, "Res brute force Way Len : %ld\n",
          get_way_len(matrix, brute_force_way, n));
  fprintf(stdout, "Brute force time : %lf\n", time);
  free(brute_force_way);

  time = wtime();
  branch_and_bound_way = branch_and_bound(matrix, n);
  time = wtime() - time;
  if (!branch_and_bound_way) {
    free(branch_and_bound_way);
    return 251;
  }
  fprintf(stdout, "Res branch and bound Min Way [ ");
  for (i = 0; i <= n; ++i) {
    fprintf(stdout, "%d ", branch_and_bound_way[i]);
  }
  fprintf(stdout, "]\n");
  fprintf(stdout, "Res branch and bound Way Len : %ld\n",
          get_way_len(matrix, branch_and_bound_way, n));
  fprintf(stdout, "Branch and bound time : %lf\n", time);

  free(branch_and_bound_way);
  free(matrix);
  return 0;
}