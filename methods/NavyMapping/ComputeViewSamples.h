#pragma once

#include<geGL/geGL.h>

class ComputeViewSamples{
  protected:
    ge::gl::TextureObject*_viewSamples = NULL;
    ge::gl::TextureObject*_position    = NULL;
    ge::gl::ProgramObject*_program     = NULL;
    unsigned              _width      ;
    unsigned              _height     ;
    const GLfloat*        _mvp        ;
    std::string           _dir        ;
    unsigned              _wgsx       ;
    unsigned              _wgsy       ;
  public:
    ComputeViewSamples(
        std::string           shaderDir  ,
        ge::gl::TextureObject*viewSamples,
        ge::gl::TextureObject*position   ,
        unsigned              width      ,
        unsigned              height     ,
        const GLfloat*        mvp        ,
        unsigned              wgsx       ,
        unsigned              wgsy       );
    ~ComputeViewSamples();
    void setViewSamples(ge::gl::TextureObject*viewSamples);
    void setPosition(ge::gl::TextureObject*position);
    void setMvp(const GLfloat*mvp);
    void operator()();
};
