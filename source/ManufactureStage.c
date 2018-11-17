#include "ManufactureStage.h"
#include "Component.h"
#include "SharedMem.h"

#include <sys/ipc.h>
#include <sys/sem.h>

#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

int ManufactureStage(key_t queueid,int ptype,int num_parts){
  printf("ManufactureStage%d Process started\n", ptype);

  char* mem = QueueAttach(queueid);

  /*init rand num generator*/
  time_t t;
  srand((unsigned) time(&t));

  /*for every part: create-sleep*/
  for(int i=0; i<num_parts; i++){
    //create component
    struct timeval t;
    gettimeofday(&t,NULL);
    Component comp = {GenerateComponentID(ptype), t, ptype};
    //insert in queue
    QueueInsert(mem,&comp);
    //sleep [0,MAX_MANUFACTURE_TIME)
    sleep(SleepTime(rand()));
  }

  /*Cleanup*/
  QueueDetach(mem);
  return 0;
}
