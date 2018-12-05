#include "AssemblageStage.h"
#include "SharedMem.h"
#include "Component.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void AssemblageStage(int* queueids, int num_parts){
  printf("Assemblage Process started\n");
  double total_wait_time=0;

  /*get queues from shared memory and their semaphores*/
  char* mems[3];
  SHMemQueue* queues[3];
  //get check sempahores
  int assemble_semids[3];
  for(int i=0; i<3; i++){
    mems[i] = QueueAttach(queueids[i]);
    queues[i] = (SHMemQueue*)mems[i];
    assemble_semids[i] = SemGet(queues[i]->assemble_semkey);
  }

  /*paint all components*/
  for(int i=0; i<num_parts; i++){
    //get one of each type
    SemDown(assemble_semids[0]);
    SemDown(assemble_semids[1]);
    SemDown(assemble_semids[2]);
    //assemble
    Product prod = QueueAssemble(queues);
    total_wait_time += CalcProductWaitTime(prod);
    printf("\tAssembled Product %ld\n", prod.id);
    fflush(stdin);
  }

  /*Cleanup*/
  for(int i=0; i<3; i++){
    QueueDeleteSemaphores(queues[i]);
    QueueDetach(mems[i]);
  }
  printf("Closing Assemblage. Average (total) Wait Time: %f\n", total_wait_time/num_parts);
}
