#include <stdio.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <unistd.h>

int ManufactureStage(int num_parts,char* shm){
  printf("ManufactureStage Process started %d\n", num_parts);
  printf("%s", shm);
  sprintf(shm,"manufacture\n");
  key_t shkey =1234;
  return 0;
}
