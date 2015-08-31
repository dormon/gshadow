#pragma once

#include<geGL/geGL.h>

#include"CreateCountMap.h"
#include"Warp.h"
#include"Unwarp.h"

class WholeWarp{
  protected:
    CreateCountMap*_createCountMap = NULL;
    Warp*          _warp           = NULL;
    Unwarp*        _unwarp         = NULL;
    ge::gl::TextureObject*_countMapX = NULL;
    ge::gl::TextureObject*_countMapY = NULL;
    ge::gl::TextureObject*_smoothX   = NULL;
    ge::gl::TextureObject*_smoothY   = NULL;
    float                 _factor    = 0.f;

    simulation::GpuGauge*_measureCountMap   = NULL;
    simulation::GpuGauge*_measureWholeX     = NULL;
    simulation::GpuGauge*_measureUnwarpX    = NULL;
    simulation::GpuGauge*_measureWholeY     = NULL;

    simulation::GpuGauge*_measureIntegrateX = NULL;
    simulation::GpuGauge*_measureOffsetX    = NULL;
    simulation::GpuGauge*_measureSmoothX    = NULL;

    simulation::GpuGauge*_measureIntegrateY = NULL;
    simulation::GpuGauge*_measureOffsetY    = NULL;
    simulation::GpuGauge*_measureSmoothY    = NULL;

  public:
    WholeWarp(
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
        unsigned smoothUsingIntegratedOffsetWGSY);
    ~WholeWarp();

    void setViewSamples(ge::gl::TextureObject*viewSamples);
    void setDesiredView(ge::gl::TextureObject*desiredView);
    void setResolution(unsigned resolution);//input
    void setWindow(unsigned window);//input
    void setFactor(float factor);//input
    void setFastSmooth(bool value);//input
    void setSmoothX(ge::gl::TextureObject*smoothX);//output
    void setSmoothY(ge::gl::TextureObject*smoothY);//output

    void setMeasureCountMap(simulation::GpuGauge*gauge);
    void setMeasureWholeX  (simulation::GpuGauge*gauge);
    void setMeasureUnwarpX (simulation::GpuGauge*gauge);
    void setMeasureWholeY  (simulation::GpuGauge*gauge);

    void setMeasureIntegrateX(simulation::GpuGauge*gauge);
    void setMeasureOffsetX   (simulation::GpuGauge*gauge);
    void setMeasureSmoothX   (simulation::GpuGauge*gauge);
    void setMeasureIntegrateY(simulation::GpuGauge*gauge);
    void setMeasureOffsetY   (simulation::GpuGauge*gauge);
    void setMeasureSmoothY   (simulation::GpuGauge*gauge);

    void operator()();

    ge::gl::TextureObject*getCountMap(){return this->_countMapX;}
};
