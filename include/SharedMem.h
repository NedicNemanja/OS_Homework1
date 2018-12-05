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

typedef struct SHMemQueue{
  volatile int back; //newest queue element
  volatile int next; //next elem to be retrieved for painting
  volatile int painted;  //next elem to be retrived for checking
  volatile int checked;  //next elem to be retrieved for assemblage
  key_t paint_semkey,check_semkey,assemble_semkey;
} SHMemQueue;

/***********Initialization and Destruction*************************************/
/*Get size of queue memory for queue elements (in bytes)*/
size_t MEMSIZE(int num_parts);

/*Get shared memory segment and semaphore keys with ftok*/
int QueueInit(key_t shkey,int num_parts);
/*Attach shared memory segment*/
char* QueueAttach(int shmid);
void QueueDeleteSemaphores(SHMemQueue* queue);
void QueueDetach(char* shm);
void QueueDelete(int shmid);

/************QUEUE OPS*********************************************************/
/*Insert Component in shared memory*/
void QueueInsert(SHMemQueue* queue,Component* comp);
/*Paint the next component of the queue*/
Component* QueuePaint(SHMemQueue* queue,int type);
/*Check the next painted component of the queue*/
void QueueCheck(SHMemQueue* queue,int type);
Product QueueAssemble(SHMemQueue** queues);
/************GETTERS***********************************************************/
/*return pointer to element at offset*/
char* GetOffset(SHMemQueue* queue, int offset);

/**********SEMAPHORE OPS******************************************************/
int SemGet(key_t semkey);
void SemDown(int semid);
void SemUp(int semid);
void SemDelete(int semid);
#endif
