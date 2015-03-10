#ifndef _DEFERRED_HPP_
#define _DEFERRED_HPP_

#include<GL/glew.h>
#include<geGL/ProgramObject.h>
#include<geGL/TextureObject.h>
#include<geGL/FramebufferObject.h>

#include"shaderdir.hpp"

typedef struct{
  ge::gl::TextureObject*     position;
  ge::gl::TextureObject*     color;
  ge::gl::TextureObject*     normal;
  ge::gl::TextureObject*     depth;
  ge::gl::TextureObject*     stencil;
  ge::gl::FramebufferObject* fbo;
	ge::gl::ProgramObject*     Create;
	unsigned Size[2];
}SDeferred;

void deferred_Init(SDeferred*Deferred,unsigned Widht,unsigned Height);
void deferred_Free(SDeferred*Deferred);

void deferred_EnableFBO(SDeferred*D);
void deferred_DisableFBO(SDeferred*D);
void deferred_EnableFBOStencil(SDeferred*D);
void deferred_BlitStencilBuffer(SDeferred*D);
void deferred_BlitDepthToDefault(SDeferred*D);


void deferred_DrawQuad(SDeferred*D,unsigned*X,unsigned*S);


void deferred_StartCreateAtomic(SDeferred*D);
void deferred_EndCreateAtomic(SDeferred*D);
void deferred_ClearStencil(SDeferred*D);

void deferred_SetTextures(SDeferred*D);

void deferred_StartCreateFrameBuffer(SDeferred*D);
void deferred_EndCreateFrameBuffer(SDeferred*D);

void deferred_StartCreateStencil(SDeferred*D);
void deferred_EndCreateStencil(SDeferred*D);


#endif//_DEFERRED_HPP_
