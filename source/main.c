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
  const key_t memkey=0x1111;
  key_t semkeys[3] = {0x2222,0x3333,0x4444};

  /*Get part queues in shared memory (shmget)*/
  int queueids[3] = { QueueInit(memkey,num_parts),
                      QueueInit(memkey,num_parts),
                      QueueInit(memkey,num_parts)};

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

  /*Setup semaphores for each queue (3x4)
  sem0 down when there are no parts available
  sem1 down when there are no parts to be painted
  sem2 down when there are no parts to be checked
  sem3 down when there are no parts to be assembled
  */
  int semids[3];
  for(int i=0; i<3; i++){
    if((semids[i]=semget(semkeys[i],4,IPC_CREAT | 0600)) < 0){
      perror("semget: ");
      exit(SEMGET);
    }
  }

  //wait for all children before you terminate
  int status;
  while (wait(&status) > 0);
  printf("Parent done\n");

  /*Cleanup*/
  for(int i=0; i<3; i++){
    //semaphores
    if(semctl(semids[i], 0, IPC_RMID) < 0){
      perror("Could not delete semaphores.\n");
      exit(SEM_DEL);
    }
    //shared memory
    QueueDelete(queueids[i]);
  }
  return 0;
}

int GetArgs(int argc,char* argv[]){
  if(argc != 2)
    exit(BAD_ARGUMENTS);
  return atoi(argv[1]);
}
