#include"Smooth.h"
#include<geCore/dtemplates.h>

Smooth::Smooth(
    std::string           dir          ,
    ge::gl::TextureObject*smooth       ,
    ge::gl::TextureObject*offset       ,
    ge::gl::TextureObject*integralCount,
    unsigned              window       ,
    float                 factor       ,
    unsigned              resolution   ,
    unsigned              wgsx         ,
    unsigned              wgsy         ){
  this->_dir           = dir          ; 
  this->_smooth        = smooth       ; 
  this->_offset        = offset       ; 
  this->_integralCount = integralCount;
  this->_window        = window       ; 
  this->_factor        = factor       ; 
  this->_resolution    = resolution   ; 
  this->_wgsx          = wgsx         ; 
  this->_wgsy          = wgsy         ; 

  this->_program = new ge::gl::ProgramObject(
      this->_dir+"smooth.comp",
      ge::gl::ShaderObject::define("WORKGROUP_SIZE_X",this->_wgsx)+
      ge::gl::ShaderObject::define("WORKGROUP_SIZE_Y",this->_wgsy)+
      "");
}

Smooth::~Smooth(){
  delete this->_program;
}

void Smooth::setSmooth(ge::gl::TextureObject*smooth){
  this->_smooth = smooth;
}

void Smooth::setOffset(ge::gl::TextureObject*offset){
  this->_offset = offset;
}

void Smooth::setIntegralCount(ge::gl::TextureObject*integralCount){
  this->_integralCount = integralCount;
}

void Smooth::setResolution(unsigned resolution){
  this->_resolution = resolution;
}

void Smooth::setWindow(unsigned window){
  this->_window = window;
}

void Smooth::setFactor(float factor){
  this->_factor = factor;
}

void Smooth::operator()(){
  this->_smooth       ->bindImage(0,0);
  this->_offset       ->bindImage(1,0);
  this->_integralCount->bindImage(2,0);
  this->_program->use();
  this->_program->set("shadowMapSize",this->_resolution);
  this->_program->set("smoothWindowSize",this->_window);
  this->_program->set("warpFactor",this->_factor);
  unsigned workSizex=ge::core::getDispatchSize(this->_resolution,this->_wgsx);
  unsigned workSizey=ge::core::getDispatchSize(this->_resolution,this->_wgsy);
  glDispatchCompute(workSizex,workSizey,1);
}


