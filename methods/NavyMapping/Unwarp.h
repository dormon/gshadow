#pragma once

#include<geGL/geGL.h>

class Unwarp{
  protected:
    std::string           _dir        ;
    ge::gl::TextureObject*_viewSamples = NULL;
    ge::gl::TextureObject*_countMapY   = NULL;
    ge::gl::TextureObject*_desiredView = NULL;
    ge::gl::TextureObject*_smoothX     = NULL;
    ge::gl::TextureObject*_smoothY     = NULL;
    float                 _factor     ;
    unsigned              _resolution ;
    unsigned              _width      ;
    unsigned              _height     ;
    unsigned              _wgsx       ;
    unsigned              _wgsy       ;
    ge::gl::ProgramObject*_program     = NULL;
  public:
    Unwarp(
        std::string           dir        ,
        ge::gl::TextureObject*viewSamples,
        ge::gl::TextureObject*countMapY  ,
        ge::gl::TextureObject*desiredView,
        ge::gl::TextureObject*smoothX    ,
        ge::gl::TextureObject*smoothY    ,
        float                 factor     ,
        unsigned              resolution ,
        unsigned              width      ,
        unsigned              height     ,
        unsigned              wgsx       ,
        unsigned              wgsy       );

    ~Unwarp();
    void setViewSamples(ge::gl::TextureObject*viewSamples);
    void setCountMapY  (ge::gl::TextureObject*countMapY  );
    void setDesiredView(ge::gl::TextureObject*desiredView);
    void setSmoothX    (ge::gl::TextureObject*smoothX    );
    void setSmoothY    (ge::gl::TextureObject*smoothY    );
    void setFactor     (float                 factor     );
    void setResolution (unsigned              resolution );

    void operator()();
};
