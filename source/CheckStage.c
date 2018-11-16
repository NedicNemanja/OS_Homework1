#include <stdio.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <unistd.h>

int CheckStage(int num_parts){
  printf("CheckStage Process started %d\n", num_parts);

  key_t shkey =1234;
  return 0;
}
