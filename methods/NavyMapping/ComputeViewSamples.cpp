#include"ComputeViewSamples.h"
#include<geCore/dtemplates.h>

ComputeViewSamples::ComputeViewSamples(
    std::string           shaderDir  ,
    ge::gl::TextureObject*viewSamples,
    ge::gl::TextureObject*position   ,
    unsigned              width      ,
    unsigned              height     ,
    const GLfloat*        mvp        ,
    unsigned              wgsx       ,
    unsigned              wgsy       ){

  this->_dir         = shaderDir;
  this->_viewSamples = viewSamples;
  this->_position    = position;
  this->_width       = width;
  this->_height      = height;
  this->_mvp         = mvp;
  this->_wgsx        = wgsx;
  this->_wgsy        = wgsy;

  this->_program = new ge::gl::ProgramObject(
      this->_dir+"viewSamples.comp",
      ge::gl::ShaderObject::define("WORKGROUP_SIZE_X",(int)this->_wgsx)+
      ge::gl::ShaderObject::define("WORKGROUP_SIZE_Y",(int)this->_wgsy)+
      "");
}

void ComputeViewSamples::setViewSamples(ge::gl::TextureObject*viewSamples){
  this->_viewSamples = viewSamples;
}

void ComputeViewSamples::setPosition(ge::gl::TextureObject*position){
  this->_position = position;
}

void ComputeViewSamples::setMvp(const GLfloat*mvp){
  this->_mvp = mvp;
}


void ComputeViewSamples::operator()(){
  float data[]={2,2};
  glClearTexImage(this->_viewSamples->getId(),0,GL_RG,GL_FLOAT,data);

  this->_position->bind(GL_TEXTURE0);
  this->_viewSamples->bindImage(1,0);
  this->_program->use();
  this->_program->set("windowSize",this->_width,this->_height);
  this->_program->set("mvp",1,GL_FALSE,this->_mvp);
  unsigned workSizex=ge::core::getDispatchSize(this->_width ,this->_wgsx);
  unsigned workSizey=ge::core::getDispatchSize(this->_height,this->_wgsy);
  glDispatchCompute(workSizex,workSizey,1);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

ComputeViewSamples::~ComputeViewSamples(){
  delete this->_program;
}
