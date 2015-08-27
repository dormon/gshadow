#pragma once

#include<geGL/geGL.h>

class CreateCountMap{
  protected:
    std::string _dir;
    ge::gl::ProgramObject*_program     = NULL;
    ge::gl::TextureObject*_viewSamples = NULL;
    ge::gl::TextureObject*_countMap    = NULL;
    ge::gl::TextureObject*_desiredView = NULL;
    unsigned _resolution;
    unsigned _width;
    unsigned _height;
    unsigned _wgsx;
    unsigned _wgsy;
  public:
    CreateCountMap(
        std::string dir,
        ge::gl::TextureObject*viewSamples,
        ge::gl::TextureObject*countMap   ,
        ge::gl::TextureObject*desiredView,
        unsigned resolution,
        unsigned width,
        unsigned height,
        unsigned wgsx,
        unsigned wgsy);
    ~CreateCountMap();
    void setResolution(unsigned resolution);
    void setViewSamples(ge::gl::TextureObject*viewSamples);
    void setCountMap   (ge::gl::TextureObject*countMap   );
    void setDesiredView(ge::gl::TextureObject*desiredView);
    void operator()();
};
