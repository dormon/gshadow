#include"init_shadowmapping.hpp"

CShadowMap::CShadowMap(unsigned ShadowMapSize){
	this->ShadowMapSize=ShadowMapSize;
	float ones[]={1,1,1,1};
	glGenTextures(1,&this->ShadowMap);
	glBindTexture(GL_TEXTURE_2D,this->ShadowMap);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_FUNC,GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,GL_COMPARE_R_TO_TEXTURE);
	glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,ones);
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT24,
			this->ShadowMapSize,this->ShadowMapSize,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);

	this->CreateShadowMap = new ge::gl::ProgramObject(
			ShaderDir+"app/createshadowmap.vp",
			ShaderDir+"app/createshadowmap.fp");

	glGenFramebuffers(1,&this->FBOShadowMap);
}

GLuint InitShadowMap(unsigned a){
	GLuint ShadowMap;
	float ones[]={1,1,1,1};
	glGenTextures(1,&ShadowMap);
	glBindTexture(GL_TEXTURE_2D,ShadowMap);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_FUNC,GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,GL_COMPARE_R_TO_TEXTURE);
	glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,ones);
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT24,
			a,a,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
	return ShadowMap;
}

void CShadowMap::Begin(float*M,float*V,float*P){
	glBindFramebuffer(GL_FRAMEBUFFER,this->FBOShadowMap);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,this->ShadowMap,0);
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	glViewport(0,0,this->ShadowMapSize,this->ShadowMapSize);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.5,10);//abychom zabranili pigmentaci

	this->CreateShadowMap->use();
	this->CreateShadowMap->set("m",1,GL_FALSE,(const float*)M);
	this->CreateShadowMap->set("v",1,GL_FALSE,(const float*)V);
	this->CreateShadowMap->set("p",1,GL_FALSE,(const float*)P);

}

void CShadowMap::End(unsigned Width,unsigned Height){
	glDisable(GL_POLYGON_OFFSET_FILL);
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glViewport(0,0,Width,Height);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}
