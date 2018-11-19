#ifndef SHAREDMEM_H
#define SHAREDMEM_H

#include "Component.h"
#include <sys/types.h>

union semun {
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
};

/*HERE MAYBE SET THESE INTS TO ATOMIC VOLAITAILE*/




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

typedef enum SEMNUM { paint_sem=0,check_sem=1,assemble_sem=2 }SEMNUM;
SEMNUM SEM_NUMBER;

/***********Initialization and Destruction*************************************/
/*Get size of queue memory for queue elements (in bytes)*/
size_t MEMSIZE(int num_parts);

/*Get shared memory segment*/
int QueueInit(key_t shkey,int num_parts,key_t semkey);
/*Attach shared memory segment*/
char* QueueAttach(int shmid);
void QueueDetach(char* shm);
void QueueDelete(int shmid,key_t semkey);

/************QUEUE OPS*********************************************************/
/*Insert Component in shared memory*/
void QueueInsert(SHMemQueue* queue,Component* comp);
/*Paint the next component of the queue*/
void QueuePaint(SHMemQueue* queue,int type);
/*Check the next painted component of the queue*/
void QueueCheck(SHMemQueue* queue,int type);

/************GETTERS***********************************************************/
/*return pointer to back element*/
char* GetBackOffset(SHMemQueue* queue);

#endif
