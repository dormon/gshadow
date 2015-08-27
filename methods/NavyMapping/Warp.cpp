#include"Warp.h"

Warp::Warp(
    std::string dir,
    unsigned resolution,
    unsigned window    ,
    float    factor    ,
    unsigned integrateWGSX,
    unsigned createOffsetWGSX,
    unsigned createOffsetWGSY,
    unsigned smoothWGSX,
    unsigned smoothWGSY,
    unsigned integrateOffsetWGSX,
    unsigned smoothUsingIntegratedOffsetWGSX,
    unsigned smoothUsingIntegratedOffsetWGSY){

  this->_integrated = new ge::gl::TextureObject(
      GL_TEXTURE_2D,GL_R32UI,1,resolution,resolution);

  this->_integratedCount = new ge::gl::TextureObject(
      GL_TEXTURE_1D,GL_R32UI,1,resolution);

  this->_offset = new ge::gl::TextureObject(
      GL_TEXTURE_2D,GL_R32F,1,resolution,resolution);
  this->_offset->texParameteri(GL_TEXTURE_WRAP_S    ,GL_CLAMP_TO_EDGE);
  this->_offset->texParameteri(GL_TEXTURE_WRAP_T    ,GL_CLAMP_TO_EDGE);
  this->_offset->texParameteri(GL_TEXTURE_MIN_FILTER,GL_LINEAR       );
  this->_offset->texParameteri(GL_TEXTURE_MAG_FILTER,GL_LINEAR)      ;

  this->_integratedOffset = new ge::gl::TextureObject(
      GL_TEXTURE_2D,GL_R32F,1,resolution,resolution);
  this->_integratedOffset->texParameteri(GL_TEXTURE_WRAP_S    ,GL_CLAMP_TO_EDGE);
  this->_integratedOffset->texParameteri(GL_TEXTURE_WRAP_T    ,GL_CLAMP_TO_EDGE);
  this->_integratedOffset->texParameteri(GL_TEXTURE_MIN_FILTER,GL_LINEAR       );
  this->_integratedOffset->texParameteri(GL_TEXTURE_MAG_FILTER,GL_LINEAR)      ;

  this->_integrate = new Integrate(
      dir,
      this->_integrated,
      this->_integratedCount,
      NULL,
      resolution,
      integrateWGSX);

  this->_createOffset = new CreateOffset(
      dir,
      this->_offset,
      this->_integrated,
      this->_integratedCount,
      resolution,
      createOffsetWGSX,
      createOffsetWGSY);

  this->_smooth = new Smooth(
      dir,
      NULL,
      this->_offset,
      this->_integratedCount,
      window,
      factor,
      resolution,
      smoothWGSX,
      smoothWGSY);

  this->_integrateOffset = new IntegrateOffset(
      dir,
      this->_integratedOffset,
      this->_offset,
      resolution,
      integrateOffsetWGSX);

  this->_smoothUsingIntegratedOffset = new SmoothUsingIntegratedOffset(
      dir,
      NULL,
      this->_integratedOffset,
      window,
      factor,
      resolution,
      smoothUsingIntegratedOffsetWGSX,
      smoothUsingIntegratedOffsetWGSY);
}

Warp::~Warp(){
  delete this->_integrated;
  delete this->_integratedCount;
  delete this->_offset;
  delete this->_integratedOffset;
  delete this->_integrate;
  delete this->_createOffset;
  delete this->_smooth;
  delete this->_integrateOffset;
  delete this->_smoothUsingIntegratedOffset;
}

void Warp::setCountMap(ge::gl::TextureObject*countMap){
  this->_integrate->setCountMap(countMap);
}

void Warp::setResolution(unsigned resolution){
  this->_createOffset->setResolution(resolution);
  this->_smooth->setResolution(resolution);
  this->_integrateOffset->setResolution(resolution);
  this->_smoothUsingIntegratedOffset->setResolution(resolution);
}

void Warp::setSmooth(ge::gl::TextureObject*smooth){
  this->_smooth->setSmooth(smooth);
  this->_smoothUsingIntegratedOffset->setSmooth(smooth);
}

void Warp::setWindow(unsigned window){
  this->_smooth->setWindow(window);
  this->_smoothUsingIntegratedOffset->setWindow(window);
}

void Warp::setFactor(float factor){
  this->_smooth->setFactor(factor);
  this->_smoothUsingIntegratedOffset->setFactor(factor);
}

void Warp::setFastSmooth(bool value){
  this->_useFastSmooth = value;
}

void Warp::setMeasureIntegrate(simulation::GpuGauge*gauge){
  this->_measureIntegrate = gauge;
}

void Warp::setMeasureOffset   (simulation::GpuGauge*gauge){
  this->_measureOffset = gauge;
}

void Warp::setMeasureSmooth   (simulation::GpuGauge*gauge){
  this->_measureSmooth = gauge;
}

void Warp::operator()(){
  this->_measureIntegrate->begin();
  (*this->_integrate)();
  this->_measureIntegrate->end();

  this->_measureOffset->begin();
  (*this->_createOffset)();
  this->_measureOffset->end();

  this->_measureSmooth->begin();
  if(!this->_useFastSmooth){
    (*this->_smooth)();
  }else{
    (*this->_integrateOffset)();
    (*this->_smoothUsingIntegratedOffset)();
  }
  this->_measureSmooth->end();
}

