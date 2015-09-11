#include"Model2DIBO.h"

Model2DIBO::Model2DIBO(ge::gl::BufferObject**dibo,ModelPN*model){
  this->_dibo  = dibo;
  this->_model = model;
}

void Model2DIBO::operator()(){
  *this->_dibo=new ge::gl::BufferObject(sizeof(unsigned)*4*this->_model->getNofMeshes());
  unsigned*dibo=(unsigned*)(*this->_dibo)->map();
  for(unsigned m=0;m<this->_model->getNofMeshes();++m){
    dibo[m*4+0]=this->_model->getMeshSize(m);
    dibo[m*4+1]=1;
    dibo[m*4+2]=this->_model->getMeshOffset(m);
    dibo[m*4+3]=0;
  }
  (*this->_dibo)->unmap();
}

