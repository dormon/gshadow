#include"deferred.hpp"

#include<iostream>
#include<stdlib.h>

void deferred_Init(
		SDeferred*D,
		unsigned Widht,
		unsigned Height){

  D->Create=new ge::gl::ProgramObject(
      ShaderDir+"app/vertex_cgb.vp",
      ShaderDir+"app/geometry_cgb.gp",
      ShaderDir+"app/fragment_cgb.fp");
  D->Size[0]=Widht;
  D->Size[1]=Height;

  D->color          = new ge::gl::TextureObject(GL_TEXTURE_2D       ,GL_RGBA16UI        ,1,Widht,Height);
  D->position       = new ge::gl::TextureObject(GL_TEXTURE_2D       ,GL_RGBA32F         ,1,Widht,Height);
  D->normal         = new ge::gl::TextureObject(GL_TEXTURE_2D       ,GL_RGBA32F         ,1,Widht,Height);
  D->stencil        = new ge::gl::TextureObject(GL_TEXTURE_2D       ,GL_R32F            ,1,Widht,Height);
  D->imageAtomicAdd = new ge::gl::TextureObject(GL_TEXTURE_2D       ,GL_R32I            ,1,Widht,Height);
  D->depth          = new ge::gl::TextureObject(GL_TEXTURE_RECTANGLE,GL_DEPTH24_STENCIL8,1,Widht,Height);

  D->color   ->bind(GL_TEXTURE1);
  D->position->bind(GL_TEXTURE2);
  D->normal  ->bind(GL_TEXTURE3);
  D->stencil ->bind(GL_TEXTURE4);
  D->imageAtomicAdd->bindImage(0,0);
  
  GLint maxUnits;
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,&maxUnits);
  std::cerr<<"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS: "<<maxUnits<<std::endl;
  maxUnits=6;
  for(GLint u=0;u<maxUnits;++u){
    GLint data;
    std::cerr<<"GL_TEXTURE"<<u<<std::endl;
    glGetIntegeri_v(GL_TEXTURE_BINDING_1D,u,&data);
    std::cerr<<"  GL_TEXTURE_BINDING_1D:                   "<<data<<std::endl;
    glGetIntegeri_v(GL_TEXTURE_BINDING_2D,u,&data);
    std::cerr<<"  GL_TEXTURE_BINDING_2D:                   "<<data<<std::endl;
    glGetIntegeri_v(GL_TEXTURE_BINDING_3D,u,&data);
    std::cerr<<"  GL_TEXTURE_BINDING_3D:                   "<<data<<std::endl;
    glGetIntegeri_v(GL_TEXTURE_BINDING_1D_ARRAY,u,&data);
    std::cerr<<"  GL_TEXTURE_BINDING_1D_ARRAY:             "<<data<<std::endl;
    glGetIntegeri_v(GL_TEXTURE_BINDING_2D_ARRAY,u,&data);
    std::cerr<<"  GL_TEXTURE_BINDING_2D_ARRAY:             "<<data<<std::endl;
    //glGetIntegeri_v(GL_TEXTURE_BINDING_CUBE_MAP_ARRAY,u,&data);
    //std::cerr<<"  GL_TEXTURE_BINDING_CUBE_MAP_ARRAY:       "<<data<<std::endl;
    glGetIntegeri_v(GL_TEXTURE_BINDING_RECTANGLE,u,&data);
    std::cerr<<"  GL_TEXTURE_BINDING_RECTANGLE:            "<<data<<std::endl;
    glGetIntegeri_v(GL_TEXTURE_BINDING_BUFFER,u,&data);
    std::cerr<<"  GL_TEXTURE_BINDING_BUFFER:               "<<data<<std::endl;
    glGetIntegeri_v(GL_TEXTURE_BINDING_CUBE_MAP,u,&data);
    std::cerr<<"  GL_TEXTURE_BINDING_CUBE_MAP:             "<<data<<std::endl;
    glActiveTexture(GL_TEXTURE0+u);
    glGetIntegerv(GL_SAMPLER_BINDING,&data);
    std::cerr<<"  GL_SAMPLER_BINDING:                      "<<data<<std::endl;

    //glGetIntegeri_v(GL_TEXTURE_BINDING_2D_MULTISAMPLE,u,&data);
    //std::cerr<<"  GL_TEXTURE_BINDING_2D_MULTISAMPLE:       "<<data<<std::endl;
    //glGetIntegeri_v(GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY,u,&data);
    //std::cerr<<"  GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY: "<<data<<std::endl;

  }
  //exit(0);

  glGenRenderbuffers(1,&D->RBO_Stencil);
  glBindRenderbuffer(GL_RENDERBUFFER,D->RBO_Stencil);
  glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,D->Size[0],D->Size[1]);

  D->fbo=new ge::gl::FramebufferObject();
  D->fbo->attachColorTexture  (GL_COLOR_ATTACHMENT4,D->color   ->getId());
  D->fbo->attachColorTexture  (GL_COLOR_ATTACHMENT5,D->position->getId());
  D->fbo->bind();
  //glFramebufferTextureLayer(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT5,D->position->getId(),0,0);
  //glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT5,D->position->getId(),0);
  //glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT5,GL_TEXTURE_2D_MULTISAMPLE,D->position->getId(),0);
  D->fbo->unbind();
  //exit(0);
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

  std::cerr<<D->fbo->getInfo();


}

void deferred_Free(SDeferred*D){
  delete D->Create;
  delete D->color;
  delete D->position;
  delete D->normal;
  delete D->stencil;
  delete D->imageAtomicAdd;
  delete D->depth;
  delete D->fbo;
}

void deferred_Clear(SDeferred*D){
}

void deferred_ClearStencil(SDeferred*D){
  D->fbo->bind();
  float Data[]={0,0,0,0};
  glClearBufferfv(GL_COLOR,GL_DRAW_BUFFER3,Data);
  D->fbo->unbind();
}

void deferred_StartCreateAtomic(SDeferred*D){
  D->fbo->bind();
}

void deferred_EndCreateAtomic(SDeferred*D){
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
  glBlitFramebuffer(0,0,D->Size[0],D->Size[1],0,0,D->Size[0],D->Size[1],
      GL_STENCIL_BUFFER_BIT/*GL_COLOR_BUFFER_BIT*/,GL_NEAREST);
}
void deferred_BlitDepthToDefault(SDeferred*D){
  D->fbo->bind(GL_READ_FRAMEBUFFER);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
  glBlitFramebuffer(0,0,D->Size[0],D->Size[1],0,0,D->Size[0],D->Size[1],
      GL_DEPTH_BUFFER_BIT,GL_NEAREST);
}

void deferred_DisableFBO(SDeferred*D){
  D->fbo->unbind();
}

void deferred_SetTextures(SDeferred*D){
  D->color   ->bind(GL_TEXTURE1);
  D->position->bind(GL_TEXTURE2);
  D->normal  ->bind(GL_TEXTURE3);
  D->stencil ->bind(GL_TEXTURE4);

  glActiveTexture(GL_TEXTURE5);
  glBindTexture(GL_TEXTURE_2D,D->imageAtomicAdd->getId());
}

