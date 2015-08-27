#pragma once

#include<geGL/geGL.h>

class CreateOffset{
  protected:
    std::string           _dir          ;
    ge::gl::TextureObject*_offset        = NULL;
    ge::gl::TextureObject*_integral      = NULL;
    ge::gl::TextureObject*_integralCount = NULL;
    ge::gl::ProgramObject*_program       = NULL;
    unsigned              _resolution   ;
    unsigned              _wgsx         ;
    unsigned              _wgsy         ;
  public:
    CreateOffset(
        std::string           dir          ,   
        ge::gl::TextureObject*offset       ,   
        ge::gl::TextureObject*integral     ,   
        ge::gl::TextureObject*integralCount,   
        unsigned              resolution   ,   
        unsigned              wgsx         ,   
        unsigned              wgsy         );   
    ~CreateOffset();
    void setOffset(ge::gl::TextureObject*offset);
    void setIntegral(ge::gl::TextureObject*integral);
    void setIntegralCount(ge::gl::TextureObject*integralCount);
    void setResolution(unsigned resolution);
    void operator()();
};
