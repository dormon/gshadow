#pragma once

#include<geGL/geGL.h>

class CreateNavyShadowMap{
  protected:
    ge::gl::ProgramObject*_program     = NULL;
    ge::gl::TextureObject*_smoothX     = NULL;
    ge::gl::TextureObject*_smoothY     = NULL;
    ge::gl::TextureObject*_desiredView = NULL;
    ge::gl::FramebufferObject*_fbo     = NULL;
    ge::gl::VertexArrayObject*_vao     = NULL;
    const float*_mvp           = NULL ;
    unsigned    _resolution    = 1024 ;
    unsigned    _tessFactor    = 16   ;
    bool        _cullTriangles = false;
    unsigned    _width;
    unsigned    _height;
    unsigned    _nofTriangles;
    bool        _lineToSM = false;
    float       _polygonOffsetFactor = 1;
    float       _polygonOffsetUnits = 1;
    float       _factor = 0;
  public:
    CreateNavyShadowMap(
        std::string dir,
        unsigned width,
        unsigned height,
        ge::gl::VertexArrayObject*sceneVAO,
        unsigned nofTriangles);
    ~CreateNavyShadowMap();
    void setShadowMap(ge::gl::TextureObject*shadowMap,GLenum target = GL_TEXTURE_2D);
    void setSmoothX(ge::gl::TextureObject*smoothX);
    void setSmoothY(ge::gl::TextureObject*smoothY);
    void setDesiredView(ge::gl::TextureObject*desiredView);
    void setMvp(const float*mvp);
    void setResolution(unsigned resolution);
    void setTessFactor(unsigned tessFactor);
    void setCullTriangles(bool cull);
    void setLineToSM(bool lineToSM);
    void setPolygonOffsetFactor(float polygonOffsetFactor);
    void setPolygonOffsetUnits(float polygonOffsetUnits);
    void setFactor(float factor);
    void operator()();
};
