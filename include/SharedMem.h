#ifndef SHAREDMEM_H
#define SHAREDMEM_H

#include "Component.h"

typedef struct SHMemQueue{
  int back; //newest queue element
  int next; //next elem to be retrieved for painting
  int checked;  //next elem to be retrived for checking
  int painted;  //next elem to be retrieved for assemblage
  char* elements; //ptr to where queue elements start (first element)
} SHMemQueue;

/*Get size of queue*/
size_t MEMSIZE(int num_parts);

/*Get shared memory segment*/
int QueueInit(key_t shkey,int num_parts);
/*Attach shared memory segment*/
char* QueueAttach(int shmid);
void QueueDetach(char* shm);
void QueueDelete(int shmid);


/*Insert Component in shared memory*/
void QueueInsert(char* mem,Component* comp);

/*return pointer to back element*/
char* GetBackOffset(char* mem);

#endif
