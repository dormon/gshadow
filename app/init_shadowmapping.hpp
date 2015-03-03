#ifndef _INIT_SHADOWMAPPING_HPP_
#define _INIT_SHADOWMAPPING_HPP_

#include<GL/glew.h>
#include<geGL/ProgramObject.h>

#include"shaderdir.hpp"

GLuint InitShadowMap(unsigned a);

class CShadowMap
{
	public:
		GLuint ShadowMap;//shadow map
		unsigned ShadowMapSize;//size of shadow map
		GLuint FBOShadowMap;//framebuffer for shadow map creation
		ge::gl::ProgramObject*CreateShadowMap;//shader for shader map creation
		CShadowMap(unsigned ShadowMapSize);
		void Begin(float*M,float*V,float*P);
		void End(unsigned Width,unsigned Height);
};

#endif//_INIT_SHADOWMAPPING_HPP_
