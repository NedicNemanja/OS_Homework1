#include "PaintshopStage.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>

int PaintshopStage(int* queueids, int num_parts){
  printf("Paintshop Process started %d\n", num_parts);
  SetPaintTimes();
  /*get queues from shared memory and their semaphores*/
  char* mems[3];
  SHMemQueue* queues[3];
  int semids[3];
  for(int i=0; i<3; i++){
    mems[i] = QueueAttach(queueids[i]);
    queues[i] = (SHMemQueue*)mems[i];
    semids[i] = queues[i]->semid;
  }

  /*paint all components*/
  for(int i=0; i<3*num_parts; i++){
    //pick which Component Type (1,2,3) to paint
    int type = PickComponentType(queues);
    //printf("(%d) painting type%d\n", i,type);
    QueuePaint(queues[type-1],type);
  }

  /*Cleanup*/
  for(int i=0; i<3; i++)
    QueueDetach(mems[i]);
  return 0;
}

/*Find the component type with the min number of components available for the
checking stage (avoid checking being idle).
But ignore queues whose paint_semaphores are down, you don't have any components
to pull from them anyway.
Return values 1,2,3 for component type 1,2 and 3. Default is 1,
if all paint_sem are down.*/
int PickComponentType(SHMemQueue** queues){
  int min = semctl(queues[0]->semid,check_sem,GETVAL);
  int type = 1;
  for(int i=1; i<=2; i++){
    int val = semctl(queues[i]->semid,check_sem,GETVAL);
    //printf("type%d val=%d sem=%d\n", i, val, semctl(queues[i]->semid,paint_sem,GETVAL));
    if((val < min) && (semctl(queues[i]->semid,paint_sem,GETVAL) > 0)){
      min = val;
      type = i+1;
    }
  }
  return type;
}

void SetPaintTimes(){
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<3; i++){
    paint_time[i] = (rand()%MAX_PAINT_TIME)/1000.0;
  }
}
