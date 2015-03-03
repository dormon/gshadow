#include"BoundingBox.h"

#include<limits>
#include<cmath>

AxisAlignBoundingBox::AxisAlignBoundingBox(){
  for(int k=0;k<3;++k){
    this->minPoint[k]=+std::numeric_limits<float>::infinity();
    this->maxPoint[k]=-std::numeric_limits<float>::infinity();
  }
}

void AxisAlignBoundingBox::addPoint(glm::vec3 point){
  for(int k=0;k<3;++k){
    this->minPoint[k]=std::fmin(this->minPoint[k],point[k]);
    this->maxPoint[k]=std::fmax(this->maxPoint[k],point[k]);
  }
}

glm::vec3 AxisAlignBoundingBox::getCenter(){
  return (this->minPoint+this->maxPoint)*.5f;
}
glm::vec3 AxisAlignBoundingBox::getSize(){
  return this->maxPoint-this->minPoint;
}
glm::vec3 AxisAlignBoundingBox::getHalfSize(){
  return this->getSize()*.5f;
}

