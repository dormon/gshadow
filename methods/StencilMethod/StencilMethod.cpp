#include"StencilMethod.h"

StencilMethod::StencilMethod(SAdjecency*adjacency){
  this->adjacency=adjacency;
}

void StencilMethod::draw(float*mvp,simulation::Light*light,bool zfail){
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS,0,0);
  if(zfail){
    glStencilOpSeparate(GL_FRONT,GL_KEEP,GL_INCR_WRAP,GL_KEEP);
    glStencilOpSeparate(GL_BACK ,GL_KEEP,GL_DECR_WRAP,GL_KEEP);
  }else{
    glStencilOpSeparate(GL_FRONT,GL_KEEP,GL_KEEP,GL_INCR_WRAP);
    glStencilOpSeparate(GL_BACK ,GL_KEEP,GL_KEEP,GL_DECR_WRAP);
  }
  glDepthFunc(GL_LESS);
  glDepthMask(GL_FALSE);

  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
  this->drawGeometry(mvp,light);
  glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
  glStencilFunc(GL_EQUAL,0,0xff);

  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
}
