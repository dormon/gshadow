#ifndef _DEFERRED_HPP_
#define _DEFERRED_HPP_

#include<GL/glew.h>
#include<geGL/ProgramObject.h>
#include<geGL/TextureObject.h>
#include<geGL/FramebufferObject.h>

#include"shaderdir.hpp"

class Deferred{
  public:
    ge::gl::TextureObject*     position = nullptr;
    ge::gl::TextureObject*     color = nullptr;
    ge::gl::TextureObject*     normal = nullptr;
    ge::gl::TextureObject*     depth = nullptr;
    ge::gl::TextureObject*     stencil = nullptr;
    ge::gl::FramebufferObject* fbo = nullptr;
    ge::gl::ProgramObject*     createProgram = nullptr;
    unsigned size[2] = {1024,1024};

    Deferred();
    ~Deferred();
    Deferred(unsigned width,unsigned height,std::string shaderDir);
    void create(unsigned width,unsigned height,std::string shaderDir);
    void activate();
    void deactivate();
    void blitDepth2Default();
    void blitStencil2Default();
    void setTextures();
    void activateCreateStencil();
};

#endif//_DEFERRED_HPP_
