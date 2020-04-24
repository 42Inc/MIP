#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * Варианты:
 * Метод ветвей и границ
 * Метод ближайших соседей
 *
 * Допущения:
 * Граф считать полносвязным
 */

#define SWAP(x, y, tmp) \
  {                     \
    tmp = x;            \
    x = y;              \
    y = tmp;            \
  }
  
static FILE* logger = NULL;
char* log_filename = "v2.log";

/* Метод полного перебора. Возвращает путь */
int* brute_force(int** matrix, int n) {
  int i = 0;
  int j = 0;
  long int way_index = 0;
  /* Путь */
  int* way = NULL;
  int* start_way = NULL;
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
  /* Не выделилась память */
  if (!way || !start_way) return NULL;

  for (i = 0; i <= n; ++i) {
    start_way[i] = i % n;
    fprintf(logger, "%d ", start_way[i]);
  }
  /* Сохраняем стартовый путь. Просто чтоб был */
  memcpy(way, start_way, (n + 1) * sizeof(int));

  if (logger) {
    fprintf(logger, "Start way:\n");
    for (i = 0; i <= n; ++i) {
      start_way[i] = i % n;
      fprintf(logger, "%d ", start_way[i]);
    }
    fprintf(logger, "\n");
  }

  if (n == 2) return way;

  /*
   * TODO: Перебор всех перестановок для индексов [1:n - 1]
   * по индексу перестановки
   * Количество перестановок = (n-1)!
   */

  return NULL;
}

/* Метод ветвей и границ */
int* branch_and_bound() { return NULL; }

int main(int argc, char** argv) {
  int i = 0;
  int j = 0;

  char* filename = "matrix.dat";
  FILE* fd = NULL;

  /* Инициализация логгера */
  if (!(logger = fopen(log_filename, "w"))) {
    logger = NULL;
  }

  /* Размер матрицы - по умолчанию 5 */
  int n = argv[1] ? atoi(argv[1]) : 5;
  /* Если передан файл */
  if (argv[2]) filename = argv[2];

  /* Матрица */
  int** matrix = NULL;
  int* brute_way = NULL;

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
    return 252;
  }

  /* Читаем матрицу */
  for (i = 0; i < n; ++i) {
    for (j = 0; j < n; ++j) {
      fscanf(fd, "%d", &matrix[i][j]);
    }
  }

  /* Печать матрицы */
  for (i = 0; i < n; ++i) {
    for (j = 0; j < n; ++j) fprintf(stderr, "%d ", matrix[i][j]);
    fprintf(stderr, "\n");
  }

  brute_way = brute_force(matrix, n);
  return 0;
}