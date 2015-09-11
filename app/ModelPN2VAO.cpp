#include"ModelPN2VAO.h"

ModelPN2VAO::ModelPN2VAO(
    ge::gl::VertexArrayObject**vao,
    ge::gl::BufferObject**vbo,
    ModelPN *model){
  this->_vao = vao;
  this->_vbo = vbo;
  this->_model = model;
}

void ModelPN2VAO::operator()(){
  *this->_vao=new ge::gl::VertexArrayObject();
  *this->_vbo=new ge::gl::BufferObject(sizeof(float)*this->_model->getNofVertices()*2*3);
  float*ptr=(float*)(*this->_vbo)->map();
  for(unsigned m=0;m<this->_model->getNofMeshes();++m){
    for(unsigned v=0;v<this->_model->getMeshSize(m);++v){
      for(unsigned k=0;k<3;++k)
        ptr[((this->_model->getMeshOffset(m)+v)*2+0)*3+k]=this->_model->getPositions()[(this->_model->getMeshOffset(m)+v)*3+k];
      for(unsigned k=0;k<3;++k)
        ptr[((this->_model->getMeshOffset(m)+v)*2+1)*3+k]=this->_model->getNormals()[(this->_model->getMeshOffset(m)+v)*3+k];
    }
  }
  (*this->_vbo)->unmap();
  (*this->_vao)->addAttrib(*this->_vbo,0,3,GL_FLOAT,sizeof(float)*6,(GLvoid*)(sizeof(float)*0));
  (*this->_vao)->addAttrib(*this->_vbo,1,3,GL_FLOAT,sizeof(float)*6,(GLvoid*)(sizeof(float)*3));

}

