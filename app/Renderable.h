#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

#include<vector>
#include<geGL/VertexArrayObject.h>
#include<geGL/FramebufferObject.h>
#include<geGL/ProgramObject.h>

class RenderState
{
  protected:
    ge::gl::VertexArrayObject*_vao;
    ge::gl::FramebufferObject*_fbo;
    ge::gl::ProgramObject    *_program;
  public:
    RenderState(RenderState*renderState);
    ge::gl::VertexArrayObject*getVAO    ();
    ge::gl::FramebufferObject*getFBO    ();
    ge::gl::ProgramObject    *getProgram();
    void set  ();
    void reset();
};


class Renderable
{
  protected:
    RenderState *_renderState;
  public:
    Renderable(RenderState*renderState);
    virtual void draw()=0;
};

class DrawArrays: public Renderable
{
  protected:
    GLenum  _mode ;
    GLint   _first;
    GLsizei _count;
  public:
    virtual void draw();
};

#endif//_RENDERABLE_H_
