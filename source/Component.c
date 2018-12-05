#include "Component.h"
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

static int ComponentType1_id;
static int ComponentType2_id;
static int ComponentType3_id;

int GenerateComponentID(int ptype){
  switch(ptype){
    case 1: return 1000+(ComponentType1_id++);
    case 2: return 2000+(ComponentType2_id++);
    case 3: return 3000+(ComponentType3_id++);
  }
}

Product CreateProduct(Component** comps){
  //create id and find oldest component
  long int id = comps[0]->id;
  double max_time = CalcWaitTime(comps[0]);
  Component* oldest_comp = comps[0];
  for(int i=1; i<3; i++){
    id *= 10000;  //shift 4 digits left
    id += comps[i]->id; //concat 4 new digits
    double t = CalcWaitTime(comps[i]);
    if(t > max_time){
      max_time = t;
      oldest_comp = comps[i];
    }
  }
  //create product
  Product p = {id, oldest_comp->creation_time};
  return p;
}

//time in seconds
double CalcWaitTime(Component* comp){
  struct timeval now;
  gettimeofday(&now,NULL);
  return (now.tv_sec-comp->creation_time.tv_sec)+(now.tv_usec-comp->creation_time.tv_usec)/1000000.0;
}

double CalcProductWaitTime(Product prod){
  struct timeval now;
  gettimeofday(&now,NULL);
  return (now.tv_sec-prod.creation_time.tv_sec)+(now.tv_usec-prod.creation_time.tv_usec)/1000000.0;
}

void PrintComponent(Component* comp){
  printf("id:%d type:%d\n", comp->id,comp->type);
}


void SetPaintTimes(){
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<3; i++){
    paint_time[i] = (rand()%MAX_PAINT_TIME)/1000.0;
    printf("\ttype%d paint_time:%f\n", i+1,paint_time[i]);
  }
}

void SetCheckTimes(){
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<3; i++){
    check_time[i] = (rand()%MAX_CHECK_TIME)/1000.0;
    printf("\ttype%d check_time: %f\n", i+1,check_time[i]);
  }
}


void SetAssemblageTime(){
  time_t t;
  srand((unsigned) time(&t));
  assemblage_time = (rand()%MAX_ASSEMBLAGE_TIME)/1000.0;
  printf("\tassemblage_time: %f\n", assemblage_time);
}

void PrintSetTimes(){
  for(int i=0; i<3; i++){
    printf("\ttype%d paint_time:%f\n", i+1,paint_time[i]);
  }
  for(int i=0; i<3; i++){
    printf("\ttype%d check_time: %f\n", i+1,check_time[i]);
  }
  printf("\tassemblage_time: %f\n", assemblage_time);
}
