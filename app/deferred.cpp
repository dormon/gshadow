#include"deferred.hpp"

#include<iostream>
#include<stdlib.h>

void deferred_Init(
		SDeferred*D,
		unsigned Widht,
		unsigned Height){

  D->Create=new ge::gl::ProgramObject(
      ShaderDir+"app/cgb.vp",
      ShaderDir+"app/cgb.gp",
      ShaderDir+"app/cgb.fp");
  D->Size[0]=Widht;
  D->Size[1]=Height;

  D->color    = new ge::gl::TextureObject(GL_TEXTURE_2D       ,GL_RGBA16UI        ,1,Widht,Height);
  D->position = new ge::gl::TextureObject(GL_TEXTURE_2D       ,GL_RGBA32F         ,1,Widht,Height);
  D->normal   = new ge::gl::TextureObject(GL_TEXTURE_2D       ,GL_RGBA32F         ,1,Widht,Height);
  D->stencil  = new ge::gl::TextureObject(GL_TEXTURE_2D       ,GL_R32F            ,1,Widht,Height);
  D->depth    = new ge::gl::TextureObject(GL_TEXTURE_RECTANGLE,GL_DEPTH24_STENCIL8,1,Widht,Height);

  D->color   ->bind(GL_TEXTURE1);
  D->position->bind(GL_TEXTURE2);
  D->normal  ->bind(GL_TEXTURE3);
  D->stencil ->bind(GL_TEXTURE4);
  
  D->fbo=new ge::gl::FramebufferObject();
  D->fbo->attachColorTexture  (GL_COLOR_ATTACHMENT4,D->color   ->getId());
  D->fbo->attachColorTexture  (GL_COLOR_ATTACHMENT5,D->position->getId());
  D->fbo->attachColorTexture  (GL_COLOR_ATTACHMENT6,D->normal  ->getId());
  D->fbo->attachColorTexture  (GL_COLOR_ATTACHMENT7,D->stencil ->getId());
  D->fbo->attachDepthTexture  (D->depth->getId());
  D->fbo->attachStencilTexture(D->depth->getId());
  D->fbo->drawBuffers(4,
      GL_COLOR_ATTACHMENT4,
      GL_COLOR_ATTACHMENT5,
      GL_COLOR_ATTACHMENT6,
      GL_COLOR_ATTACHMENT7);
  if(!D->fbo->check())
    std::cerr<<"framebuffer je rozsypany\n";
}

void deferred_Free(SDeferred*D){
  delete D->Create;
  delete D->color;
  delete D->position;
  delete D->normal;
  delete D->stencil;
  delete D->depth;
  delete D->fbo;
}

void deferred_ClearStencil(SDeferred*D){
  D->fbo->bind();
  float Data[]={0,0,0,0};
  glClearBufferfv(GL_COLOR,GL_DRAW_BUFFER3,Data);
  D->fbo->unbind();
}

void deferred_StartCreateFrameBuffer(SDeferred*D){
  D->fbo->bind();
}

void deferred_EndCreateFrameBuffer(SDeferred*D){
  D->fbo->unbind();
}

void deferred_StartCreateStencil(SDeferred*D){
  D->fbo->bind();
  glClear(GL_STENCIL_BUFFER_BIT);
}

void deferred_EndCreateStencil(SDeferred*D){
  D->fbo->unbind();
}

void deferred_EnableFBO(SDeferred*D){
  D->fbo->bind();
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
  float clearPosition[]={0.f/0.f,0.f/0.f,0.f/0.f};
  glClearTexImage(D->position->getId(),0,GL_RGB,GL_FLOAT,clearPosition);
}

void deferred_EnableFBOStencil(SDeferred*D){
  D->fbo->bind();
}

void deferred_BlitStencilBuffer(SDeferred*D){
  D->fbo->bind(GL_READ_FRAMEBUFFER);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
  glBlitFramebuffer(0,0,D->Size[0],D->Size[1],0,0,D->Size[0],D->Size[1],GL_STENCIL_BUFFER_BIT,GL_NEAREST);
}
void deferred_BlitDepthToDefault(SDeferred*D){
  D->fbo->bind(GL_READ_FRAMEBUFFER);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
  glBlitFramebuffer(0,0,D->Size[0],D->Size[1],0,0,D->Size[0],D->Size[1],GL_DEPTH_BUFFER_BIT,GL_NEAREST);
}

void deferred_DisableFBO(SDeferred*D){
  D->fbo->unbind();
}

void deferred_SetTextures(SDeferred*D){
  D->color   ->bind(GL_TEXTURE1);
  D->position->bind(GL_TEXTURE2);
  D->normal  ->bind(GL_TEXTURE3);
  D->stencil ->bind(GL_TEXTURE4);
}

