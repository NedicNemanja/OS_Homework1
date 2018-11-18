#ifndef MANUFACTURESTAGE_H
#define MANUFACTURESTAGE_H

#include <sys/types.h> //key_t

#define MAX_MANUFACTURE_TIME 500 //in miliseconds
#define SleepTime(r) ((r%MAX_MANUFACTURE_TIME)/1000.0)

int ManufactureStage(int queueid,int ptype,int num_parts,key_t semkey);

#endif
