#!/usr/bin/env python3

int main(int argc, char **argv) {
  int i = 0;

  long int tact = 1;

  long int last_tact = tact;

  int min_time = 1;
  int max_time = 50;

  int pipeline_length = argv[1] ? atoi(argv[1]) : 5;
  if (pipeline_length <= 0) return 253;

  long int items_count = 100000;

  long int items_left = items_count;

  long int items_complete = 0;


  long int last_items_complete = 0;


  long int slowly_pipeline_step_time = 0;

  long int sync_pipeline_time = 0;
  long int async_pipeline_time = 0;

  long int diff_pipeline_time = 0;

  long int *pipeline_times = NULL;
  pipeline_times = (long int *)malloc(pipeline_length * sizeof(long int));
  typedef struct pipeline_state {

    int state;
  } pipeline_state_t;

  pipeline_state_t *pipeline_status = NULL;

  if (!pipeline_times) return 255;

  for (i = 0; i < pipeline_length; ++i) {
    pipeline_times[i] = rand() % (max_time - min_time) + min_time;
    slowly_pipeline_step_time = pipeline_times[i] > slowly_pipeline_step_time
                                    ? pipeline_times[i]
                                    : slowly_pipeline_step_time;
  }

  sync_pipeline_time =
      slowly_pipeline_step_time * (pipeline_length + items_count - 1);
  fprintf(stdout, "Sync pipeline time: %ld\n", sync_pipeline_time);
  pipeline_status =
      (pipeline_state_t *)malloc(pipeline_length * sizeof(pipeline_state_t));
  if (!pipeline_status) return 254;
  for (i = 0; i < pipeline_length; ++i) {
    pipeline_status[i].ttl = -1;
    pipeline_status[i].state = 0;
  }

  while (1) {

    if (last_items_complete ^ items_complete) {

      async_pipeline_time += tact - last_tact;
      last_tact = tact;
      last_items_complete = items_complete;
    }

    if (!(items_complete ^ items_count)) break;

    if (pipeline_length == 1) {

      if (pipeline_status[0].ttl <= 0 && !pipeline_status[0].state &&
          items_left) {
        --items_left;

        pipeline_status[0].ttl = pipeline_times[0] - 1;
        pipeline_status[0].state = 1;
      }
      if (pipeline_status[0].ttl <= 0 && pipeline_status[0].state) {
        ++items_complete;
        pipeline_status[0].state = 0;
      }
    } else {

      for (i = pipeline_length - 1; i >= 0; --i) {
        if (i == 0) {

          if (pipeline_status[i].ttl <= 0 && !pipeline_status[i].state &&
              items_left) {
            --items_left;

            pipeline_status[i].ttl = pipeline_times[i] - 1;
            pipeline_status[i].state = 1;
          }
        } else if (i == pipeline_length - 1) {

          if (pipeline_status[i].ttl <= 0 && pipeline_status[i].state) {
            ++items_complete;
            pipeline_status[i].state = 0;
          }

          if (!pipeline_status[i].state) {
            if (pipeline_status[i - 1].ttl <= 0 &&
                pipeline_status[i - 1].state) {
              pipeline_status[i - 1].state = 0;
              pipeline_status[i].state = 1;
              pipeline_status[i].ttl = pipeline_times[i];
            }
          }
        } else {

          if (!pipeline_status[i].state) {
            if (pipeline_status[i - 1].ttl <= 0 &&
                pipeline_status[i - 1].state) {
              pipeline_status[i - 1].state = 0;
              pipeline_status[i].state = 1;
              pipeline_status[i].ttl = pipeline_times[i];
            }
          }
        }
      }
    }

    for (i = pipeline_length - 1; i >= 0; --i) {
      --pipeline_status[i].ttl;
    }
    ++tact;
  }

  fprintf(stdout, "Practical async pipeline time: %ld\n", async_pipeline_time);

  async_pipeline_time = 0;
  for (i = 0; i < pipeline_length; ++i) {
    async_pipeline_time += pipeline_times[i];
  }
  async_pipeline_time += slowly_pipeline_step_time * (items_count - 1);
  fprintf(stdout, "Theoretical async pipeline time: %ld\n", async_pipeline_time);
  diff_pipeline_time = sync_pipeline_time - async_pipeline_time;
  fprintf(stdout, "Time diff: %ld\n", diff_pipeline_time);
  fprintf(stderr, "%d\t%ld\t%ld\n", pipeline_length, sync_pipeline_time, async_pipeline_time);
  return 0;
}
