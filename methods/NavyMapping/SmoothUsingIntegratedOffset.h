#pragma once

#include<geGL/geGL.h>

class SmoothUsingIntegratedOffset{
  protected:
    std::string           _dir             ;
    ge::gl::TextureObject*_smooth           = NULL;
    ge::gl::TextureObject*_integratedOffset = NULL;
    ge::gl::ProgramObject*_program          = NULL;
    unsigned              _window          ;
    float                 _factor          ;
    unsigned              _resolution      ;
    unsigned              _wgsx            ;
    unsigned              _wgsy            ;
  public:
    SmoothUsingIntegratedOffset(
        std::string           dir             ,
        ge::gl::TextureObject*smooth          ,
        ge::gl::TextureObject*integratedOffset,
        unsigned              window          ,
        float                 factor          ,
        unsigned              resolution      ,
        unsigned              wgsx            ,
        unsigned              wgsy            );
    ~SmoothUsingIntegratedOffset();
    void setSmooth(ge::gl::TextureObject*smooth);
    void setIntegratedOffset(ge::gl::TextureObject*integratedOffset);
    void setResolution(unsigned resolution);
    void setWindow(unsigned window);
    void setFactor(float factor);
    void operator()();
};
