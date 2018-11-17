#include "ErrorCodes.h"
#include "SharedMem.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>  //perror
//shared mem
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h> //memcpy

size_t MEMSIZE(int num_parts){
  return (size_t)(num_parts*sizeof(Component));
}

/*Get shared memory segment*/
int QueueInit(key_t shkey,int num_parts){
  int shmid;
  if((shmid=shmget(shkey,sizeof(SHMemQueue)+MEMSIZE(num_parts),
  IPC_CREAT | 0600)) < 0){
    perror("shmget: ");
    exit(SHMGET);
  }
  //initialize queue data
  char* mem = QueueAttach(shmid);
  struct SHMemQueue queue = {-1,-1,-1,-1,mem+sizeof(SHMemQueue)};
  memcpy(mem,&queue,sizeof(SHMemQueue));
  QueueDetach(mem);
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

/*Insert Component in shared memory*/
void QueueInsert(char* mem,Component* comp){
  char* dest = GetBackOffset(mem)+sizeof(Component);
  memcpy(dest,comp,sizeof(Component));
}

/*return pointer to back element*/
char* GetBackOffset(char* mem){
  SHMemQueue* SHMqueue = (SHMemQueue*)mem;
  return SHMqueue->elements+(SHMqueue->back*sizeof(Component));
}
