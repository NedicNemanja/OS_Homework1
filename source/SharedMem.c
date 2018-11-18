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
#include <sys/sem.h>  //semaphores

size_t MEMSIZE(int num_parts){
  return (size_t)(num_parts*sizeof(Component));
}

/*Get shared memory segment*/
int QueueInit(key_t shkey,int num_parts,key_t semkey){
  //get memory segment
  int shmid;
  if((shmid=shmget(shkey,sizeof(SHMemQueue)+MEMSIZE(num_parts),
  IPC_CREAT | 0600)) < 0){
    perror("shmget: ");
    exit(SHMGET);
  }
  //initialize queue data
  char* mem = QueueAttach(shmid);
  struct SHMemQueue SHMqueue = {-1,-1,-1,-1,mem+sizeof(SHMemQueue),0};
  //..along with semaphores
  if((SHMqueue.semid=semget(semkey,3,IPC_CREAT | 0600)) < 0){
    perror("semget: ");
    exit(SEMGET);
  }
  union semun arg;
  arg.val = 0;
  for(int sem_num=0; sem_num<3; sem_num++){
    if(semctl(SHMqueue.semid,sem_num,SETVAL,arg) == -1){
      perror("semctl: ");
      exit(SEMCTL);
    }
  }
  //write queue data to shared memory
  memcpy(mem,&SHMqueue,sizeof(SHMemQueue));
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

void QueueDelete(int shmid,key_t semkey){
  //delete shared memory
  shmctl(shmid,IPC_RMID,0);
  //delete semaphores
  int semid;  //get semid to delete
  if((semid=semget(semkey,3,IPC_CREAT | 0600)) < 0){
    perror("semget: ");
    exit(SEMGET);
  }
  printf("value of sem0 %d\n", semctl(semid,0,GETVAL));
  if(semctl(semid,0,IPC_RMID)==-1){ //delete
    perror("Failed to delete semaphore.");
    exit(SEMCTL);
  }
}

/*Insert Component in shared memory*/
void QueueInsert(char* mem,Component* comp){
  char* dest = GetBackOffset(mem)+sizeof(Component);
  memcpy(dest,comp,sizeof(Component));
  //increase semaphore
  struct sembuf sops;
  sops.sem_num = 0; //sem0 controls num of parts to be painted
  sops.sem_op = 1;  //up it by +1
  sops.sem_flg = 0;
  if(semop(GetSemId(mem),&sops,1) != 0){
    perror("semaphore op +1 failed.");
    exit(SEMOP);
  }
}



/*return pointer to back element*/
char* GetBackOffset(char* mem){
  SHMemQueue* SHMqueue = (SHMemQueue*)mem;
  return SHMqueue->elements+(SHMqueue->back*sizeof(Component));
}

/*return semaphore id of queue at mem*/
int GetSemId(char* mem){
  SHMemQueue* SHMqueue = (SHMemQueue*)mem;
  return SHMqueue->semid;
}
