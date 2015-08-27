#include"SmoothUsingIntegratedOffset.h"

SmoothUsingIntegratedOffset::SmoothUsingIntegratedOffset(
    std::string           dir             ,
    ge::gl::TextureObject*smooth          ,
    ge::gl::TextureObject*integratedOffset,
    unsigned              window          ,
    float                 factor          ,
    unsigned              resolution      ,
    unsigned              wgsx            ,
    unsigned              wgsy            ){
  this->_dir              = dir             ;
  this->_smooth           = smooth          ;
  this->_integratedOffset = integratedOffset;
  this->_window           = window          ;
  this->_factor           = factor          ;
  this->_resolution       = resolution      ;
  this->_wgsx             = wgsx            ;
  this->_wgsy             = wgsy            ;

  this->_program = new ge::gl::ProgramObject(
      this->_dir+"smoothUsingIntegratedOffset.comp",
      ge::gl::ShaderObject::define("WORKGROUP_SIZE_X",this->_wgsx)+
      ge::gl::ShaderObject::define("WORKGROUP_SIZE_Y",this->_wgsy)+
      "");
}

SmoothUsingIntegratedOffset::~SmoothUsingIntegratedOffset(){
  delete this->_program;
}

void SmoothUsingIntegratedOffset::setSmooth(ge::gl::TextureObject*smooth){
  this->_smooth = smooth;
}

void SmoothUsingIntegratedOffset::setIntegratedOffset(ge::gl::TextureObject*integratedOffset){
  this->_integratedOffset = integratedOffset;
}

void SmoothUsingIntegratedOffset::setResolution(unsigned resolution){
  this->_resolution = resolution;
}

void SmoothUsingIntegratedOffset::setWindow(unsigned window){
  this->_window = window;
}

void SmoothUsingIntegratedOffset::setFactor(float factor){
  this->_factor = factor;
}

void SmoothUsingIntegratedOffset::operator()(){
  this->_smooth          ->bindImage(0,0);
  this->_integratedOffset->bindImage(1,0);
  this->_program->use();
  this->_program->set("shadowMapSize",this->_resolution);
  this->_program->set("smoothWindowSize",this->_window);
  this->_program->set("warpFactor",this->_factor);
  unsigned workSizex=ge::core::getDispatchSize(this->_resolution,this->_wgsx);
  unsigned workSizey=ge::core::getDispatchSize(this->_resolution,this->_wgsy);
  glDispatchCompute(workSizex,workSizey,1);
}


