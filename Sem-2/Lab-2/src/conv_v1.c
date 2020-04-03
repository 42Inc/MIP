#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
  int i = 0;

  /* Границы времени выполнения этапа конвейера */
  int min_time = 4;
  int max_time = 5;

  /* Количество этапов конвейра */
  int pipeline_length = argv[1] ? atoi(argv[1]) : 5;
  if (pipeline_length <= 0) return 253;

  /* Количество элементов для обработки */
  long int items_count = 5;
  /* Оставшиеся элементы */
  long int items_left = items_count;
  /* Обработанные элементы */
  long int items_complete = 0;

  /* Время самого медленного этапа конвейера */
  long int slowly_pipeline_step_time = 0;
  /* Время полного прохождения синхронного/асинхронного конвейера */
  long int sync_pipeline_time = 0;
  long int async_pipeline_time = 0;
  /* Разница времен. Синхронный - Асинхронный */
  long int diff_pipeline_time = 0;

  /* Время всех этапов конвейера */
  long int *pipeline_times = NULL;
  pipeline_times = (long int *)malloc(pipeline_length * sizeof(long int));
  typedef struct pipeline_state {
    /* Оставшееся время выполнения этапа асинхронного конвейера */
    long int ttl;
    /* Статус этапа асинхронного конвейера
     * 0 - свободен
     * 1 - обрабатывает элемент
     */
    int state;
    /* Общее время работы этапа над элементами */
    int work_time;
  } pipeline_state_t;

  pipeline_state_t *pipeline_status = NULL;

  if (!pipeline_times) return 255;
  srand(time(0));
  printf("Times:\n");
  for (i = 0; i < pipeline_length; ++i) {
    pipeline_times[i] = rand() % (max_time - min_time) + min_time;
    slowly_pipeline_step_time = pipeline_times[i] > slowly_pipeline_step_time
                                    ? pipeline_times[i]
                                    : slowly_pipeline_step_time;
    printf("Step %02d: %ld\n", i, pipeline_times[i]);
  }
  /*
   * a b c d e
   *   a b c d e
   *     a b c d e
   *       a b c d e
   *         a b c d e
   *           a b c d e
   */
  /* Расчет времени выполнения синхронного конвейера */
  sync_pipeline_time =
      slowly_pipeline_step_time * (pipeline_length + items_count - 1);
  printf("Sync pipeline time: %ld\n", sync_pipeline_time);

  /* Расчет времени выполнения асинхронного конвейера */
  pipeline_status =
      (pipeline_state_t *)malloc(pipeline_length * sizeof(pipeline_state_t));
  if (!pipeline_status) return 254;
  for (i = 0; i < pipeline_length; ++i) {
    pipeline_status[i].ttl = -1;
    pipeline_status[i].state = 0;
    pipeline_status[i].work_time = 0;
  }

  /* Цикл пока есть элементы */
  while (items_complete ^ items_count) {
    if (pipeline_length == 1) {
      /* Одноэтапный конвейер */
      if (pipeline_status[0].ttl <= 0 && !pipeline_status[0].state &&
          items_left) {
        --items_left;
        pipeline_status[0].ttl = pipeline_times[0];
        pipeline_status[0].state = 1;
      }
      if (pipeline_status[0].ttl <= 0 && pipeline_status[0].state) {
        ++items_complete;
        pipeline_status[0].state = 0;
        pipeline_status[0].work_time +=
            pipeline_times[0] - pipeline_status[0].ttl;
      }
    } else {
      /* Многоэтапный конвейер */
      for (i = pipeline_length - 1; i >= 0; --i) {
        if (i == 0) {
          /*
           * Если первый этап закончил обработку и свободен - берет на обработку
           * следующий элемент
           */
          if (pipeline_status[i].ttl <= 0 && !pipeline_status[i].state &&
              items_left) {
            --items_left;
            pipeline_status[i].ttl = pipeline_times[i];
            pipeline_status[i].state = 1;
          }
        } else if (i == pipeline_length - 1) {
          /*
           * Если последний этап закончил обработку и было что обратывать -
           * Увеличивает кол-во обработанных элементов
           */
          if (pipeline_status[i].ttl <= 0 && pipeline_status[i].state) {
            ++items_complete;
            pipeline_status[i].state = 0;
            pipeline_status[i].work_time +=
                pipeline_times[i] - pipeline_status[i].ttl;
          }
          /*
           * Если последний этап свободен - проверяет предыдущий, если
           * предыдущий этап закончил обработку и было что обратывать - забирает
           * элемент себе, меняя состояния
           */
          if (!pipeline_status[i].state) {
            if (pipeline_status[i - 1].ttl <= 0 &&
                pipeline_status[i - 1].state) {
              pipeline_status[i - 1].state = 0;
              pipeline_status[i].state = 1;
              pipeline_status[i].ttl = pipeline_times[i];
              pipeline_status[i - 1].work_time +=
                  pipeline_times[i - 1] - pipeline_status[i - 1].ttl;
            }
          }
        } else {
          /*
           * Если i-ый этап свободен - проверяет предыдущий, если предыдущий
           * этап закончил обработку и было что обратывать - забирает элемент
           * себе, меняя состояния
           */
          if (!pipeline_status[i].state) {
            if (pipeline_status[i - 1].ttl <= 0 &&
                pipeline_status[i - 1].state) {
              pipeline_status[i - 1].state = 0;
              pipeline_status[i].state = 1;
              pipeline_status[i].ttl = pipeline_times[i];
              pipeline_status[i - 1].work_time +=
                  pipeline_times[i - 1] - pipeline_status[i - 1].ttl;
              ;
            }
          }
        }
      }
    }
    /* Такт времени - декремент оставшегося времени работы этапов */
    for (i = pipeline_length - 1; i >= 0; --i) {
      --pipeline_status[i].ttl;
    }
  }
  /* Подсчет общего времени работы всех этапов конвейера */
  for (i = 0; i < pipeline_length; ++i) {
    async_pipeline_time += pipeline_status[i].work_time;
  }
  printf("Async pipeline time: %ld\n", async_pipeline_time);
  diff_pipeline_time = sync_pipeline_time - async_pipeline_time;
  printf("Time diff: %ld\n", diff_pipeline_time);
  return 0;
}