#include "SharedMem.h"
#include "CheckStage.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>

int CheckStage(int queueid,int ptype,int num_parts){
  printf("CheckStage Process started %d\n", num_parts);
  SetCheckTimes();
  //get queue from shared memory
  char* mem = QueueAttach(queueid);
  SHMemQueue* queue = (SHMemQueue*)mem;

  /*check components*/
  for(int i=0; i<num_parts; i++){
    QueueCheck(queue,ptype);
  }
  return 0;
}

void SetCheckTimes(){
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<3; i++){
    check_time[i] = (rand()%MAX_CHECK_TIME)/1000.0;
  }
}
