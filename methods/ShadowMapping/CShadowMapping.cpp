#include"CShadowMapping.hpp"

#include"../ShadowMapping/createsm.h"

#define CLASSNAME CShadowMapping
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
  "adjacency",
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
  ADJACENCY,
  LIGHT,
  GBUFFER_POSITION,
  SHADOWMASK,
  MEASURE_CREATESHADOWMAP,
  MEASURE_CREATESHADOWMASK
DEFVARSIDEND

DEFGETNOFDEP
DEFGETDEP

void CShadowMapping::update(){
  if(
      this->_changed[VARS[FOCUSPOINT]]||
      this->_changed[VARS[LIGHT     ]]||
      this->_changed[VARS[FOVY      ]]||
      this->_changed[VARS[NEAR      ]]||
      this->_changed[VARS[FAR       ]]){
    this->_computeMatrices();
    this->_changed[VARS[FOCUSPOINT]]=false;
    this->_changed[VARS[LIGHT     ]]=false;
    this->_changed[VARS[FOVY      ]]=false;
    this->_changed[VARS[NEAR      ]]=false;
    this->_changed[VARS[FAR       ]]=false;
  }
  if(this->_changed[VARS[RESOLUTION]]){
    this->_createShadowMap();
    this->_changed[VARS[RESOLUTION]]=false;
  }
  if(this->_changed[VARS[SHADOWMASK]]){
    this->_createShadowMapFBO();
    this->_changed[VARS[SHADOWMASK]]=false;
  }
}

void CShadowMapping::createShadowMask(){
  GETGPUGAUGE(MEASURE_CREATESHADOWMAP)->begin();
  this->CreateShadowMap();
  GETGPUGAUGE(MEASURE_CREATESHADOWMAP)->end();

  GETGPUGAUGE(MEASURE_CREATESHADOWMASK)->begin();
  this->_createShadowMask->use();
  this->_createShadowMask->set("BPV",1,GL_FALSE,glm::value_ptr(this->_bpv));
  GETTEXTURE(GBUFFER_POSITION)->bind(GL_TEXTURE0);
  this->_shadowMap->bind(GL_TEXTURE1);
  this->_shadowMaskFBO->bind();
  this->_emptyVAO->bind();
  glDrawArrays(GL_POINTS,0,1);
  this->_emptyVAO->unbind();
  this->_shadowMaskFBO->unbind();
  GETGPUGAUGE(MEASURE_CREATESHADOWMASK)->end();
}

void CShadowMapping::createShadowMask(GLuint mask){
  this->_shadowMaskFBO->attachColorTexture(GL_COLOR_ATTACHMENT0,mask);
  this->createShadowMask();
}


CShadowMapping::CShadowMapping(simulation::SimulationData*data):simulation::SimulationObject(data){
  this->_simulationData->registerUser(this);
  this->_shadowMap = NULL;
  this->_fbo       = NULL;
  this->_csm       = NULL;
  this->_shadowMaskFBO = NULL;
  this->_emptyVAO  = new ge::gl::VertexArrayObject();
  this->_computeMatrices   ();
  this->_createShadowMap   ();
  this->_createShadowMapFBO();

  this->_csm = new ge::gl::ProgramObject(
      GETSTRING(SHADERDIRECTORY)+"methods/ShadowMapping/createshadowmap.vp",
      GETSTRING(SHADERDIRECTORY)+"methods/ShadowMapping/createshadowmap.fp");

  this->_createShadowMask = new ge::gl::ProgramObject(
      GETSTRING(SHADERDIRECTORY)+"methods/ShadowMapping/createShadowMask.vp",
      GETSTRING(SHADERDIRECTORY)+"methods/ShadowMapping/createShadowMask.gp",
      GETSTRING(SHADERDIRECTORY)+"methods/ShadowMapping/createShadowMask.fp");
}

CShadowMapping::~CShadowMapping(){
  this->_simulationData->unregisterUser(this);
  delete this->_csm;
  delete this->_createShadowMask;
  delete this->_shadowMap;
  delete this->_fbo;
  delete this->_shadowMaskFBO;
  delete this->_emptyVAO;
}

void CShadowMapping::_createShadowMap(){
  if(this->_shadowMap)delete this->_shadowMap;
  this->_shadowMap = createSM(GETUINT(RESOLUTION));
  if(!this->_fbo)this->_fbo=new ge::gl::FramebufferObject();
  this->_fbo->attachDepthTexture(this->_shadowMap->getId());
}

void CShadowMapping::_computeMatrices(){
  this->setMatrices(glm::perspective(GETFLOAT(FOVY),1.f,GETFLOAT(NEAR),GETFLOAT(FAR)),
      glm::lookAt(glm::vec3(GETLIGHT->position),GETVEC3(FOCUSPOINT),glm::vec3(0.f,1.f,0.f)));
}

void CShadowMapping::_createShadowMapFBO(){
  if(!this->_shadowMaskFBO)this->_shadowMaskFBO=new ge::gl::FramebufferObject();
  this->_shadowMaskFBO->attachColorTexture(GL_COLOR_ATTACHMENT0,GETTEXTURE(SHADOWMASK)->getId());
}

void CShadowMapping::CreateShadowMap(){
  this->_csm->use();
  this->_csm->set("v",1,GL_FALSE,(float*)glm::value_ptr(this->_lightView));
  this->_csm->set("p",1,GL_FALSE,(float*)glm::value_ptr(this->_lightProjection));

  this->_fbo->bind();
  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
  glViewport(0,0,GETUINT(RESOLUTION),GETUINT(RESOLUTION));
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);
  glClear(GL_DEPTH_BUFFER_BIT);

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2.5,10);

  GETVAO(SCENEVAO)->bind();
  glDrawArrays(GL_TRIANGLES,0,GETADJACENCY->NumTriangles*3);
  GETVAO(SCENEVAO)->unbind();

  glViewport(0,0,GETUVEC2(WINDOWSIZE).x,GETUVEC2(WINDOWSIZE).y);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glDisable(GL_POLYGON_OFFSET_FILL);
  this->_fbo->unbind();
}

void CShadowMapping::setMatrices(glm::mat4 lp,glm::mat4 lv){
  this->_lightView       = lv;
  this->_lightProjection = lp;
  this->_bpv=
    glm::scale(glm::mat4(1.),glm::vec3(.5))*
    glm::translate(glm::mat4(1.),glm::vec3(1.))*
    this->_lightProjection*
    this->_lightView;
}

ge::gl::TextureObject*CShadowMapping::getShadowMap(){
  return this->_shadowMap;
}

