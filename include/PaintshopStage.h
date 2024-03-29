#ifndef PAINTSHOPSTAGE_H
#define PAINTSHOPSTAGE_H

#include "SharedMem.h"

int PaintshopStage(int* queueids, int num_parts);

/*Find the component type with the min number of components available for the
checking stage (to avoid checking processes being idle).
But ignore queues whose paint_semaphores are down, you don't have any components
to pull from them anyway.
Return values 1,2,3 for component type 1,2 and 3. Default is 1,
if all paint_sem are down.*/
int PickComponentType(SHMemQueue** qs, int* checksem,int* paintsem, int nparts);

#endif
