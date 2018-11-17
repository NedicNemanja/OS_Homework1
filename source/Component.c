#include "Component.h"

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
