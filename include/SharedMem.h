#ifndef SHAREDMEM_H
#define SHAREDMEM_H

/*Get size of queue*/
size_t MEMSIZE(int num_parts);

/*Get shared memory segment*/
int QueueInit(key_t shkey,int num_parts);

/*Attach shared memory segment*/
char* QueueAttach(int shmid);

void QueueDetach(char* shm);

void QueueDelete(int shmid);
#endif
