#include"CreateWarping.h"

ge::gl::TextureObject*createDesiredViewTexture(){
  return new ge::gl::TextureObject(GL_TEXTURE_2D,GL_RGBA32F,1,1,1);
}

ge::gl::TextureObject*createSmoothTexture(unsigned resolution){
  ge::gl::TextureObject*result=new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32F,1,resolution,resolution);
  result->texParameteri(GL_TEXTURE_WRAP_S    ,GL_CLAMP_TO_EDGE);
  result->texParameteri(GL_TEXTURE_WRAP_T    ,GL_CLAMP_TO_EDGE);
  result->texParameteri(GL_TEXTURE_MIN_FILTER,GL_LINEAR       );
  result->texParameteri(GL_TEXTURE_MAG_FILTER,GL_LINEAR)      ;
  return result;
}

CreateWarping::CreateWarping(
    std::string dir,
    unsigned width,
    unsigned height,
    unsigned resolution,
    unsigned window    ,
    float    factor    ,
    unsigned computeViewSamplesWGSX,
    unsigned computeViewSamplesWGSY,
    unsigned createDesiredViewWGSX,
    unsigned createDesiredViewWGSY,
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
  this->_viewSamples = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_RG32F,1,
      width,height);

  this->_computeViewSamples = new ComputeViewSamples(
      dir,
      this->_viewSamples,
      NULL,
      width,
      height,
      NULL,
      computeViewSamplesWGSX,
      computeViewSamplesWGSY);

  this->_createDesiredView = new CreateDesiredView(
      dir,
      width,
      height,
      this->_viewSamples,
      NULL,
      createDesiredViewWGSX,
      createDesiredViewWGSY);

  this->_wholeWarp = new WholeWarp(
      dir,
      width,
      height,
      resolution,
      window,
      factor,
      createCountMapWGSX,                
      createCountMapWGSY,
      unwarpWGSX,
      unwarpWGSY,
      integrateWGSX,
      createOffsetWGSX,
      createOffsetWGSY,
      smoothWGSX,
      smoothWGSY,
      integrateOffsetWGSX,
      smoothUsingIntegratedOffsetWGSX,
      smoothUsingIntegratedOffsetWGSY);
  this->_wholeWarp->setViewSamples(this->_viewSamples);
}

CreateWarping::~CreateWarping(){
  delete this->_computeViewSamples;
  delete this->_createDesiredView ;
  delete this->_wholeWarp         ;
  delete this->_viewSamples       ;
}

void CreateWarping::setPosition(ge::gl::TextureObject*position){
  this->_computeViewSamples->setPosition(position);
}

void CreateWarping::setMvp(const float*mvp){
  this->_computeViewSamples->setMvp(mvp);
}

void CreateWarping::setResolution(unsigned resolution){
  this->_wholeWarp->setResolution(resolution);
}

void CreateWarping::setWindow(unsigned window){
  this->_wholeWarp->setWindow(window);
}

void CreateWarping::setFactor(float factor){
  this->_wholeWarp->setFactor(factor);
}

void CreateWarping::setFastSmooth(bool value){
  this->_wholeWarp->setFastSmooth(value);
}

void CreateWarping::setSmoothX(ge::gl::TextureObject*smoothX){
  this->_wholeWarp->setSmoothX(smoothX);
}

void CreateWarping::setSmoothY(ge::gl::TextureObject*smoothY){
  this->_wholeWarp->setSmoothY(smoothY);
}

void CreateWarping::setDesiredView(ge::gl::TextureObject*desiredView){
  this->_createDesiredView->setDesiredView(desiredView);
  this->_wholeWarp->setDesiredView(desiredView);
}


void CreateWarping::setMeasureComputeViewSamples(simulation::GpuGauge*gauge){
  this->_measureComputeViewSamples = gauge;
}

void CreateWarping::setMeasureCreateDesiredView(simulation::GpuGauge*gauge){
  this->_measureCreateDesiredView = gauge;
}

void CreateWarping::setMeasureWholeWarp        (simulation::GpuGauge*gauge){
  this->_measureWholeWarp = gauge;
}

void CreateWarping::setMeasureCountMap(simulation::GpuGauge*gauge){
  this->_wholeWarp->setMeasureCountMap(gauge);
}

void CreateWarping::setMeasureWholeX  (simulation::GpuGauge*gauge){
  this->_wholeWarp->setMeasureWholeX(gauge);
}

void CreateWarping::setMeasureUnwarpX (simulation::GpuGauge*gauge){
  this->_wholeWarp->setMeasureUnwarpX(gauge);
}

void CreateWarping::setMeasureWholeY  (simulation::GpuGauge*gauge){
  this->_wholeWarp->setMeasureWholeY(gauge);
}

void CreateWarping::setMeasureIntegrateX(simulation::GpuGauge*gauge){
  this->_wholeWarp->setMeasureIntegrateX(gauge);
}

void CreateWarping::setMeasureOffsetX   (simulation::GpuGauge*gauge){
  this->_wholeWarp->setMeasureOffsetX(gauge);
}

void CreateWarping::setMeasureSmoothX   (simulation::GpuGauge*gauge){
  this->_wholeWarp->setMeasureSmoothX(gauge);
}

void CreateWarping::setMeasureIntegrateY(simulation::GpuGauge*gauge){
  this->_wholeWarp->setMeasureIntegrateY(gauge);
}

void CreateWarping::setMeasureOffsetY   (simulation::GpuGauge*gauge){
  this->_wholeWarp->setMeasureOffsetY(gauge);
}

void CreateWarping::setMeasureSmoothY   (simulation::GpuGauge*gauge){
  this->_wholeWarp->setMeasureSmoothY(gauge);
}

void CreateWarping::operator()(){
  this->_measureComputeViewSamples->begin();
  (*this->_computeViewSamples)();
  this->_measureComputeViewSamples->end();
  
  this->_measureCreateDesiredView->begin();
  (*this->_createDesiredView)();
  this->_measureCreateDesiredView->end();

  this->_measureWholeWarp->begin();
  (*this->_wholeWarp)();
  this->_measureWholeWarp->end();
}

