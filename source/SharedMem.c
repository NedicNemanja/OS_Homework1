#include "ErrorCodes.h"
#include "SharedMem.h"
#include "Component.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>  //perror
//shared mem
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

size_t MEMSIZE(int num_parts){
  return (size_t)(num_parts*sizeof(Component));
}

/*Get shared memory segment*/
int QueueInit(key_t shkey,int num_parts){
  int shmid;
  if((shmid=shmget(shkey,MEMSIZE(num_parts),IPC_CREAT | 0600)) < 0){
    perror("shmget: ");
    exit(SHMGET);
  }
  return shmid;
}

/*Attach shared memory segment*/
char* QueueAttach(int shmid){
  char* shmemory=NULL;
  if((shmemory=shmat(shmid,NULL,0)) == (char*)-1){
    perror("shmat: ");
    exit(SHMAT);
  }
  return shmemory;
}

void QueueDetach(char* shm){
  shmdt(shm);
}

void QueueDelete(int shmid){
  shmctl(shmid,IPC_RMID,0);
}
