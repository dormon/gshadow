#include"Model2AABB.h"

#include"BoundingBox.h"

Model2AABB::Model2AABB(ge::gl::BufferObject**aabb,ModelPN*model){
  this->_aabb = aabb;
  this->_model = model;
}

void Model2AABB::operator()(){
  *this->_aabb=new ge::gl::BufferObject(sizeof(float)*4*2*this->_model->getNofMeshes());
  float*aabbData=(float*)(*this->_aabb)->map();
  for(unsigned m=0;m<this->_model->getNofMeshes();++m){
    AxisAlignBoundingBox aabb;
    for(unsigned v=0;v<this->_model->getNofVertices();++v){
      const float*data=this->_model->getPositions()+v*3;
      aabb.addPoint(glm::vec3(data[0],data[1],data[2]));

    }
    aabbData[m*4*2+0]=aabb.minPoint[0];
    aabbData[m*4*2+1]=aabb.minPoint[1];
    aabbData[m*4*2+2]=aabb.minPoint[2];
    aabbData[m*4*2+3]=1;

    aabbData[m*4*2+4]=aabb.maxPoint[0];
    aabbData[m*4*2+5]=aabb.maxPoint[1];
    aabbData[m*4*2+6]=aabb.maxPoint[2];
    aabbData[m*4*2+7]=1;
  }
  (*this->_aabb)->unmap();
}

