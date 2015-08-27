#include"IntegrateOffset.h"

#include<geCore/dtemplates.h>

IntegrateOffset::IntegrateOffset(
    std::string           dir             ,
    ge::gl::TextureObject*integratedOffset,
    ge::gl::TextureObject*offset          ,
    unsigned              resolution      ,
    unsigned              wgsx            ){
  this->_dir              = dir             ;
  this->_integratedOffset = integratedOffset;
  this->_offset           = offset         ; 
  this->_resolution       = resolution     ; 
  this->_wgsx             = wgsx           ; 
  this->_program = new ge::gl::ProgramObject(
      this->_dir+"integrateoffset.comp",
      ge::gl::ShaderObject::define("WORKGROUP_SIZE_X",this->_wgsx)+
      "");
}

IntegrateOffset::~IntegrateOffset(){
  delete this->_program;
}

void IntegrateOffset::setIntegratedOffet(ge::gl::TextureObject*integratedOffset){
  this->_integratedOffset = integratedOffset;
}

void IntegrateOffset::setOffset         (ge::gl::TextureObject*offset          ){
  this->_offset = offset;
}

void IntegrateOffset::setResolution     (unsigned resolution                   ){
  this->_resolution = resolution;
}

void IntegrateOffset::operator()(){
  this->_integratedOffset->bindImage(0,0);
  this->_offset          ->bindImage(1,0);
  this->_program->use();
  this->_program->set("shadowMapSize",this->_resolution);
  unsigned workSizex=ge::core::getDispatchSize(this->_resolution,this->_wgsx);
  glDispatchCompute(workSizex,1,1);
}

