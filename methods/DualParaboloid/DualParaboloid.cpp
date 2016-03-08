#include"DualParaboloid.h"

#include<geCore/dtemplates.h>

#include"../ShadowMapping/createsm.h"

#undef CLASSNAME
#define CLASSNAME DualParaboloid
#include"../ShadowMethodMacro.h"
#include"createCubeShadowMapTexture.h"

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
  "measure.shadowMap.createShadowMask",
  "dp.drawSM0",
  "dp.drawSM1"
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
  MEASURE_CREATESHADOWMASK,
  DRAWSM0,
  DRAWSM1
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

void DualParaboloid::_createShadowMaskFBO(){
  if(!this->_shadowMaskFBO)this->_shadowMaskFBO=new ge::gl::FramebufferObject();
  this->_shadowMaskFBO->attachTexture(GL_COLOR_ATTACHMENT0,GETTEXTURE(SHADOWMASK)->getId());
}



void DualParaboloid::_computeMatrices(){
  glm::vec3 lp=glm::vec3(GETLIGHT->position);
  for(unsigned i=0;i<2;++i)
    this->_lightView[i]=glm::lookAt(lp,lp+glm::vec3(-1.f+2.f*i,0.f,0.f),glm::vec3(0.f,1.f,0.f));
}


DualParaboloid::DualParaboloid(simulation::SimulationData*data):simulation::SimulationObject(data){
  this->_simulationData->registerUser(this);
  this->_emptyVAO      = new ge::gl::VertexArrayObject();
  
  std::string dir=GETSTRING(SHADERDIRECTORY)+"methods/DualParaboloid/";
  this->_csm = new ge::gl::ProgramObject(
      dir+"createShadowMap.vp",
      ge::gl::ShaderObject::include(dir+"dpProj.vp"),
      dir+"createShadowMap.fp");
  this->_createShadowMask   = new ge::gl::ProgramObject(
      dir+"createShadowMask.vp",
      dir+"createShadowMask.gp",
      dir+"createShadowMask.fp",
      ge::gl::ShaderObject::include(dir+"dpProj.vp"));

  this->_createShadowMap();
  this->_computeMatrices();
  this->_createShadowMaskFBO();
}

DualParaboloid::~DualParaboloid(){
  delete this->_csm;
  delete this->_createShadowMask;
  delete this->_shadowMap[0];
  delete this->_shadowMap[1];
}

void DualParaboloid::_createShadowMap(){
  for(unsigned i=0;i<2;++i){
    if(this->_shadowMap[i])delete this->_shadowMap[i];
    this->_shadowMap[i] = createSM(GETUINT(RESOLUTION));
  }

  for(unsigned side=0;side<2u;++side){
    if(!this->_fbo[side])this->_fbo[side]=new ge::gl::FramebufferObject();
    //TODO po oprave gpu engine predelat
    this->_fbo[side]->bind();
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        this->_shadowMap[side]->getId(),
        0);
    this->_fbo[side]->unbind();
  }
}

void DualParaboloid::createShadowMap(){
  this->_computeMatrices();
  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
  glViewport(0,0,GETUINT(RESOLUTION),GETUINT(RESOLUTION));
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2.5,10);
  glEnable(GL_CLIP_PLANE0);
  this->_csm->use();
  for(unsigned side=0;side<2;++side){
    this->_csm->set("v",1,GL_FALSE,glm::value_ptr(this->_lightView[side]));
    this->_csm->set("near",GETFLOAT(NEAR));
    this->_csm->set("far" ,GETFLOAT(FAR));
    this->_fbo[side]->bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    GETVAO(SCENEVAO)->bind();
    glDrawArrays(GL_TRIANGLES,0,GETFASTADJACENCY->getNofTriangles()*3);
    GETVAO(SCENEVAO)->unbind();
    this->_fbo[side]->unbind();
  }
  glDisable(GL_CLIP_PLANE0);
  glViewport(0,0,GETUVEC2(WINDOWSIZE).x,GETUVEC2(WINDOWSIZE).y);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glDisable(GL_POLYGON_OFFSET_FILL);
}

void DualParaboloid::createShadowMask(GLuint mask){
  this->_shadowMaskFBO->attachTexture(GL_COLOR_ATTACHMENT0,mask);
  this->createShadowMask();
}

void DualParaboloid::createShadowMask(){
  this->_computeMatrices();
  GETGPUGAUGE(MEASURE_CREATESHADOWMAP)->begin();
  this->createShadowMap();
  GETGPUGAUGE(MEASURE_CREATESHADOWMAP)->end();

  GETGPUGAUGE(MEASURE_CREATESHADOWMASK)->begin();
  this->_createShadowMask->use();
  this->_createShadowMask->set("near",GETFLOAT(NEAR));
  this->_createShadowMask->set("far",GETFLOAT(FAR));
  this->_createShadowMask->set("lightPosition",1,glm::value_ptr(GETLIGHT->position));
  float data[2*16];
  std::memcpy(data   ,glm::value_ptr(this->_lightView[0]),sizeof(float)*16);
  std::memcpy(data+16,glm::value_ptr(this->_lightView[1]),sizeof(float)*16);
  this->_createShadowMask->set("v",2,GL_FALSE,data);
  GETTEXTURE(GBUFFER_POSITION)->bind(GL_TEXTURE0);
  this->_shadowMap[0]->bind(GL_TEXTURE1);
  this->_shadowMap[1]->bind(GL_TEXTURE2);
  this->_shadowMaskFBO->bind();
  this->_emptyVAO->bind();
  glDrawArrays(GL_POINTS,0,1);
  this->_emptyVAO->unbind();
  this->_shadowMaskFBO->unbind();
  GETGPUGAUGE(MEASURE_CREATESHADOWMASK)->end();
}
