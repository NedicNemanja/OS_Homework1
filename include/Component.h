#ifndef COMPONENT_H
#define COMPONENT_H

#include <sys/time.h>

/*time in miliseconds required for paint/check for each Component type*/
double paint_time[3],check_time[3];

typedef struct Component{
  int id;
  struct timeval creation_time;
  int type;
} Component;

int GenerateComponentID(int ptype);

#endif
