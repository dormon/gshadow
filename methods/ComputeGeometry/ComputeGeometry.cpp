#include"ComputeGeometry.h"

#define CLASSNAME ComputeGeometry
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

ComputeGeometry::ComputeGeometry(simulation::SimulationData*data):simulation::SimulationObject(data){
  ___;
  this->_simulationData->registerUser(this);
  this->_caps    = NULL;
  this->_sides   = NULL;
  this->_maskFBO = NULL;
  ___;
  this->_caps  = new CGeometryCapsAlt(GETFASTADJACENCY);
  ___;
  this->_sides = new CComputeSides(GETFASTADJACENCY,GETUINT(WORKGROUPSIZE),GETBOOL(CULL_SIDE));
  ___;
  this->_maskFBO = new ge::gl::FramebufferObject();
  ___;
  this->_maskFBO->attachColorTexture(GL_COLOR_ATTACHMENT0,GETTEXTURE(SHADOWMASK)->getId());
  ___;
  this->_maskFBO->attachStencilTexture(GETTEXTURE(GBUFFER_STENCIL)->getId());
  this->_blit = new ge::gl::ProgramObject(
      GETSTRING(SHADERDIRECTORY)+"methods/Stencil/blit.vp",
      GETSTRING(SHADERDIRECTORY)+"methods/Stencil/blit.gp",
      GETSTRING(SHADERDIRECTORY)+"methods/Stencil/blit.fp");
  this->_emptyVAO = new ge::gl::VertexArrayObject();
}

ComputeGeometry::~ComputeGeometry(){
  this->_simulationData->unregisterUser(this);
  delete this->_sides;
  delete this->_caps;
  delete this->_maskFBO;
  delete this->_blit;
  delete this->_emptyVAO;
}

void ComputeGeometry::updateData(){
  if(this->_sides)delete this->_sides;
  if(this->_caps)delete this->_caps;
  this->_caps  = new CGeometryCapsAlt(GETFASTADJACENCY);
  this->_sides = new CComputeSides(GETFASTADJACENCY,GETUINT(WORKGROUPSIZE),GETBOOL(CULL_SIDE));
}
void ComputeGeometry::updateProgram(){
  if(this->_sides)delete this->_sides;
  this->_sides = new CComputeSides(GETFASTADJACENCY,GETUINT(WORKGROUPSIZE),GETBOOL(CULL_SIDE));
}

void ComputeGeometry::createShadowMask(){
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

void ComputeGeometry::createShadowMask(GLuint mask){
  this->_maskFBO->attachColorTexture(GL_COLOR_ATTACHMENT0,mask);
  this->createShadowMask();
}


