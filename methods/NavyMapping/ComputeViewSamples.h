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
    bool                  _useDP      ;
    float                 _near       ;
    float                 _far        ;
  public:
    ComputeViewSamples(
        std::string           shaderDir  ,
        ge::gl::TextureObject*viewSamples,
        ge::gl::TextureObject*position   ,
        unsigned              width      ,
        unsigned              height     ,
        const GLfloat*        mvp        ,
        unsigned              wgsx       ,
        unsigned              wgsy       ,
        std::string           dpProjFile = "");
    ~ComputeViewSamples();
    void setViewSamples(ge::gl::TextureObject*viewSamples);
    void setPosition(ge::gl::TextureObject*position);
    void setMvp(const GLfloat*mvp);
    void setNear(float near){this->_near = near;}
    void setFar(float far){this->_far = far;}
    void operator()();
};
