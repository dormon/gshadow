#ifndef _DEFERRED_HPP_
#define _DEFERRED_HPP_

#include<GL/glew.h>
#include<geGL/ProgramObject.h>
#include<geGL/TextureObject.h>
#include<geGL/FramebufferObject.h>

#include"shaderdir.hpp"

#define glError() {\
	GLenum err=glGetError();\
	while(err!=GL_NO_ERROR){\
		fprintf(stderr,"glError: %s caught at %s:%u\n", (char *)gluErrorString(err), __FILE__, __LINE__); \
		err=glGetError();\
	}\
}


typedef struct{
	//GLuint FBO;//framebuffer
	//GLuint RBO_Depth;//render buffer depth
	GLuint RBO_Stencil;//render buffer stencil buffer
  ge::gl::TextureObject*position;
  ge::gl::TextureObject*color;
  ge::gl::TextureObject*normal;
  ge::gl::TextureObject*depth;
  ge::gl::TextureObject*stencil;
  ge::gl::TextureObject*imageAtomicAdd;
  ge::gl::FramebufferObject*fbo;
	//GLuint Color;//color texture
  //GLuint Position;//position texture
	//GLuint Normal;//normal texture
	//GLuint Depth;
	//GLuint Stencil;
	//GLuint ImageAtomicAdd;
	ge::gl::ProgramObject*Create;
	unsigned Size[2];//size of screen
}SDeferred;

void deferred_Init(SDeferred*Deferred,unsigned Widht,unsigned Height);
void deferred_Free(SDeferred*Deferred);

void deferred_EnableFBO(SDeferred*D);
void deferred_DisableFBO(SDeferred*D);
void deferred_EnableFBOStencil(SDeferred*D);
void deferred_BlitStencilBuffer(SDeferred*D);
void deferred_BlitDepthToDefault(SDeferred*D);


void deferred_DrawQuad(SDeferred*D,
		unsigned*X,unsigned*S);


void deferred_StartCreateAtomic(SDeferred*D);
void deferred_EndCreateAtomic(SDeferred*D);
void deferred_ClearStencil(SDeferred*D);

void deferred_SetTextures(SDeferred*D);

void deferred_StartCreateFrameBuffer(SDeferred*D);
void deferred_EndCreateFrameBuffer(SDeferred*D);

void deferred_StartCreateStencil(SDeferred*D);
void deferred_EndCreateStencil(SDeferred*D);


#endif//_DEFERRED_HPP_
