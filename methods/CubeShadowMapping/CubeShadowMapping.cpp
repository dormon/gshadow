#include"CubeShadowMapping.h"

#include<geCore/dtemplates.h>

#undef ___
#define ___

#include"../ShadowMapping/createsm.h"

#undef CLASSNAME
#define CLASSNAME CubeShadowMapping
#include"../ShadowMethodMacro.h"

DEFVARSSTART
  "shaderDirectory",
  "sceneVAO",
  "shadowMapMethods.resolution",
  "shadowMapMethods.focusPoint",
  "shadowMapMethods.fovy",
  "shadowMapMethods.near",
  "shadowMapMethods.far",
  "window.size",
  "fastAdjacency",
  "light",
  "gbuffer.position",
  "shadowMask",
  "measure.shadowMap.createShadowMap",
  "measure.shadowMap.createShadowMask"
DEFVARSEND

DEFVARSIDSTART
  SHADERDIRECTORY=0,
  SCENEVAO,
  RESOLUTION,
  FOCUSPOINT,
  FOVY,
  NEAR,
  FAR,
  WINDOWSIZE,
  FASTADJACENCY,
  LIGHT,
  GBUFFER_POSITION,
  SHADOWMASK,
  MEASURE_CREATESHADOWMAP,
  MEASURE_CREATESHADOWMASK
DEFVARSIDEND

DEFGETNOFDEP
DEFGETDEP

DEFUPDATEROUTINESSTART
  GETPOINTER(_computeMatrices   ),
  GETPOINTER(_createShadowMap   ),
  GETPOINTER(_createShadowMaskFBO),
DEFUPDATEROUTINESEND

DEFUPDATEROUTINEIDSTART
  COMPUTEMATRICES   ,
  CREATESHADOWMAP   ,
  CREATESHADOWMASKFBO,
DEFUPDATEROUTINEIDEND

DEFVAR2UPDATESTART
  FOCUSPOINT,getMask(COMPUTEMATRICES   ),
  LIGHT     ,getMask(COMPUTEMATRICES   ),
  FOVY      ,getMask(COMPUTEMATRICES   ),
  NEAR      ,getMask(COMPUTEMATRICES   ),
  FAR       ,getMask(COMPUTEMATRICES   ),
  RESOLUTION,getMask(CREATESHADOWMAP   ),
  SHADOWMASK,getMask(CREATESHADOWMASKFBO),
DEFVAR2UPDATEEND

DEFUPDATE

void CubeShadowMapping::_createShadowMaskFBO(){
  if(!this->_shadowMaskFBO)this->_shadowMaskFBO=new ge::gl::FramebufferObject();
  this->_shadowMaskFBO->attachColorTexture(GL_COLOR_ATTACHMENT0,GETTEXTURE(SHADOWMASK)->getId());
}



void CubeShadowMapping::_computeMatrices(){
  //table 8.19
  //{x,y,z}
  const float axes[]={
    +0,+0,+1, +0,-1,+0, +1,+0,+0,
    +0,+0,-1, +0,-1,+0, -1,+0,+0,
    +1,+0,+0, +0,+0,+1, +0,-1,+0,
    +1,+0,+0, +0,+0,-1, +0,+1,+0,
    -1,+0,+0, +0,-1,+0, +0,+0,+1,
    +1,+0,+0, +0,-1,+0, +0,+0,-1, 
  };

  this->_lightProjection=glm::perspective(glm::pi<float>()/2.f,1.f,GETFLOAT(NEAR),GETFLOAT(FAR));
  for(unsigned side=0;side<6u;++side){
    this->_lightView[side]=glm::mat4(1.f);
    for(unsigned axis=0;axis<3;++axis)
      for(unsigned k=0;k<3;++k)
        this->_lightView[side][axis][k]=axes[(side*3+axis)*3+k];
    this->_bpv[side]=biasMatrix()*this->_lightProjection*this->_lightView[side];
  }
}


CubeShadowMapping::CubeShadowMapping(simulation::SimulationData*data):simulation::SimulationObject(data){
  this->_simulationData->registerUser(this);
  this->_emptyVAO      = new ge::gl::VertexArrayObject();
  
  std::string dir=GETSTRING(SHADERDIRECTORY)+"methods/CubeShadowMapping/";
  this->_csm = new ge::gl::ProgramObject(
      dir+"createShadowMap.vp",
      dir+"createShadowMap.fp");
  this->_createShadowMask   = new ge::gl::ProgramObject(
      dir+"createShadowMask.vp",
      dir+"createShadowMask.gp",
      dir+"createShadowMask.fp");

  this->_createShadowMap();
  this->_computeMatrices();
  this->_createShadowMaskFBO();
}

CubeShadowMapping::~CubeShadowMapping(){
  delete this->_csm;
  delete this->_createShadowMask;
  delete this->_shadowMap;
  for(unsigned side=0;side<6;++side)delete this->_fbo[side];
}

void CubeShadowMapping::_createShadowMap(){
  if(this->_shadowMap)delete this->_shadowMap;
  this->_shadowMap = new ge::gl::TextureObject(
      GL_TEXTURE_CUBE_MAP,
      GL_DEPTH_COMPONENT24,
      1,
      GETUINT(RESOLUTION),
      GETUINT(RESOLUTION));
  float ones[]={1,1,1,1};
  this->_shadowMap->texParameteri (GL_TEXTURE_MIN_FILTER  ,GL_NEAREST             );
  this->_shadowMap->texParameteri (GL_TEXTURE_MAG_FILTER  ,GL_NEAREST             );
  this->_shadowMap->texParameteri (GL_TEXTURE_WRAP_S      ,GL_CLAMP_TO_BORDER     );
  this->_shadowMap->texParameteri (GL_TEXTURE_WRAP_T      ,GL_CLAMP_TO_BORDER     );
  this->_shadowMap->texParameteri (GL_TEXTURE_COMPARE_FUNC,GL_LEQUAL              );
  this->_shadowMap->texParameteri (GL_TEXTURE_COMPARE_MODE,GL_COMPARE_R_TO_TEXTURE);
  this->_shadowMap->texParameterfv(GL_TEXTURE_BORDER_COLOR,ones                   );

  GLenum cubeMapSides[]={
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z
  };
  for(unsigned side=0;side<6u;++side){
    if(!this->_fbo[side])this->_fbo[side]=new ge::gl::FramebufferObject();
    //TODO po oprave gpu engine predelat
    this->_fbo[side]->bind();
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        cubeMapSides[side],
        this->_shadowMap->getId(),
        0);
    this->_fbo[side]->unbind();
  }
}

void CubeShadowMapping::createShadowMap(){
  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
  glViewport(0,0,GETUINT(RESOLUTION),GETUINT(RESOLUTION));
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2.5,10);

  this->_csm->use();
  glm::mat4 modelMatrix=glm::translate(glm::mat4(1.0f),-glm::vec3(GETLIGHT->position));
  this->_csm->set("m",1,GL_FALSE,(const float*)glm::value_ptr(modelMatrix));
  this->_csm->set("p",1,GL_FALSE,glm::value_ptr(this->_lightProjection));
  for(unsigned side=0;side<6;++side){
    this->_csm->set("v",1,GL_FALSE,glm::value_ptr(this->_lightView[side]));
    this->_fbo[side]->bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    GETVAO(SCENEVAO)->bind();
    glDrawArrays(GL_TRIANGLES,0,GETFASTADJACENCY->getNofTriangles()*3);
    GETVAO(SCENEVAO)->unbind();
    this->_fbo[side]->unbind();
  }
  glViewport(0,0,GETUVEC2(WINDOWSIZE).x,GETUVEC2(WINDOWSIZE).y);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glDisable(GL_POLYGON_OFFSET_FILL);
}

void CubeShadowMapping::createShadowMask(GLuint mask){
  this->_shadowMaskFBO->attachColorTexture(GL_COLOR_ATTACHMENT0,mask);
  this->createShadowMask();
}

void CubeShadowMapping::createShadowMask(){
  GETGPUGAUGE(MEASURE_CREATESHADOWMAP)->begin();
  this->createShadowMap();
  GETGPUGAUGE(MEASURE_CREATESHADOWMAP)->end();

  GETGPUGAUGE(MEASURE_CREATESHADOWMASK)->begin();
  this->_createShadowMask->use();
  this->_createShadowMask->set("near",GETFLOAT(NEAR));
  this->_createShadowMask->set("far",GETFLOAT(FAR));
  this->_createShadowMask->set("lightPosition",1,glm::value_ptr(GETLIGHT->position));
  GETTEXTURE(GBUFFER_POSITION)->bind(GL_TEXTURE0);
  this->_shadowMap->bind(GL_TEXTURE1);
  this->_shadowMaskFBO->bind();
  this->_emptyVAO->bind();
  glDrawArrays(GL_POINTS,0,1);
  this->_emptyVAO->unbind();
  this->_shadowMaskFBO->unbind();
  GETGPUGAUGE(MEASURE_CREATESHADOWMASK)->end();
}
