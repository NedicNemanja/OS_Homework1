#include "ErrorCodes.h"
#include "ManufactureStage.h"
#include "Component.h"
#include "SharedMem.h"

#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*Creates components of the specified ptype. Inserts them in Queue.
Incerases paint_sem.*/
int ManufactureStage(int queueid,int ptype,int num_parts){
  printf("ManufactureStage%d Process started\n", ptype);
  //get queue from shared memory
  char* mem = QueueAttach(queueid);
  SHMemQueue* queue = (SHMemQueue*)mem;
  //get semaphores
  int paint_semid = SemGet(queue->paint_semkey);
  //init rand num generator for random sleep time
  time_t t;
  srand((unsigned) time(&t));

  /*for every part: create-sleep*/
  for(int i=0; i<num_parts; i++){
    //create component
    usleep(SleepTime(rand())*1000000);
    struct timeval creation_time;
    gettimeofday(&creation_time,NULL);
    Component comp = {GenerateComponentID(ptype), creation_time, ptype};
    //insert component in queue (to be painted)
    QueueInsert(queue,&comp);
    //let paint know one more is ready to be painted
    SemUp(paint_semid);
  }

  /*Cleanup*/
  QueueDetach(mem);
  printf("Closing Manufacture%d.\n", ptype);
  return 0;
}
