#pragma once

#include<geGL/geGL.h>

class UnwarpAll{
  protected:
    std::string _dir;
    ge::gl::TextureObject*_viewSamples = NULL;//input
    ge::gl::TextureObject*_desiredView = NULL;//input
    ge::gl::TextureObject*_smoothX     = NULL;//input
    ge::gl::TextureObject*_smoothY     = NULL;//input
    ge::gl::TextureObject*_countMap    = NULL;//output
    float                 _factor     ;
    unsigned              _resolution ;
    unsigned              _width      ;
    unsigned              _height     ;
    ge::gl::ProgramObject*_program     = NULL;
  public:
    UnwarpAll(
        std::string           dir        ,
        ge::gl::TextureObject*viewSamples,
        ge::gl::TextureObject*countMapY  ,
        ge::gl::TextureObject*desiredView,
        ge::gl::TextureObject*smoothX    ,
        ge::gl::TextureObject*smoothY    ,
        float                 factor     ,
        unsigned              resolution ,
        unsigned              width      ,
        unsigned              height     );
    ~UnwarpAll();
    void setViewSamples(ge::gl::TextureObject*viewSamples);
    void setCountMap   (ge::gl::TextureObject*countMapY  );
    void setDesiredView(ge::gl::TextureObject*desiredView);
    void setSmoothX    (ge::gl::TextureObject*smoothX    );
    void setSmoothY    (ge::gl::TextureObject*smoothY    );
    void setFactor     (float                 factor     );
    void setResolution (unsigned              resolution );
    void operator()();
};
