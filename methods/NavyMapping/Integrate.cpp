#include"Integrate.h"

Integrate::Integrate(
    std::string dir,
    ge::gl::TextureObject*integral,
    ge::gl::TextureObject*integralCount,
    ge::gl::TextureObject*countMap,
    unsigned resolution,
    unsigned wgsx){
  this->_dir = dir;
  this->_integral      = integral     ;
  this->_integralCount = integralCount;
  this->_countMap      = countMap     ;
  this->_resolution    = resolution   ;
  this->_wgsx = wgsx;

  this->_program = new ge::gl::ProgramObject(
      this->_dir+"integrate.comp",
      ge::gl::ShaderObject::define("WORKGROUP_SIZE_X",this->_wgsx)+
      "");
}

Integrate::~Integrate(){
  delete this->_program;
}

void Integrate::setResolution(unsigned resolution){
  this->_resolution = resolution;
}

void Integrate::setIntegral(ge::gl::TextureObject*integral){
  this->_integral = integral;
}

void Integrate::setIntegralCount(ge::gl::TextureObject*integralCount){
  this->_integralCount = integralCount;
}

void Integrate::setCountMap(ge::gl::TextureObject*countMap){
  this->_countMap = countMap;
}

void Integrate::operator()(){
  this->_countMap     ->bindImage(0,0);
  this->_integral     ->bindImage(1,0);
  this->_integralCount->bindImage(2,0);
  this->_program->use();
  this->_program->set("shadowMapSize",this->_resolution);
  unsigned workSizex=ge::core::getDispatchSize(this->_resolution,this->_wgsx);
  glDispatchCompute(workSizex,1,1);
}

