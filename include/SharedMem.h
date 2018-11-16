#ifndef SHAREDMEM_H
#define SHAREDMEM_H

/*Get size of part type*/
size_t SizeOfPartType(int ptype);

/*Get size of queue*/
size_t MEMSIZE(int num_parts,int ptype);

/*Get shared memory segment*/
int QueueInit(key_t shkey,int num_parts,int ptype);

/*Attach shared memory segment*/
char* QueueAttach(int shmid);

void QueueDetach(char* shm);

void QueueDelete(int shmid);
#endif
