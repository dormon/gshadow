#include"RTW.h"

#include<geUtil/CameraObject.h>
#include"../ShadowMapping/createsm.h"

#define CLASSNAME RTWBack
#include"../ShadowMethodMacro.h"

DEFVARSSTART
  "shaderDirectory",
  "sceneVAO",
  "shadowMapMethods.resolution",
  "shadowMapMethods.focusPoint",
  "shadowMapMethods.fovy",
  "shadowMapMethods.near",
  "shadowMapMethods.far",
  "shadowMapMethods.offset.factor",
  "shadowMapMethods.offset.units",
  "window.size",
  "fastAdjacency",
  "light",
  "camera",
  "gbuffer.position",
  "shadowMask",
  "rtw.program.CIM.WORKGROUP_SIZE_X",
  "rtw.program.CIM.WORKGROUP_SIZE_Y",
  "rtw.program.CIM1D.WORKGROUP_SIZE_X",
  "rtw.program.CIM1D.WALKING_WINDOW_SIZE",
  "rtw.program.CSM.TESS_FACTOR",
  "rtw.importance_passes",
  "rtw.drawLinesToSM",
  "rtw.maxDistance",
  "measure.rtw.createImportance",
  "measure.rtw.createShadowMap",
  "measure.rtw.createShadowMask",
  "measure.rtw.importance2D",
  "measure.rtw.importance1D",
  "measure.rtw.smooth",
  "measure.rtw.sum",
  "measure.rtw.whole"
DEFVARSEND

DEFVARSIDSTART
  SHADERDIRECTORY=0,
  SCENEVAO,
  RESOLUTION,
  FOCUSPOINT,
  FOVY,
  NEAR,
  FAR,
  FACTOR,
  UNITS,
  WINDOWSIZE,
  FASTADJACENCY,
  LIGHT,
  CAMERA,
  GBUFFER_POSITION,
  SHADOWMASK,
  CIM_SIZE_X,
  CIM_SIZE_Y,
  CIM1D_SIZE_X,
  CIM1D_WINDOW_SIZE,
  TESS_FACTOR,
  PASSES,
  LINE_TO_SM,
  MAX_DISTANCE,
  MEASURE_CREATEIMPORTANCE,
  MEASURE_CREATESHADOWMAP,
  MEASURE_CREATESHADOWMASK,
  MEASURE_IMPORTANCE2D,
  MEASURE_IMPORTANCE1D,
  MEASURE_SMOOTH,
  MEASURE_SUM,
  MEASURE_WHOLE
DEFVARSIDEND

DEFGETNOFDEP
DEFGETDEP
/*
DEFUPDATEROUTINESSTART
  GETPOINTER(_computeMatrices),
  GETPOINTER(_setWindowSize),
  GETPOINTER(_createShadowMap),
  GETPOINTER(_createShadowMapFBO),
  GETPOINTER(_setMeasureCreateShadowMask)
DEFUPDATEROUTINESEND

DEFUPDATEROUTINEIDSTART
  COMPUTEMATRICES=0,
  SETWINDOWSIZE,
  CREATESHADOWMAP,
  CREATESHADOWMAPFBO,
  SETMEASURECREATESHADOWMASK
DEFUPDATEROUTINEIDEND

DEFVAR2UPDATESTART
  FOCUSPOINT,SETBIT(COMPUTEMATRICES),
  LIGHT,SETBIT(COMPUTEMATRICES),
  FOVY,SETBIT(COMPUTEMATRICES),
  NEAR,SETBIT(COMPUTEMATRICES),
  FAR,SETBIT(COMPUTEMATRICES),
  WINDOWSIZE,SETBIT(SETWINDOWSIZE),

  WORKGROUPSIZE,SETBIT(PROGRAM),
  CULL_SIDE,SETBIT(PROGRAM)
DEFVAR2UPDATEEND


DEFUPDATE
*/


void RTWBack::update(){
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

void RTWBack::_createRTWMask(){
  this->_createRTWMaskProgram->use();
  glm::mat4 mvp = this->_lightProjection*this->_lightView;
  this->_createRTWMaskProgram->set("mvp",1,GL_FALSE,(const float*)glm::value_ptr(mvp));

  this->_createRTWMaskProgram->set("shadowMapSize",GETUINT(RESOLUTION));
  this->_sumX->bind(GL_TEXTURE0);
  this->_sumY->bind(GL_TEXTURE1);
  GETTEXTURE(GBUFFER_POSITION)->bind(GL_TEXTURE2);
  this->_shadowMap->bind(GL_TEXTURE3);


  this->_shadowMaskFBO->bind();
  this->_emptyVAO->bind();
  glDrawArrays(GL_POINTS,0,1);
  this->_emptyVAO->unbind();
  this->_shadowMaskFBO->unbind();
}

void RTWBack::createShadowMask(){
  GETGPUGAUGE(MEASURE_WHOLE)->begin();
  {
    GETGPUGAUGE(MEASURE_CREATEIMPORTANCE)->begin();
    {
      GETGPUGAUGE(MEASURE_IMPORTANCE2D)->begin();
      this->_createImportance();
      GETGPUGAUGE(MEASURE_IMPORTANCE2D)->end();

      GETGPUGAUGE(MEASURE_IMPORTANCE1D)->begin();
      this->_createImportance1D();
      GETGPUGAUGE(MEASURE_IMPORTANCE1D)->end();

      GETGPUGAUGE(MEASURE_SMOOTH)->begin();
      this->_smoothImportance1D();
      GETGPUGAUGE(MEASURE_SMOOTH)->end();

      GETGPUGAUGE(MEASURE_SUM)->begin();
      this->_sumImportance1D();
      GETGPUGAUGE(MEASURE_SUM)->end();
    }
    GETGPUGAUGE(MEASURE_CREATEIMPORTANCE)->end();


    GETGPUGAUGE(MEASURE_CREATESHADOWMAP)->begin();
    this->_createRTWMap();
    GETGPUGAUGE(MEASURE_CREATESHADOWMAP)->end();

    GETGPUGAUGE(MEASURE_CREATESHADOWMASK)->begin();
    this->_createRTWMask();
    GETGPUGAUGE(MEASURE_CREATESHADOWMASK)->end();
  }
  GETGPUGAUGE(MEASURE_WHOLE)->end();
}

RTWBack::RTWBack(simulation::SimulationData*data):simulation::SimulationObject(data){
  this->_simulationData->registerUser(this);
  this->_shadowMap = NULL;
  this->_fbo       = NULL;
  this->_shadowMap = NULL;
  this->_fbo       = NULL;
  this->_shadowMap = NULL;
  this->_shadowMask    = NULL;
  this->_shadowMaskFBO = NULL;

  this->_emptyVAO  = new ge::gl::VertexArrayObject();

  this->_computeMatrices   ();
  this->_createShadowMap   ();
  this->_createShadowMapFBO();

  std::string dir=GETSTRING(SHADERDIRECTORY)+"methods/RTW/";

  this->_createRTWMaskProgram = new ge::gl::ProgramObject(
      dir+"createRTWMask.vp",
      dir+"createRTWMask.gp",
      dir+"createRTWMask.fp",
      ge::gl::ShaderObject::include(dir+"warpFunction.vp"));


  this->_createImportanceMap = new ge::gl::ProgramObject(
      dir+"createImportanceMap.comp",
      this->_simulationData->define("rtw.program.CIM"));

  this->_importanceMap = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_RGBA32F,1,
      GETUINT(RESOLUTION),GETUINT(RESOLUTION));

  this->_create1DImportance = new ge::gl::ProgramObject(
      dir+"create1DImportanceMap.comp",
      this->_simulationData->define("rtw.program.CIM1D"));
  this->_importanceX = new ge::gl::TextureObject(GL_TEXTURE_1D,GL_RGBA32F,1,GETUINT(RESOLUTION));
  this->_importanceY = new ge::gl::TextureObject(GL_TEXTURE_1D,GL_RGBA32F,1,GETUINT(RESOLUTION));

  this->_smoothProgram = new ge::gl::ProgramObject(
      dir+"smooth.comp",
      this->_simulationData->define("rtw.program.CIM1D"));
  this->_smoothX = new ge::gl::TextureObject(GL_TEXTURE_1D,GL_RGBA32F,1,GETUINT(RESOLUTION));
  this->_smoothY = new ge::gl::TextureObject(GL_TEXTURE_1D,GL_RGBA32F,1,GETUINT(RESOLUTION));

  this->_sumProgram = new ge::gl::ProgramObject(
      dir+"sum.comp",
      this->_simulationData->define("rtw.program.CIM1D"));
  this->_sumX = new ge::gl::TextureObject(GL_TEXTURE_1D,GL_R32F,1,GETUINT(RESOLUTION));
  this->_sumY = new ge::gl::TextureObject(GL_TEXTURE_1D,GL_R32F,1,GETUINT(RESOLUTION));

  this->_sumX->texParameteri(GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  this->_sumX->texParameteri(GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
  this->_sumX->texParameteri(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  this->_sumX->texParameteri(GL_TEXTURE_MAG_FILTER,GL_LINEAR);

  this->_sumY->texParameteri(GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  this->_sumY->texParameteri(GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
  this->_sumY->texParameteri(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  this->_sumY->texParameteri(GL_TEXTURE_MAG_FILTER,GL_LINEAR);

  this->_createRTWProgram = new ge::gl::ProgramObject(
      dir+"createRTWTS.vp",
      dir+"createRTWTS.cp",
      ge::gl::ShaderObject::include(dir+"warpFunction.vp"),
      dir+"createRTWTS.ep",
      ge::gl::ShaderObject::include(dir+"warpFunction.vp"),
      dir+"createRTWTS.fp");

  this->_drawGridProgram = new ge::gl::ProgramObject(
      dir+"drawgrid.vp",
      dir+"drawgrid.cp",
      ge::gl::ShaderObject::include(dir+"warpFunction.vp"),
      dir+"drawgrid.ep",
      ge::gl::ShaderObject::include(dir+"warpFunction.vp"),
      dir+"drawgrid.fp");
}

void RTWBack::_createShadowMap(){
  if(this->_shadowMap)delete this->_shadowMap;
  this->_shadowMap = createSM(GETUINT(RESOLUTION));
  if(!this->_fbo)this->_fbo=new ge::gl::FramebufferObject();
  this->_fbo->attachDepthTexture(this->_shadowMap->getId());
}

void RTWBack::setMatrices(glm::mat4 lp,glm::mat4 lv){
  this->_lightView       = lv;
  this->_lightProjection = lp;
  this->_bpv=
    glm::scale(glm::mat4(1.),glm::vec3(.5))*
    glm::translate(glm::mat4(1.),glm::vec3(1.))*
    this->_lightProjection*
    this->_lightView;
}

void RTWBack::_computeMatrices(){
  this->setMatrices(glm::perspective(GETFLOAT(FOVY),1.f,GETFLOAT(NEAR),GETFLOAT(FAR)),
      glm::lookAt(glm::vec3(GETLIGHT->position),GETVEC3(FOCUSPOINT),glm::vec3(0.f,1.f,0.f)));
}

void RTWBack::_createShadowMapFBO(){
  this->_shadowMask=(ge::gl::TextureObject*)this->_simulationData->getObject(VARS[SHADOWMASK],NULL);
  if(!this->_shadowMaskFBO)this->_shadowMaskFBO=new ge::gl::FramebufferObject();
  this->_shadowMaskFBO->attachColorTexture(GL_COLOR_ATTACHMENT0,this->_shadowMask->getId());
}


RTWBack::~RTWBack(){
  this->_simulationData->unregisterUser(this);
  delete this->_shadowMap;
  delete this->_fbo;
  delete this->_shadowMaskFBO;
  delete this->_emptyVAO;
  delete this->_createRTWMaskProgram;
  delete this->_createImportanceMap;
  delete this->_importanceMap;
  delete this->_create1DImportance;
  delete this->_importanceX;
  delete this->_importanceY;
  delete this->_smoothProgram;
  delete this->_smoothX;
  delete this->_smoothY;
  delete this->_sumProgram;
  delete this->_sumX;
  delete this->_sumY;
  delete this->_createRTWProgram;
  delete this->_drawGridProgram;
}

void RTWBack::_createImportance(){
  glm::uvec2 winSize=GETUVEC2(WINDOWSIZE);

  glClearTexImage(this->_importanceMap->getId(),0,GL_RGBA,GL_FLOAT,NULL);

  glm::vec3 camPos = glm::vec3(glm::inverse(GETCAMERA->getView())*glm::vec4(0,0,0,1));

  this->_createImportanceMap->use();
  this->_createImportanceMap->set("BPV",1,GL_FALSE,glm::value_ptr(this->_bpv));
  this->_createImportanceMap->set("shadowMapSize",GETUINT(RESOLUTION));
  this->_createImportanceMap->set("windowSize",winSize.x,winSize.y);
  this->_createImportanceMap->set("cameraPosition",1,glm::value_ptr(camPos));
  this->_createImportanceMap->set("maxDistance",GETFLOAT(MAX_DISTANCE));
  unsigned workSizex=winSize.x/GETUINT(CIM_SIZE_X)+1;
  unsigned workSizey=winSize.y/GETUINT(CIM_SIZE_Y)+1;

  GETTEXTURE(GBUFFER_POSITION)->bind(GL_TEXTURE0);
  this->_importanceMap->bindImage(1,0);

  for(unsigned i=0;i<GETUINT(PASSES);++i){
    glDispatchCompute(workSizex,workSizey,1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  }
}

ge::gl::TextureObject*RTWBack::getImportanceMap(){
  return this->_importanceMap;
}
ge::gl::TextureObject*RTWBack::getImportanceX(){
  return this->_importanceX;
}
ge::gl::TextureObject*RTWBack::getImportanceY(){
  return this->_importanceY;
}
ge::gl::TextureObject*RTWBack::getSmoothX(){
  return this->_smoothX;
}
ge::gl::TextureObject*RTWBack::getSmoothY(){
  return this->_smoothY;
}
ge::gl::TextureObject*RTWBack::getSumX(){
  return this->_sumX;
}
ge::gl::TextureObject*RTWBack::getSumY(){
  return this->_sumY;
}
ge::gl::TextureObject*RTWBack::getShadowMap(){
  return this->_shadowMap;
}

void RTWBack::_createImportance1D(){
  glClearTexImage(this->_importanceX->getId(),0,GL_RGBA,GL_FLOAT,NULL);
  glClearTexImage(this->_importanceY->getId(),0,GL_RGBA,GL_FLOAT,NULL);

  this->_create1DImportance->use();
  this->_create1DImportance->set("shadowMapSize",GETUINT(RESOLUTION));
  this->_importanceMap->bindImage(0,0);

  unsigned workSizex=GETUINT(RESOLUTION)/GETUINT(CIM1D_SIZE_X)+1;

  this->_create1DImportance->set("x1D",true);
  this->_importanceX->bindImage(1,0);
  glDispatchCompute(workSizex,1,1);

  this->_create1DImportance->set("x1D",false);
  this->_importanceY->bindImage(1,0);
  glDispatchCompute(workSizex,1,1);
}

void RTWBack::_smoothImportance1D(){
  glClearTexImage(this->_smoothX->getId(),0,GL_RGBA,GL_FLOAT,NULL);
  glClearTexImage(this->_smoothY->getId(),0,GL_RGBA,GL_FLOAT,NULL);

  this->_smoothProgram->use();
  this->_smoothProgram->set("shadowMapSize",GETUINT(RESOLUTION));
  this->_smoothProgram->set("windowSize",GETUINT(CIM1D_WINDOW_SIZE));

  unsigned workSizex=GETUINT(RESOLUTION)/GETUINT(CIM1D_SIZE_X)+1;

  this->_importanceX->bindImage(0,0);
  this->_smoothX    ->bindImage(1,0);
  glDispatchCompute(workSizex,1,1);

  this->_importanceY->bindImage(0,0);
  this->_smoothY    ->bindImage(1,0);
  glDispatchCompute(workSizex,1,1);
}

void RTWBack::_sumImportance1D(){
  glClearTexImage(this->_sumX->getId(),0,GL_RED,GL_FLOAT,NULL);
  glClearTexImage(this->_sumY->getId(),0,GL_RED,GL_FLOAT,NULL);

  this->_sumProgram->use();
  this->_sumProgram->set("shadowMapSize",GETUINT(RESOLUTION));

  unsigned workSizex=GETUINT(RESOLUTION)/GETUINT(CIM1D_SIZE_X)+1;

  this->_smoothX    ->bindImage(0,0);
  this->_sumX       ->bindImage(1,0);
  glDispatchCompute(workSizex,1,1);

  this->_smoothY    ->bindImage(0,0);
  this->_sumY       ->bindImage(1,0);
  glDispatchCompute(workSizex,1,1);
}

void RTWBack::_createRTWMap(){
  glm::mat4 mvp=this->_lightProjection*this->_lightView;

  this->_createRTWProgram->use();
  this->_createRTWProgram->set("mvp",1,GL_FALSE,glm::value_ptr(mvp));
  this->_createRTWProgram->set("shadowMapSize",GETUINT(RESOLUTION));
  this->_createRTWProgram->set("tessFactor",GETUINT(TESS_FACTOR));

  this->_sumX->bind(GL_TEXTURE0);
  this->_sumY->bind(GL_TEXTURE1);

  this->_fbo->bind();
  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
  glViewport(0,0,GETUINT(RESOLUTION),GETUINT(RESOLUTION));
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);
  glClear(GL_DEPTH_BUFFER_BIT);

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(GETFLOAT(FACTOR),GETFLOAT(UNITS));

  GETVAO(SCENEVAO)->bind();
  glPatchParameteri(GL_PATCH_VERTICES,3);
  if(GETBOOL(LINE_TO_SM))glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glDrawArrays(GL_PATCHES,0,GETFASTADJACENCY->getNofTriangles()*3);
  if(GETBOOL(LINE_TO_SM))glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  GETVAO(SCENEVAO)->unbind();

  glViewport(0,0,GETUVEC2(WINDOWSIZE).x,GETUVEC2(WINDOWSIZE).y);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glDisable(GL_POLYGON_OFFSET_FILL);
  this->_fbo->unbind();
}

void RTWBack::drawGrid(float x,float y,float sx,float sy){
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glViewport(
      x *GETUVEC2(WINDOWSIZE).x,
      y *GETUVEC2(WINDOWSIZE).y,
      sx*GETUVEC2(WINDOWSIZE).x,
      sy*GETUVEC2(WINDOWSIZE).y);
  this->_drawGridProgram->use();
  this->_drawGridProgram->set("shadowMapSize",GETUINT(RESOLUTION));

  this->_sumX->bind(GL_TEXTURE0);
  this->_sumY->bind(GL_TEXTURE1);
  this->_emptyVAO->bind();
  glPatchParameteri(GL_PATCH_VERTICES,1);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glDrawArrays(GL_PATCHES,0,1);
  this->_emptyVAO->unbind();
  glViewport(0,0,GETUVEC2(WINDOWSIZE).x,GETUVEC2(WINDOWSIZE).y);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}

void RTWBack::createShadowMask(GLuint mask){
  this->_shadowMaskFBO->attachColorTexture(GL_COLOR_ATTACHMENT0,mask);
  this->createShadowMask();
}
