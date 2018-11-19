#ifndef CHECKSTAGE_H
#define CHECKSTAGE_H

int CheckStage(int queueid,int ptype,int num_parts);

#define MAX_CHECK_TIME 200 //in miliseconds
void SetCheckTimes();

#endif
