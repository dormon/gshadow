#include"dormonShadowTest0.h"

/*
#define CLASSNAME DormonShadowTest0
#include"../ShadowMethodMacro.h"

DEFVARSSTART
  "shaderDirectory",
  "fastAdjacency",
  "light",
  "camera",
  "shadowMask",
  "gbuffer.fbo",
  "gbuffer.stencil",
  "computeMethod.program.WORKGROUPSIZE",
  "computeMethod.program.CULL_SIDE",
  "measure.computeGeometry.computeSides",
  "measure.computeGeometry.draw",
  "measure.computeGeometry.blit"
DEFVARSEND

DEFVARSIDSTART
  SHADERDIRECTORY=0,
  FASTADJACENCY,
  LIGHT,
  CAMERA,
  SHADOWMASK,
  GBUFFER_FBO,
  GBUFFER_STENCIL,
  WORKGROUPSIZE,
  CULL_SIDE,
  MEASURE_COMPUTESIDES,
  MEASURE_DRAW,
  MEASURE_BLIT
DEFVARSIDEND

DEFGETNOFDEP
DEFGETDEP

DEFUPDATEROUTINESSTART
  GETPOINTER(updateData),
  GETPOINTER(updateProgram)
DEFUPDATEROUTINESEND

DEFUPDATEROUTINEIDSTART
  DATA=0,
  PROGRAM
DEFUPDATEROUTINEIDEND

DEFVAR2UPDATESTART
  FASTADJACENCY,SETBIT(DATA),
  WORKGROUPSIZE,SETBIT(PROGRAM),
  CULL_SIDE,SETBIT(PROGRAM)
DEFVAR2UPDATEEND

DEFUPDATE

DormonShadowTest0::DormonShadowTest0(simulation::SimulationData*data):simulation::SimulationObject(data){
  this->_simulationData->registerUser(this);
  this->_caps    = NULL;
  this->_sides   = NULL;
  this->_maskFBO = NULL;
  this->_caps  = new CGeometryCapsAlt(GETFASTADJACENCY);
  this->_sides = new CComputeSides(GETFASTADJACENCY,GETUINT(WORKGROUPSIZE),GETBOOL(CULL_SIDE));
  this->_maskFBO = new ge::gl::FramebufferObject();
  this->_maskFBO->attachColorTexture(GL_COLOR_ATTACHMENT0,GETTEXTURE(SHADOWMASK)->getId());
  this->_maskFBO->attachStencilTexture(GETTEXTURE(GBUFFER_STENCIL)->getId());
  this->_blit = new ge::gl::ProgramObject(
      GETSTRING(SHADERDIRECTORY)+"methods/Stencil/blit.vp",
      GETSTRING(SHADERDIRECTORY)+"methods/Stencil/blit.gp",
      GETSTRING(SHADERDIRECTORY)+"methods/Stencil/blit.fp");
  this->_emptyVAO = new ge::gl::VertexArrayObject();
}

DormonShadowTest0::~DormonShadowTest0(){
  this->_simulationData->unregisterUser(this);
  delete this->_sides;
  delete this->_caps;
  delete this->_maskFBO;
  delete this->_blit;
  delete this->_emptyVAO;
}

void DormonShadowTest0::updateData(){
  if(this->_sides)delete this->_sides;
  if(this->_caps)delete this->_caps;
  this->_caps  = new CGeometryCapsAlt(GETFASTADJACENCY);
  this->_sides = new CComputeSides(GETFASTADJACENCY,GETUINT(WORKGROUPSIZE),GETBOOL(CULL_SIDE));
}
void DormonShadowTest0::updateProgram(){
  if(this->_sides)delete this->_sides;
  this->_sides = new CComputeSides(GETFASTADJACENCY,GETUINT(WORKGROUPSIZE),GETBOOL(CULL_SIDE));
}

void DormonShadowTest0::createShadowMask(){
  ge::util::CameraObject*cam=GETCAMERA;
  glm::mat4 mvp=cam->getProjection()*cam->getView();
  ge::gl::FramebufferObject*fbo=(ge::gl::FramebufferObject*)GETOBJECT(GBUFFER_FBO);

  GETGPUGAUGE(MEASURE_COMPUTESIDES)->begin();
  this->_sides->ComputeSides(glm::value_ptr(mvp),GETLIGHT);
  GETGPUGAUGE(MEASURE_COMPUTESIDES)->end();

  GETGPUGAUGE(MEASURE_DRAW)->begin();
  glClearTexImage(
    GETTEXTURE(SHADOWMASK)->getId(),
    0,GL_RED,GL_FLOAT,NULL);
  fbo->bind();
  glEnable(GL_STENCIL_TEST);
  glClear(GL_STENCIL_BUFFER_BIT);
  glStencilFunc(GL_ALWAYS,0,0);
  glStencilOpSeparate(GL_FRONT,GL_KEEP,GL_INCR_WRAP,GL_KEEP);
  glStencilOpSeparate(GL_BACK,GL_KEEP,GL_DECR_WRAP,GL_KEEP);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_FALSE);
  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
  this->_sides->DrawSides(glm::value_ptr(mvp),GETLIGHT);
  this->_caps ->DrawCaps (glm::value_ptr(mvp),GETLIGHT);
  fbo->unbind();
  GETGPUGAUGE(MEASURE_DRAW)->end();

  GETGPUGAUGE(MEASURE_BLIT)->begin();
  this->_maskFBO->bind();
  glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
  glStencilFunc(GL_EQUAL,0,0xff);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glDepthFunc(GL_ALWAYS);
  glDepthMask(GL_FALSE);
  this->_blit->use();
  this->_emptyVAO->bind();
  glDrawArrays(GL_POINTS,0,1);
  this->_emptyVAO->unbind();
  this->_maskFBO->unbind();
  glDisable(GL_STENCIL_TEST);
  GETGPUGAUGE(MEASURE_BLIT)->end();
}

void DormonShadowTest0::createShadowMask(GLuint mask){
  this->_maskFBO->attachColorTexture(GL_COLOR_ATTACHMENT0,mask);
  this->createShadowMask();
}
*/

#include"../../app/deferred.hpp"

#include"../ComputeSides/AdjacencyToVBO.h"

void DormonShadowTest0::update(){
  //TODO
}

DormonShadowTest0::DormonShadowTest0(std::shared_ptr<ge::util::sim::NamespaceWithUsers>const&data):User(data){
  auto shaderRoot = data->get<std::string>("shaderDirectory")+"methods/DormonShadowTest0/";

  this->_sidesProgram = std::make_shared<ge::gl::ProgramObject>(
      shaderRoot+"sides.comp",
      ge::gl::ShaderObject::define("MAX_MULTIPLICITY",(int)data->get<Adjacency>("fastAdjacency").getMaxMultiplicity())+
      "");

  this->_fillProgram = std::make_shared<ge::gl::ProgramObject>(
      shaderRoot+"fill.comp");

  ge::gl::BufferObject*b;
  AdjacencyToVBO(&b,&data->get<Adjacency>("fastAdjacency"))();
  this->_edges=std::shared_ptr<ge::gl::BufferObject>(b);
  this->_contour = std::make_shared<ge::gl::TextureObject>(GL_TEXTURE_2D,GL_R32I,1,data->get<unsigned[2]>("window.size")[0],data->get<unsigned[2]>("window.size")[1]);


  //data->registerUser(this,)
  //TODO all variables
  this->_caps    = NULL;
  this->_sides   = NULL;
  this->_maskFBO = nullptr;
  this->_caps  = new CGeometryCapsAlt(&data->get<Adjacency>("fastAdjacency"));
  this->_sides = new CComputeSides(&data->get<Adjacency>("fastAdjacency"),
      data->get<unsigned>("computeMethod.program.WORKGROUPSIZE"),data->get<bool>("computeMethod.program.CULL_SIDE"));
  this->_maskFBO = std::make_shared<ge::gl::FramebufferObject>();
  this->_maskFBO->attachTexture(GL_COLOR_ATTACHMENT0 ,data->get<ge::gl::TextureObject>("shadowMask").getId());
  this->_maskFBO->attachTexture(GL_STENCIL_ATTACHMENT,data->get<Deferred>("gbuffer").depth->getId());
  this->_blit = new ge::gl::ProgramObject(
      shaderRoot+"blit.vp",
      shaderRoot+"blit.gp",
      shaderRoot+"blit.fp");
  this->_emptyVAO = new ge::gl::VertexArrayObject();
}

DormonShadowTest0::~DormonShadowTest0(){
  ___;
  this->_sidesProgram = nullptr;
  this->_capsProgram = nullptr;
  ___;
  this->_fillProgram = nullptr;
  ___;
  this->_edges = nullptr;
  ___;
  this->_contour = nullptr;
  ___;
  //this->_simulationData->unregisterUser(this);
  //data
  delete this->_sides;
  delete this->_caps;
  this->_maskFBO = nullptr;
  delete this->_blit;
  delete this->_emptyVAO;
  ___;
}

void DormonShadowTest0::updateData(){
  if(this->_sides)delete this->_sides;
  if(this->_caps)delete this->_caps;
  this->_caps  = new CGeometryCapsAlt(&this->_sData->get<Adjacency>("fastAdjacency"));
  this->_sides = new CComputeSides(
      &this->_sData->get<Adjacency>("fastAdjacency"),
      this->_sData->get<unsigned>("computeMethod.program.WORKGROUPSIZE"),
      this->_sData->get<bool>("computeMethod.program.CULL_SIDE"));
}

void DormonShadowTest0::updateProgram(){
  if(this->_sides)delete this->_sides;
  this->_sides = new CComputeSides(
      &this->_sData->get<Adjacency>("fastAdjacency"),
      this->_sData->get<unsigned>("computeMethod.program.WORKGROUPSIZE"),
      this->_sData->get<bool>("computeMethod.program.CULL_SIDE"));
}

void DormonShadowTest0::createShadowMask(){
  //glFinish();
  //glMemoryBarrier(GL_ALL_BARRIER_BITS);
  glMemoryBarrier(GL_FRAMEBUFFER_BARRIER_BIT);
  glm::mat4 mvp=this->_sData->get<ge::util::CameraObject>("camera").getProjection()*this->_sData->get<ge::util::CameraObject>("camera").getView();
  this->_sidesProgram->use();
  //this->_sidesProgram->bindImage("viewSamples",this->_sData->get<Deferred>("gbuffer").position);
  //this->_sidesProgram->bindImage("contour"    ,this->_contour);
  this->_sData->get<Deferred>("gbuffer").position->bindImage(0,0);
  this->_contour->bindImage(1,0);
  this->_sidesProgram->bindSSBO ("edges"      ,&*this->_edges);
  this->_sidesProgram->set("mvp",1,GL_FALSE,glm::value_ptr(mvp));
  this->_sidesProgram->set("numEdges",this->_sData->get<Adjacency>("fastAdjacency").getNofEdges());
  this->_sidesProgram->set("lightPosition",1,this->_sData->get<float[4]>("light.position"));
  this->_sidesProgram->set("windowSize",1,this->_sData->get<unsigned[2]>("window.size"));
  this->_sidesProgram->set("noif",this->_sData->get<bool>("dormonShadowTest0.program.SIDE.noif"));
  
  glDispatchCompute(
      ge::core::getDispatchSize(this->_sData->get<unsigned[2]>("window.size")[0],8),
      ge::core::getDispatchSize(this->_sData->get<unsigned[2]>("window.size")[1],8),
      1);

  //glFinish();
  glClearTexImage(this->_sData->get<ge::gl::TextureObject>("shadowMask").getId(),0,GL_RED,GL_FLOAT,NULL);
  //glFinish();
  this->_fillProgram->use();
  //this->_fillProgram->bindImage("contour",this->_contour);
  //this->_fillProgram->bindImage("shadowMask",&this->_sData->get<ge::gl::TextureObject>("shadowMask"));
  this->_contour->bindImage(0,0);
  this->_sData->get<ge::gl::TextureObject>("shadowMask").bindImage(1,0);
  this->_fillProgram->set("windowSize",1,this->_sData->get<unsigned[2]>("window.size"));
  this->_fillProgram->set("fill",this->_sData->get<bool>("dormonShadowTest0.program.FILL.fill"));

  glDispatchCompute(
      ge::core::getDispatchSize(this->_sData->get<unsigned[2]>("window.size")[1],64),
      1,
      1);

}

void DormonShadowTest0::createShadowMask(GLuint mask){
  this->_maskFBO->attachTexture(GL_COLOR_ATTACHMENT0,mask);
  this->createShadowMask();
}

