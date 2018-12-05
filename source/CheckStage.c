#include "SharedMem.h"
#include "CheckStage.h"

#include <stdio.h>

int CheckStage(int queueid,int ptype,int num_parts){
  printf("CheckStage%d Process started\n", ptype);
  //get queue from shared memory
  char* mem = QueueAttach(queueid);
  SHMemQueue* queue = (SHMemQueue*)mem;
  //get check & assemble sempahores
  int check_semid = SemGet(queue->check_semkey);
  int assemble_semid = SemGet(queue->assemble_semkey);

  /*check components*/
  for(int i=0; i<num_parts; i++){
    //try and get one from paint
    SemDown(check_semid);
    //got it, now check it
    QueueCheck(queue,ptype);
    //one more ready for assemblage
    SemUp(assemble_semid);
  }
  //Cleanup
  QueueDetach(mem);
  printf("Closing Check%d.\n", ptype);
  return 0;
}
