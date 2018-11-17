#ifndef COMPONENT_H
#define COMPONENT_H

#include <sys/time.h>

typedef struct Component{
  int id;
  struct timeval creation_time;
  int type;
} Component;

int GenerateComponentID(int ptype);

#endif
