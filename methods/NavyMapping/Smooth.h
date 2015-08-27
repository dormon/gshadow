#pragma once

#include<geGL/geGL.h>

class Smooth{
  protected:
    std::string           _dir          ;
    ge::gl::TextureObject*_smooth        = NULL;
    ge::gl::TextureObject*_offset        = NULL;
    ge::gl::TextureObject*_integralCount = NULL;
    ge::gl::ProgramObject*_program       = NULL;
    unsigned              _window       ;
    float                 _factor       ;
    unsigned              _resolution   ;
    unsigned              _wgsx         ;
    unsigned              _wgsy         ;
  public:
    Smooth(
        std::string           dir          ,
        ge::gl::TextureObject*smooth       ,
        ge::gl::TextureObject*offset       ,
        ge::gl::TextureObject*integralCount,
        unsigned              window       ,
        float                 factor       ,
        unsigned              resolution   ,
        unsigned              wgsx         ,
        unsigned              wgsy         );
    ~Smooth();
    void setSmooth(ge::gl::TextureObject*smooth);
    void setOffset(ge::gl::TextureObject*offset);
    void setIntegralCount(ge::gl::TextureObject*integralCount);
    void setResolution(unsigned resolution);
    void setWindow(unsigned window);
    void setFactor(float factor);
    void operator()();
};
