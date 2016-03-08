#include"deferred.hpp"

#include<iostream>
#include<stdlib.h>

Deferred::Deferred(){}

Deferred::~Deferred(){
  delete this->createProgram;
  delete this->color;
  delete this->position;
  delete this->normal;
  delete this->stencil;
  delete this->depth;
  delete this->fbo;
}

Deferred::Deferred(unsigned width,unsigned height,std::string shaderDir){
  this->create(width,height,shaderDir);
}

void Deferred::create(unsigned width,unsigned height,std::string shaderDir){
  this->size[0]=width;
  this->size[1]=height;

  this->color    = new ge::gl::TextureObject(GL_TEXTURE_2D       ,GL_RGBA16UI        ,1,width,height);
  this->position = new ge::gl::TextureObject(GL_TEXTURE_2D       ,GL_RGBA32F         ,1,width,height);
  this->normal   = new ge::gl::TextureObject(GL_TEXTURE_2D       ,GL_RGBA32F         ,1,width,height);
  this->stencil  = new ge::gl::TextureObject(GL_TEXTURE_2D       ,GL_R32F            ,1,width,height);
  this->depth    = new ge::gl::TextureObject(GL_TEXTURE_RECTANGLE,GL_DEPTH24_STENCIL8,1,width,height);
  this->color   ->bind(1);
  this->position->bind(2);
  this->normal  ->bind(3);
  this->stencil ->bind(4);

  this->fbo=new ge::gl::FramebufferObject();
  this->fbo->attachTexture(GL_COLOR_ATTACHMENT4 ,this->color   ->getId());
  this->fbo->attachTexture(GL_COLOR_ATTACHMENT5 ,this->position->getId());
  this->fbo->attachTexture(GL_COLOR_ATTACHMENT6 ,this->normal  ->getId());
  this->fbo->attachTexture(GL_COLOR_ATTACHMENT7 ,this->stencil ->getId());
  this->fbo->attachTexture(GL_DEPTH_ATTACHMENT  ,this->depth->getId());
  this->fbo->attachTexture(GL_STENCIL_ATTACHMENT,this->depth->getId());
  this->fbo->drawBuffers(4,
      GL_COLOR_ATTACHMENT4,
      GL_COLOR_ATTACHMENT5,
      GL_COLOR_ATTACHMENT6,
      GL_COLOR_ATTACHMENT7);
  if(!this->fbo->check())
    std::cerr<<"framebuffer je rozsypany\n";

  this->createProgram=new ge::gl::ProgramObject(
      shaderDir+"app/cgb.vp",
      shaderDir+"app/cgb.fp");
}

void Deferred::activate(){
  this->fbo->bind();
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
  float clearPosition[]={0.f/0.f,0.f/0.f,0.f/0.f};
  glClearTexImage(this->position->getId(),0,GL_RGB,GL_FLOAT,clearPosition);
}

void Deferred::deactivate(){
  this->fbo->unbind();
}

void Deferred::blitDepth2Default(){
  this->fbo->bind(GL_READ_FRAMEBUFFER);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
  glBlitFramebuffer(0,0,this->size[0],this->size[1],0,0,this->size[0],this->size[1],GL_DEPTH_BUFFER_BIT,GL_NEAREST);
}

void Deferred::blitStencil2Default(){
  this->fbo->bind(GL_READ_FRAMEBUFFER);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
  glBlitFramebuffer(0,0,this->size[0],this->size[1],0,0,this->size[0],this->size[1],GL_STENCIL_BUFFER_BIT,GL_NEAREST);
}

void Deferred::setTextures(){
  this->color   ->bind(1);
  this->position->bind(2);
  this->normal  ->bind(3);
  this->stencil ->bind(4);
}

void Deferred::activateCreateStencil(){
  this->fbo->bind();
  glClear(GL_STENCIL_BUFFER_BIT);
}


