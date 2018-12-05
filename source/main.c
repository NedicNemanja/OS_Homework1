#include "ErrorCodes.h"
#include "ManufactureStage.h"
#include "PaintshopStage.h"
#include "CheckStage.h"
#include "AssemblageStage.h"
#include "SharedMem.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>  //perror
//shared memory
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <sys/sem.h>  //semaphores
#include <sys/wait.h> //wait for children
#include <unistd.h> //fork

int GetArgs(int argc,char* argv[]);

int main(int argc,char* argv[]){
  int num_parts = GetArgs(argc,argv);
  key_t memkeys[3] = {0x1111,0x2222,0x3333};
  SetPaintTimes();
  SetCheckTimes();
  SetAssemblageTime();

  /*Get SHMemQueues in shared memory(shmget)*/
  int queueids[3] = { QueueInit(memkeys[0],num_parts),
                      QueueInit(memkeys[1],num_parts),
                      QueueInit(memkeys[2],num_parts)};

  /*Create ManufactureStage processes*/
  for(int i=0; i<3; i++){
    if(fork() == 0){
      return ManufactureStage(queueids[i],i+1,num_parts);
    }
  }
  /*Create Paintshop process*/
  if(fork() == 0){
    return PaintshopStage(queueids,num_parts);
  }
  /*Create CheckStage processes*/
  for(int i=0; i<3; i++){
    if(fork() == 0){
      return CheckStage(queueids[i],i+1,num_parts);
    }
  }

  /*Run assemblage process*/
  AssemblageStage(queueids, num_parts);

  //wait for all children before you terminate
  int status;
  while (wait(&status) > 0);
  printf("Children done\n");

  PrintSetTimes();
  /*Cleanup*/
  for(int i=0; i<3; i++){
    //delete shared memory
    QueueDelete(queueids[i]);
  }
  return 0;
}

int GetArgs(int argc,char* argv[]){
  if(argc != 2)
    exit(BAD_ARGUMENTS);
  return atoi(argv[1]);
}
