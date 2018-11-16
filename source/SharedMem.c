#include "ErrorCodes.h"
#include "SharedMem.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>  //perror
//shared mem
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

size_t SizeOfPartType(int ptype){
  switch(ptype){
    case 1: return sizeof(struct Part1);
    case 2: return sizeof(struct Part2);
    case 3: return sizeof(struct Part3);
  }
}

size_t MEMSIZE(int num_parts,int ptype){
  return (size_t)(num_parts*SizeOfPartType(ptype);
}

/*Get shared memory segment*/
int QueueInit(key_t shkey,int num_parts,int ptype){
  int shmid;
  if((shmid=shmget(shkey,MEMSIZE(num_parts,ptype),IPC_CREAT | 0600)) < 0){
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
