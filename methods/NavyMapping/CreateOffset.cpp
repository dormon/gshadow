#include"CreateOffset.h"
#include"geCore/dtemplates.h"

CreateOffset::CreateOffset(
    std::string           dir          ,   
    ge::gl::TextureObject*offset       ,   
    ge::gl::TextureObject*integral     ,   
    ge::gl::TextureObject*integralCount,   
    unsigned              resolution   ,   
    unsigned              wgsx         ,   
    unsigned              wgsy         ){
  this->_dir          = dir          ;
  this->_offset       = offset       ;
  this->_integral     = integral     ;
  this->_integralCount= integralCount;
  this->_resolution   = resolution   ;
  this->_wgsx         = wgsx         ;
  this->_wgsy         = wgsy         ;

  this->_program = new ge::gl::ProgramObject(
      this->_dir+"offset.comp",
      ge::gl::ShaderObject::define("WORKGROUP_SIZE_X",this->_wgsx)+
      ge::gl::ShaderObject::define("WORKGROUP_SIZE_Y",this->_wgsy)+
      "");
}

CreateOffset::~CreateOffset(){
  delete this->_program;
}

void CreateOffset::setOffset(ge::gl::TextureObject*offset){
  this->_offset = offset;
}

void CreateOffset::setIntegral(ge::gl::TextureObject*integral){
  this->_integral = integral;
}

void CreateOffset::setIntegralCount(ge::gl::TextureObject*integralCount){
  this->_integralCount = integralCount;
}

void CreateOffset::setResolution(unsigned resolution){
  this->_resolution = resolution;
}

void CreateOffset::operator()(){
  this->_offset       ->bindImage(0,0);
  this->_integral     ->bindImage(1,0);
  this->_integralCount->bindImage(2,0);
  this->_program->use();
  this->_program->set("shadowMapSize",this->_resolution);
  unsigned workSizex=ge::core::getDispatchSize(this->_resolution,this->_wgsx);
  unsigned workSizey=ge::core::getDispatchSize(this->_resolution,this->_wgsy);
  glDispatchCompute(workSizex,workSizey,1);
}

