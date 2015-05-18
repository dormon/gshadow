#include"core.h"

unsigned getDispatchSize(unsigned workSize,unsigned workGroupSize){
  return workSize%workGroupSize?workSize/workGroupSize+1:workSize/workGroupSize;
}
