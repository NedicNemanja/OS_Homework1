#ifndef COMPONENT_H
#define COMPONENT_H

#include <sys/time.h>

/***************Component random times************************************/
void SetPaintTimes();
void SetCheckTimes();
void SetAssemblageTime();
void PrintSetTimes();

#define MAX_MANUFACTURE_TIME 100 //in miliseconds
/*time in miliseconds required for paint for each Component type*/
double paint_time[3];
#define MAX_PAINT_TIME 100  //in miliseconds
/*time in miliseconds required for check for each Component type*/
double check_time[3];
#define MAX_CHECK_TIME 100 //in miliseconds
/*time in miliseconds required for assemblage*/
double assemblage_time;
#define MAX_ASSEMBLAGE_TIME 100 //in miliseconds

/***********************Component*********************************************/
typedef struct Component{
  int id; //4digits
  struct timeval creation_time;
  int type; //1,2 or 3
} Component;

int GenerateComponentID(int ptype);
void PrintComponent(Component* comp);
//time since creation_time in seconds
double CalcWaitTime(Component* comp);
/**************************Product********************************************/
typedef struct Product{
  long int id;  //12 digits
  struct timeval creation_time;
} Product;

Product CreateProduct(Component** comps);
//time since creation_time in seconds
double CalcProductWaitTime(Product prod);

#endif
