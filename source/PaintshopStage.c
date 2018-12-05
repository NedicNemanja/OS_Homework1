#include "PaintshopStage.h"
#include "ErrorCodes.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>

int PaintshopStage(int* queueids, int num_parts){
  printf("Paintshop Process started\n");
  double paint_wait_time=0;
  /*get queues from shared memory and their semaphores*/
  char* mems[3];
  SHMemQueue* queues[3];
  //paint & check sempahores
  int paint_semids[3],check_semids[3];
  for(int i=0; i<3; i++){
    mems[i] = QueueAttach(queueids[i]);
    queues[i] = (SHMemQueue*)mems[i];
    paint_semids[i] = SemGet(queues[i]->paint_semkey);
    check_semids[i] = SemGet(queues[i]->check_semkey);
  }

  /*paint all components*/
  for(int i=0; i<3*num_parts; i++){
    //pick which Component Type (1,2,3) to paint
    int type = PickComponentType(queues,check_semids,paint_semids,num_parts);
    //try to get that component from queue
    SemDown(paint_semids[type-1]);
    //got it,paint it and return the component
    Component* comp = QueuePaint(queues[type-1],type);
    //calc its wait time since it was created
    paint_wait_time += CalcWaitTime(comp);
    //one more ready to be checnked
    SemUp(check_semids[type-1]);
  }

  /*Cleanup*/
  for(int i=0; i<3; i++){
    QueueDetach(mems[i]);
  }
  printf("Closing Paintshop. Average (paint) Wait Time: %f\n", paint_wait_time/(3*num_parts));
  return 0;
}

/*Find the component type with the min number of components available for the
checking stage (avoid checking being idle).
But ignore queues whose paint_semaphores are down, you don't have any components
to pull from them anyway.
Return values 1,2,3 for component type 1,2 and 3. Default is 1,
if all paint_sem are down.*/
int PickComponentType(SHMemQueue** queues, int* check_semids,int* paint_semids,
  int num_parts){
  int min,type=-1;
  //find first type that is available to paint and set it as min
  int i;
  for(i=0; i<3; i++){
    if(semctl(paint_semids[0],0,GETVAL) > 0){
      min = semctl(check_semids[i],0,GETVAL);
      type = i+1;
      break;
    }
  }
  //continue searching for min of other types
  for(int j=i+1; j<3; j++){
    int val = semctl(check_semids[j],0,GETVAL);
    if((val <= min) && (semctl(paint_semids[j],0,GETVAL) > 0)){
      min = val;
      type = j+1;
    }
  }
  //if nothing was available, then pick a type whose parts have not all been painted yet
  if(type == -1){
    for(int t=0; t<3; t++){
      if(queues[t]->next < num_parts){
        type = t+1;
        break;
      }
    }
  }
  if(type == -1) exit(PICK_TYPE);
  return type;
}
