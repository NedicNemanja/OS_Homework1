#include "ErrorCodes.h"
#include "ManufactureStage.h"
#include "PaintshopStage.h"
#include "CheckStage.h"
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
  key_t memkeys[3] = {1111,1112,0x1113};
  key_t semkeys[3] = {2221,2222,0x2223};

  /*Get SHMemQueues in shared memory(shmget) and init their semaphores(semget)*/
  int queueids[3] = { QueueInit(memkeys[0],num_parts,semkeys[0]),
                      QueueInit(memkeys[1],num_parts,semkeys[1]),
                      QueueInit(memkeys[2],num_parts,semkeys[2])};

  /*Create ManufactureStage processes*/
  for(int i=0; i<3; i++){
    if(fork() == 0){
      return ManufactureStage(queueids[i],i+1,num_parts,semkeys[i]);
    }
  }
  /*Create Paintshop process*/
  if(fork() == 0){
    return PaintshopStage(atoi(argv[1]));
  }
  /*Create CheckStage processes*/
  for(int i=0; i<3; i++){
    if(fork() == 0){
      return CheckStage(atoi(argv[1]));
    }
  }

  /*Run assemblage process*/


  //wait for all children before you terminate
  int status;
  while (wait(&status) > 0);
  printf("Parent done\n");

  /*Cleanup*/
  for(int i=0; i<3; i++){
    //delete shared memory and its semaphores
    QueueDelete(queueids[i],semkeys[i]);
  }
  return 0;
}

int GetArgs(int argc,char* argv[]){
  if(argc != 2)
    exit(BAD_ARGUMENTS);
  return atoi(argv[1]);
}
