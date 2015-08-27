#pragma once

#include<geGL/geGL.h>
#include"Integrate.h"
#include"CreateOffset.h"
#include"Smooth.h"
#include"IntegrateOffset.h"
#include"SmoothUsingIntegratedOffset.h"
#include"../../simulationData/StandardTypes.h"

class Warp{
  protected:
    ge::gl::TextureObject*_integrated       = NULL;
    ge::gl::TextureObject*_integratedCount  = NULL;
    ge::gl::TextureObject*_offset           = NULL;
    ge::gl::TextureObject*_integratedOffset = NULL;
    Integrate*                  _integrate                   = NULL;
    CreateOffset*               _createOffset                = NULL;
    Smooth*                     _smooth                      = NULL;
    IntegrateOffset*            _integrateOffset             = NULL;
    SmoothUsingIntegratedOffset*_smoothUsingIntegratedOffset = NULL;
    bool _useFastSmooth = false;
    simulation::GpuGauge*_measureIntegrate = NULL;
    simulation::GpuGauge*_measureOffset    = NULL;
    simulation::GpuGauge*_measureSmooth    = NULL;

  public:
    Warp(
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
        unsigned smoothUsingIntegratedOffsetWGSY);

    ~Warp();
    void setCountMap(ge::gl::TextureObject*countMap);//input
    void setResolution(unsigned resolution);//input
    void setSmooth(ge::gl::TextureObject*smooth);//output
    void setWindow(unsigned window);//input
    void setFactor(float factor);//input
    void setFastSmooth(bool value);//input
    void setMeasureIntegrate(simulation::GpuGauge*gauge);
    void setMeasureOffset   (simulation::GpuGauge*gauge);
    void setMeasureSmooth   (simulation::GpuGauge*gauge);
    void operator()();
};
