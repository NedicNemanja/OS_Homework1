#include "ErrorCodes.h"
#include "ManufactureStage.h"
#include "PaintshopStage.h"
#include "CheckStage.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
//shared memory
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <sys/sem.h>  //semaphores
#include <sys/wait.h> //wait for children
#include <unistd.h> //fork

int main(int argc,char* argv[]){

  /*Setup shared memory*/
  key_t shkey =1;
  int shmid;
  char* shm;
  if((shmid=shmget(shkey,24,IPC_CREAT | 0660)) < 0){
    perror("shmget: ");
    exit(SHMGET);
  }
  if((shm=shmat(shmid,NULL,0)) == (char*)-1){
    perror("shmat: ");
    exit(SHMAT);
  }

  sprintf(shm,"Hello world\n");
  /*Create ManufactureStage processes*/
  for(int i=0; i<3; i++){
    if(fork() == 0){
      return ManufactureStage(atoi(argv[1]),shm);
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

  //cleanup
  shmdt(shm);
  shmctl(shmid,IPC_RMID,0);
  return 0;
}
