#include"CShadowMapping.hpp"

#include"../../app/deferred.hpp"
#include"../ShadowMapping/createsm.h"
#include"../../app/fastAdjacency.h"
/*
#undef CLASSNAME
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
  GETPOINTER(_createShadowMapFBO),
DEFUPDATEROUTINESEND

DEFUPDATEROUTINEIDSTART
  COMPUTEMATRICES   ,
  CREATESHADOWMAP   ,
  CREATESHADOWMAPFBO,
DEFUPDATEROUTINEIDEND

DEFVAR2UPDATESTART
  FOCUSPOINT,getMask(COMPUTEMATRICES   ),
  LIGHT     ,getMask(COMPUTEMATRICES   ),
  FOVY      ,getMask(COMPUTEMATRICES   ),
  NEAR      ,getMask(COMPUTEMATRICES   ),
  FAR       ,getMask(COMPUTEMATRICES   ),
  RESOLUTION,getMask(CREATESHADOWMAP   ),
  SHADOWMASK,getMask(CREATESHADOWMAPFBO),
DEFVAR2UPDATEEND

DEFUPDATE
*/
/*
#define CLASS_NAME CShadowMapping
#define BASECLASS_NAME SimulationObject
DEF_VARSANDROUTINES(
    "shadowMapMethods.focusPoint",&CShadowMapping::_computeMatrices,
    "light"                      ,&CShadowMapping::_computeMatrices,
    "shadowMapMethods.fovy"      ,&CShadowMapping::_computeMatrices,
    "shadowMapMethods.near"      ,&CShadowMapping::_computeMatrices,
    "shadowMapMethods.far"       ,&CShadowMapping::_computeMatrices,
    "shadowMapMethods.resolution",&CShadowMapping::_createShadowMap,
    "shadowMask"                 ,&CShadowMapping::_createShadowMapFBO);
// */


CShadowMapping::CShadowMapping(std::shared_ptr<ge::util::sim::NamespaceWithUsers>const&data):ge::util::sim::User(data){
  //this->_simulationData->registerUser(this);
  //DEF_CONSTRUCTOR;
  this->_emptyVAO  = new ge::gl::VertexArrayObject();
  this->_computeMatrices   ();
  this->_createShadowMap   ();
  this->_createShadowMapFBO();

  std::string dir=this->_sData->get<std::string>("shaderDirectory")+"methods/ShadowMapping/";
  this->_csm = new ge::gl::ProgramObject(
      dir+"createshadowmap.vp",
      dir+"createshadowmap.fp");

  this->_createShadowMask = new ge::gl::ProgramObject(
      dir+"createShadowMask.vp",
      dir+"createShadowMask.gp",
      dir+"createShadowMask.fp");
}

CShadowMapping::~CShadowMapping(){
  //this->_simulationData->unregisterUser(this);
  delete this->_csm;
  delete this->_createShadowMask;
  delete this->_shadowMap;
  delete this->_fbo;
  delete this->_shadowMaskFBO;
  delete this->_emptyVAO;
}

void CShadowMapping::update(){
  //TODO
}

void CShadowMapping::createShadowMask(GLuint mask){
  this->_shadowMaskFBO->attachTexture(GL_COLOR_ATTACHMENT0,mask);
  this->createShadowMask();
}



void CShadowMapping::_createShadowMap(){
  if(this->_shadowMap)delete this->_shadowMap;
  this->_shadowMap = createSM(this->_sData->get<unsigned>("shadowMapMethods.resolution",1024));
  if(!this->_fbo)this->_fbo=new ge::gl::FramebufferObject();
  this->_fbo->attachTexture(GL_DEPTH_ATTACHMENT,this->_shadowMap->getId());
}

glm::mat4 computeLightView(glm::vec3 light,glm::vec3 focus){
  glm::vec3 viewVector = focus-light;
  glm::vec3 up = glm::cross(viewVector,viewVector+glm::vec3(1.f));
  return glm::lookAt(light,focus,up);
}

void CShadowMapping::_computeMatrices(){
  auto lp=this->_sData->get<float[3]>("light.position");
  auto fp=this->_sData->get<float[3]>("shadowMapMethods.focusPoint");
  this->setMatrices(
      glm::perspective(
        this->_sData->get<float>("shadowMapMethods.fovy",45),
        1.f,
        this->_sData->get<float>("shadowMapMethods.near",1),
        this->_sData->get<float>("shadowMapMethods.far",1000)),
      computeLightView(glm::vec3(lp[0],lp[1],lp[2]),glm::vec3(fp[0],fp[1],fp[2])));
}

void CShadowMapping::_createShadowMapFBO(){
  if(!this->_shadowMaskFBO)this->_shadowMaskFBO=new ge::gl::FramebufferObject();
  this->_shadowMaskFBO->attachTexture(GL_COLOR_ATTACHMENT0,this->_sData->get<ge::gl::TextureObject>("shadowMask").getId());
}

void CShadowMapping::CreateShadowMap(){
  this->_computeMatrices();
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);
  glPolygonOffset(2.5,10);

  this->_fbo->bind();
  glClear(GL_DEPTH_BUFFER_BIT);
  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
  glViewport(0,0,
      this->_sData->get<unsigned>("shadowMapMethods.resolution",1024),
      this->_sData->get<unsigned>("shadowMapMethods.resolution",1024));
  glEnable(GL_POLYGON_OFFSET_FILL);

  this->_csm->use();
  this->_csm->set("v",1,GL_FALSE,(float*)glm::value_ptr(this->_lightView));
  this->_csm->set("p",1,GL_FALSE,(float*)glm::value_ptr(this->_lightProjection));
  this->_sData->get<ge::gl::VertexArrayObject>("sceneVAO").bind();
  glDrawArrays(GL_TRIANGLES,0,this->_sData->get<Adjacency>("fastAdjacency").getNofTriangles()*3);
  this->_sData->get<ge::gl::VertexArrayObject>("sceneVAO").unbind();

  glDisable(GL_POLYGON_OFFSET_FILL);
  glViewport(0,0,
      this->_sData->get<unsigned[]>("window.size")[0],
      this->_sData->get<unsigned[]>("window.size")[1]);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  this->_fbo->unbind();
}

void CShadowMapping::createShadowMask(){
  //GETGPUGAUGE(MEASURE_CREATESHADOWMAP)->begin();
  this->CreateShadowMap();
  //GETGPUGAUGE(MEASURE_CREATESHADOWMAP)->end();
  
  //GETGPUGAUGE(MEASURE_CREATESHADOWMASK)->begin();
  this->_createShadowMask->use();
  this->_createShadowMask->set("BPV",1,GL_FALSE,(const float*)glm::value_ptr(this->_bpv));
  this->_sData->get<Deferred>("gbuffer").position->bind(0);
  this->_shadowMap->bind(1);
  this->_shadowMaskFBO->bind();
  this->_emptyVAO->bind();
  glDrawArrays(GL_POINTS,0,1);
  this->_emptyVAO->unbind();
  this->_shadowMaskFBO->unbind();
  //GETGPUGAUGE(MEASURE_CREATESHADOWMASK)->end();
}

void CShadowMapping::setMatrices(glm::mat4 lp,glm::mat4 lv){
  this->_lightView       = lv;
  this->_lightProjection = lp;
  this->_bpv=biasMatrix()*this->_lightProjection*this->_lightView;
}

ge::gl::TextureObject*CShadowMapping::getShadowMap(){
  return this->_shadowMap;
}

/*
CreateProgramObject::CreateProgramObject(unsigned stages,std::shared_ptr<ge::core::Accessor>const&program):Function(stages){
}

void CreateProgramObject::operator()(){
  this->beginOperator();
  this->endOperator();
}
*/

/*
ShadowMapping::ShadowMapping(std::shared_ptr<ge::core::Accessor>shadowMask):Function(3){
  this->_output = shadowMask;
}

void ShadowMapping::operator()(){
  this->resolveInputs();

}
*/

