#include"Unwarp.h"

#include<geCore/dtemplates.h>
Unwarp::Unwarp(
    std::string           dir        ,
    ge::gl::TextureObject*viewSamples,
    ge::gl::TextureObject*countMapY  ,
    ge::gl::TextureObject*desiredView,
    ge::gl::TextureObject*smoothX    ,
    ge::gl::TextureObject*smoothY    ,
    float                 factor     ,
    unsigned              resolution ,
    unsigned              width      ,
    unsigned              height     ,
    unsigned              wgsx       ,
    unsigned              wgsy       ){
  this->_dir         = dir        ;
  this->_viewSamples = viewSamples;
  this->_countMapY   = countMapY  ;
  this->_desiredView = desiredView;
  this->_smoothX     = smoothX    ;
  this->_smoothY     = smoothY    ;
  this->_factor      = factor     ;
  this->_resolution  = resolution ;
  this->_width       = width      ;
  this->_height      = height     ;
  this->_wgsx        = wgsx       ;
  this->_wgsy        = wgsy       ;

  this->_program   = new ge::gl::ProgramObject(
      this->_dir+"unwarp.comp",
      ge::gl::ShaderObject::define("WORKGROUP_SIZE_X",this->_wgsx)+
      ge::gl::ShaderObject::define("WORKGROUP_SIZE_Y",this->_wgsy)+
      ge::gl::ShaderObject::include(this->_dir+"dv.vp")+
      ge::gl::ShaderObject::include(this->_dir+"nv.vp")+
      "");
}

Unwarp::~Unwarp(){
  delete this->_program;
}

void Unwarp::setViewSamples(ge::gl::TextureObject*viewSamples){
  this->_viewSamples = viewSamples;
}

void Unwarp::setCountMapY  (ge::gl::TextureObject*countMapY  ){
  this->_countMapY = countMapY;
}

void Unwarp::setDesiredView(ge::gl::TextureObject*desiredView){
  this->_desiredView = desiredView;
}

void Unwarp::setSmoothX    (ge::gl::TextureObject*smoothX    ){
  this->_smoothX = smoothX;
}

void Unwarp::setSmoothY    (ge::gl::TextureObject*smoothY    ){
  this->_smoothY = smoothY;
}

void Unwarp::setFactor     (float                 factor     ){
  this->_factor = factor;
}

void Unwarp::setResolution (unsigned              resolution ){
  this->_resolution = resolution;
}

void Unwarp::operator()(){
  unsigned data[]={1};
  glClearTexImage(this->_countMapY->getId(),0,GL_RED_INTEGER,GL_UNSIGNED_INT,data);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  this->_viewSamples->bindImage(0,0);
  this->_countMapY  ->bindImage(1,0);
  this->_program->use();
  this->_program->set("shadowMapSize",this->_resolution);
  this->_program->set("windowSize",this->_width,this->_height);

  this->_program->set("useWarping",this->_factor>0.f);
  //this->_desiredView->bindImage(2,0);
  this->_desiredView->bind(GL_TEXTURE2);
  this->_smoothX->bind(GL_TEXTURE3);
  this->_smoothY->bind(GL_TEXTURE4);

  unsigned workSizex=ge::core::getDispatchSize(this->_width ,this->_wgsx);
  unsigned workSizey=ge::core::getDispatchSize(this->_height,this->_wgsy);
  glDispatchCompute(workSizex,workSizey,1);
}
