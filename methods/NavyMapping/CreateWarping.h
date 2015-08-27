#pragma once

#include"ComputeViewSamples.h"
#include"CreateDesiredView.h"
#include"WholeWarp.h"

ge::gl::TextureObject*createDesiredViewTexture();
ge::gl::TextureObject*createSmoothTexture(unsigned resolution);

class CreateWarping{
  protected:
    ComputeViewSamples* _computeViewSamples = NULL;
    CreateDesiredView*  _createDesiredView  = NULL;
    WholeWarp*          _wholeWarp          = NULL;
    ge::gl::TextureObject* _viewSamples = NULL;

    simulation::GpuGauge*_measureComputeViewSamples = NULL;
    simulation::GpuGauge*_measureCreateDesiredView  = NULL;
    simulation::GpuGauge*_measureWholeWarp          = NULL;
  public:
    CreateWarping(
        std::string dir,
        unsigned width,
        unsigned height,
        unsigned resolution,
        unsigned window    ,
        float    factor    ,
        unsigned computeViewSamplesWGSX = 8,
        unsigned computeViewSamplesWGSY = 8,
        unsigned createDesiredViewWGSX = 8,
        unsigned createDesiredViewWGSY = 8,
        unsigned createCountMapWGSX = 8,
        unsigned createCountMapWGSY = 8,
        unsigned unwarpWGSX = 8,
        unsigned unwarpWGSY = 8,
        unsigned integrateWGSX = 64,
        unsigned createOffsetWGSX = 8,
        unsigned createOffsetWGSY = 8,
        unsigned smoothWGSX = 8,
        unsigned smoothWGSY = 8,
        unsigned integrateOffsetWGSX = 64,
        unsigned smoothUsingIntegratedOffsetWGSX = 8,
        unsigned smoothUsingIntegratedOffsetWGSY = 8);
    ~CreateWarping();
    void setPosition(ge::gl::TextureObject*position);//input
    void setMvp(const float*mvp);//input
    void setResolution(unsigned resolution);//input
    void setWindow(unsigned window);//input
    void setFactor(float factor);//input
    void setFastSmooth(bool value);//input
    void setSmoothX(ge::gl::TextureObject*smoothX);//output
    void setSmoothY(ge::gl::TextureObject*smoothY);//output
    void setDesiredView(ge::gl::TextureObject*desiredView);//output


    void setMeasureComputeViewSamples(simulation::GpuGauge*gauge);
    void setMeasureCreateDesiredView(simulation::GpuGauge*gauge);
    void setMeasureWholeWarp        (simulation::GpuGauge*gauge);

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
};
