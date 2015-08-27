#include"WholeWarp.h"

WholeWarp::WholeWarp(
    std::string dir,
    unsigned width,
    unsigned height,
    unsigned resolution,
    unsigned window    ,
    float    factor    ,
    unsigned createCountMapWGSX,
    unsigned createCountMapWGSY,
    unsigned unwarpWGSX,
    unsigned unwarpWGSY,
    unsigned integrateWGSX,
    unsigned createOffsetWGSX,
    unsigned createOffsetWGSY,
    unsigned smoothWGSX,
    unsigned smoothWGSY,
    unsigned integrateOffsetWGSX,
    unsigned smoothUsingIntegratedOffsetWGSX,
    unsigned smoothUsingIntegratedOffsetWGSY){

  this->_countMapX = new ge::gl::TextureObject(
      GL_TEXTURE_2D,GL_R32UI,1,resolution,resolution);
  this->_countMapY = new ge::gl::TextureObject(
      GL_TEXTURE_2D,GL_R32UI,1,resolution,resolution);

  this->_createCountMap = new CreateCountMap(
      dir,
      NULL,
      this->_countMapX,
      NULL,
      resolution,
      width,
      height,
      createCountMapWGSX,
      createCountMapWGSY);

  this->_warp = new Warp(
      dir,
      resolution,
      window,
      factor,
      integrateWGSX,
      createOffsetWGSX,
      createOffsetWGSY,
      smoothWGSX,
      smoothWGSY,
      integrateWGSX,
      smoothWGSX,
      smoothWGSY);

  this->_unwarp = new Unwarp(
      dir,
      NULL,
      this->_countMapY,
      NULL,
      NULL,
      NULL,
      factor,
      resolution,
      width,
      height,
      unwarpWGSX,
      unwarpWGSY);

}

WholeWarp::~WholeWarp(){
  delete this->_createCountMap;
  delete this->_warp;
  delete this->_unwarp;
  delete this->_countMapX;
  delete this->_countMapY;
}

void WholeWarp::setViewSamples(ge::gl::TextureObject*viewSamples){
  this->_createCountMap->setViewSamples(viewSamples);
  this->_unwarp->setViewSamples(viewSamples);
}

void WholeWarp::setDesiredView(ge::gl::TextureObject*desiredView){
  this->_createCountMap->setDesiredView(desiredView);
  this->_unwarp->setDesiredView(desiredView);
}

void WholeWarp::setResolution(unsigned resolution){
  this->_createCountMap->setResolution(resolution);
  this->_warp->setResolution(resolution);
  this->_unwarp->setResolution(resolution);
}

void WholeWarp::setWindow(unsigned window){
  this->_warp->setWindow(window);
}

void WholeWarp::setFactor(float factor){
  this->_factor = factor;
  this->_warp->setFactor(factor);
  this->_unwarp->setFactor(factor);
}

void WholeWarp::setFastSmooth(bool value){
  this->_warp->setFastSmooth(value);
}

void WholeWarp::setSmoothX(ge::gl::TextureObject*smoothX){
  this->_smoothX = smoothX;
  this->_unwarp->setSmoothX(smoothX);
}

void WholeWarp::setSmoothY(ge::gl::TextureObject*smoothY){
  this->_smoothY = smoothY;
  this->_unwarp->setSmoothY(smoothY);
}

void WholeWarp::setMeasureCountMap(simulation::GpuGauge*gauge){
  this->_measureCountMap = gauge;
}

void WholeWarp::setMeasureWholeX  (simulation::GpuGauge*gauge){
  this->_measureWholeX = gauge;
}

void WholeWarp::setMeasureUnwarpX (simulation::GpuGauge*gauge){
  this->_measureUnwarpX = gauge;
}

void WholeWarp::setMeasureWholeY  (simulation::GpuGauge*gauge){
  this->_measureWholeY = gauge;
}

void WholeWarp::setMeasureIntegrateX(simulation::GpuGauge*gauge){
  this->_measureIntegrateX = gauge;
}

void WholeWarp::setMeasureOffsetX   (simulation::GpuGauge*gauge){
  this->_measureOffsetX = gauge;
}

void WholeWarp::setMeasureSmoothX   (simulation::GpuGauge*gauge){
  this->_measureSmoothX = gauge;
}

void WholeWarp::setMeasureIntegrateY(simulation::GpuGauge*gauge){
  this->_measureIntegrateY = gauge;
}

void WholeWarp::setMeasureOffsetY   (simulation::GpuGauge*gauge){
  this->_measureSmoothY = gauge;
}

void WholeWarp::setMeasureSmoothY   (simulation::GpuGauge*gauge){
  this->_measureSmoothY = gauge;
}

void WholeWarp::operator()(){
  if(this->_factor<=0.f)return;
  this->_measureCountMap->begin();
  (*this->_createCountMap)();
  this->_measureCountMap->end();

  this->_measureWholeX->begin();
  this->_warp->setSmooth(this->_smoothX);
  this->_warp->setMeasureIntegrate(this->_measureIntegrateX);
  this->_warp->setMeasureOffset   (this->_measureOffsetX);
  this->_warp->setMeasureSmooth   (this->_measureSmoothX);
  this->_warp->setCountMap(this->_countMapX);
  (*this->_warp)();
  this->_measureWholeX->end();

  this->_measureUnwarpX->begin();
  (*this->_unwarp)();
  this->_measureUnwarpX->end();

  this->_measureWholeY->begin();
  this->_warp->setSmooth(this->_smoothY);
  this->_warp->setMeasureIntegrate(this->_measureIntegrateY);
  this->_warp->setMeasureOffset   (this->_measureOffsetY   );
  this->_warp->setMeasureSmooth   (this->_measureSmoothY   );
  this->_warp->setCountMap(this->_countMapY);
  (*this->_warp)();
  this->_measureWholeY->end();
}

