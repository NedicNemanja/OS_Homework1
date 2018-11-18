#ifndef SHAREDMEM_H
#define SHAREDMEM_H

#include "Component.h"

union semun {
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
};


typedef struct SHMemQueue{
  int back; //newest queue element
  int next; //next elem to be retrieved for painting
  int checked;  //next elem to be retrived for checking
  int painted;  //next elem to be retrieved for assemblage
  char* elements; //ptr to where queue elements start (first element)
  int semid;  //semaphore set id
  /*
  sem0 down when there are no parts to be painted
  sem1 down when there are no parts to be checked
  sem2 down when there are no parts to be assembled
  */
} SHMemQueue;


/*Get size of queue*/
size_t MEMSIZE(int num_parts);

/*Get shared memory segment*/
int QueueInit(key_t shkey,int num_parts,key_t semkey);
/*Attach shared memory segment*/
char* QueueAttach(int shmid);
void QueueDetach(char* shm);
void QueueDelete(int shmid,key_t semkey);


/*Insert Component in shared memory*/
void QueueInsert(char* mem,Component* comp);

/*******GETTERS*******************************/
/*return pointer to back element*/
char* GetBackOffset(char* mem);
/*return semaphore id of queue at mem*/
int GetSemId(char* mem);
#endif
