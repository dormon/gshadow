#include"UnwarpAll.h"

#include<geCore/dtemplates.h>

UnwarpAll::UnwarpAll(
    std::string           dir        ,
    ge::gl::TextureObject*viewSamples,
    ge::gl::TextureObject*countMap   ,
    ge::gl::TextureObject*desiredView,
    ge::gl::TextureObject*smoothX    ,
    ge::gl::TextureObject*smoothY    ,
    float                 factor     ,
    unsigned              resolution ,
    unsigned              width      ,
    unsigned              height     ){
  this->_dir         = dir        ;
  this->_viewSamples = viewSamples;
  this->_countMap    = countMap   ;
  this->_desiredView = desiredView;
  this->_smoothX     = smoothX    ;
  this->_smoothY     = smoothY    ;
  this->_factor      = factor     ;
  this->_resolution  = resolution ;
  this->_width       = width      ;
  this->_height      = height     ;

  this->_program   = new ge::gl::ProgramObject(
      this->_dir+"uall.comp",
      ge::gl::ShaderObject::define("WORKGROUP_SIZE_X",8)+
      ge::gl::ShaderObject::define("WORKGROUP_SIZE_Y",8)+
      ge::gl::ShaderObject::include(this->_dir+"dv.vp")+
      ge::gl::ShaderObject::include(this->_dir+"nv.vp")+
      "");
}

UnwarpAll::~UnwarpAll(){
  delete this->_program;
}

void UnwarpAll::setViewSamples(ge::gl::TextureObject*viewSamples){
  this->_viewSamples = viewSamples;
}

void UnwarpAll::setCountMap   (ge::gl::TextureObject*countMap   ){
  this->_countMap = countMap;
}

void UnwarpAll::setDesiredView(ge::gl::TextureObject*desiredView){
  this->_desiredView = desiredView;
}

void UnwarpAll::setSmoothX    (ge::gl::TextureObject*smoothX    ){
  this->_smoothX = smoothX;
}

void UnwarpAll::setSmoothY    (ge::gl::TextureObject*smoothY    ){
  this->_smoothY = smoothY;
}

void UnwarpAll::setFactor     (float                 factor     ){
  this->_factor = factor;
}

void UnwarpAll::setResolution (unsigned              resolution ){
  this->_resolution = resolution;
}

void UnwarpAll::operator()(){
  unsigned data[]={1};
  glClearTexImage(this->_countMap->getId(),0,GL_RED_INTEGER,GL_UNSIGNED_INT,data);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  this->_viewSamples->bindImage(0,0);
  this->_countMap   ->bindImage(1,0);

  this->_program->use();
  this->_program->set("shadowMapSize",this->_resolution);
  this->_program->set("windowSize",this->_width,this->_height);
  this->_program->set("useWarping",this->_factor>0.f);

  this->_desiredView->bind(GL_TEXTURE2);
  this->_smoothX->bind(GL_TEXTURE3);
  this->_smoothY->bind(GL_TEXTURE4);

  unsigned workSizex=ge::core::getDispatchSize(this->_width,8);
  unsigned workSizey=ge::core::getDispatchSize(this->_height,8);
  glDispatchCompute(workSizex,workSizey,1);
}

