#include "ManufactureStage.h"

#include <stdio.h>

#include <sys/ipc.h>
#include <sys/sem.h>

#include <unistd.h>
#include <time.h>

int ManufactureStage(key_t queue,int ptype,int num_parts){
  printf("ManufactureStage%d Process started\n", ptype);

  /*init rand num generator*/
  time_t t;
  srand((unsigned) time(&t));

  for(int i=0; i<num_parts; i++){
    //crate part
    //insert part queue
    double sleep_time = (rand()%MAX_MANUFACTURE_TIME)/1000.0;
    printf("man%d sleep for %f\n", ptype, sleep_time);
    sleep(sleep_time);
  }

  return 0;
}
