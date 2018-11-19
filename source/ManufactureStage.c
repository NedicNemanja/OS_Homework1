#include "ErrorCodes.h"
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

int ManufactureStage(int queueid,int ptype,int num_parts){
  printf("ManufactureStage%d Process started\n", ptype);
  //get queue from shared memory
  char* mem = QueueAttach(queueid);
  SHMemQueue* queue = (SHMemQueue*)mem;
  //init rand num generator for random sleep time
  time_t t;
  srand((unsigned) time(&t));

  /*for every part: create-sleep*/
  for(int i=0; i<num_parts; i++){
    //create component
    sleep(SleepTime(rand()));
    struct timeval creation_time;
    gettimeofday(&creation_time,NULL);
    Component comp = {GenerateComponentID(ptype), creation_time, ptype};
    //insert in queue
    QueueInsert(queue,&comp);
  }

  /*Cleanup*/
  QueueDetach(mem);
  return 0;
}
