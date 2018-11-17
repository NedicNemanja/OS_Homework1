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
  const key_t memkey=0x1111, semkey=0x2222;

  /*Get part queues in shared memory (shmget)*/
  int queueid1 = QueueInit(memkey,num_parts); //queue for type1 parts
  int queueid2 = QueueInit(memkey,num_parts); //..type2...
  int queueid3 = QueueInit(memkey,num_parts); //...type3...

  /*Setup semaphores
  sem%4=0 down when there are no parts available
  sem%4=1 down when there are no parts to be painted
  sem%4=2 down when there are no parts to be checked
  sem%4=3 down when there are no parts to be assembled
  */
  int semid;
  if((semid=semget(semkey,12,IPC_CREAT | 0600)) < 0){
    perror("semget: ");
    exit(SEMGET);
  }

  /*Create ManufactureStage processes*/
  for(int type=1; type<=3; type++){
    if(fork() == 0){
      return ManufactureStage(memkey,type,num_parts);
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

  //wait for all children before you terminate
  printf("waiting...\n");
  int status;
  while (wait(&status) > 0);
  printf("Parent done\n");

  /*Cleanup*/
  //semaphores
  if(semctl(semid, 0, IPC_RMID) < 0){
    perror("Could not delete semaphores.\n");
    exit(SEM_DEL);
  }
  //shared memory
  QueueDelete(queueid1);
  QueueDelete(queueid2);
  QueueDelete(queueid3);
  return 0;
}

int GetArgs(int argc,char* argv[]){
  if(argc != 2)
    exit(BAD_ARGUMENTS);
  return atoi(argv[1]);
}
