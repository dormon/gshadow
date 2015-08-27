
#include"NavyMapping.h"

#include"../../app/core.h"

#include"../../GPUPerfApi/GpuPerfApi.h"
#include"../ShadowMapping/createsm.h"

#define CLASSNAME NavyMapping
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
  "gbuffer.position",
  "shadowMask",
  "gpa",
  "nv.program.VS.WORKGROUP_SIZE_X",
  "nv.program.VS.WORKGROUP_SIZE_Y",
  "nv.program.FDV.WORKGROUP_SIZE_X",
  "nv.program.FDV.WORKGROUP_SIZE_Y",
  "nv.program.COUNTMAP.WORKGROUP_SIZE_X",
  "nv.program.COUNTMAP.WORKGROUP_SIZE_Y",
  "nv.program.INTEGRATE.WORKGROUP_SIZE_X",
  "nv.program.OFFSET.WORKGROUP_SIZE_X",
  "nv.program.OFFSET.WORKGROUP_SIZE_Y",
  "nv.program.SMOOTH.WORKGROUP_SIZE_X",
  "nv.program.SMOOTH.WORKGROUP_SIZE_Y",
  "nv.program.smoothWindowSize",
  "nv.program.INTEGRATEOFFSET.WORKGROUP_SIZE_X",
  "nv.use_fast_smooth",
  "nv.program.warpFactor",
  "nv.program.NVMAP.TESS_FACTOR",
  "nv.useWarping",
  "nv.drawLinesToSM",
  "nv.cullTriangles",
  "measure.shadowMap.createShadowMap",
  "measure.shadowMap.createShadowMask",
  "measure.nv.computeViewSamples",
  "measure.nv.dv"                ,
  "measure.nv.countmap"          ,
  "measure.nv.integratex"        ,
  "measure.nv.offsetx"           ,
  "measure.nv.smootx"            ,
  "measure.nv.unwarpx"           ,
  "measure.nv.integratey"        ,
  "measure.nv.offsety"           ,
  "measure.nv.smooty"            ,
  "measure.nv.wholewarp"         ,
  "measure.nv.whole"             ,
  "measure.nv.wholex"            ,
  "measure.nv.wholey"
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
  GBUFFER_POSITION,
  SHADOWMASK,
  GPA,
  VS_SIZE_X,
  VS_SIZE_Y,
  FDV_SIZE_X,
  FDV_SIZE_Y,
  COUNTMAP_SIZE_X,
  COUNTMAP_SIZE_Y,
  INTEGRATE_SIZE_X,
  OFFSET_SIZE_X,
  OFFSET_SIZE_Y,
  SMOOTH_SIZE_X,
  SMOOTH_SIZE_Y,
  SMOOTH_WINDOW,
  INTOFF_X,
  USE_FAST_SMOOTH,
  WARP_FACTOR,
  TESS_FACTOR,
  USE_WARPING,
  LINE_TO_SM,
  CULL_TRIANGLES,
  MEASURE_CREATESHADOWMAP,
  MEASURE_CREATESHADOWMASK,
  MEASURE_CVS,
  MEASURE_DV,
  MEASURE_COUNTMAP,
  MEASURE_INTEGRATEX,
  MEASURE_OFFSETX,
  MEASURE_SMOOTHX,
  MEASURE_UNWARPX,
  MEASURE_INTEGRATEY,
  MEASURE_OFFSETY,
  MEASURE_SMOOTHY,
  MEASURE_WHOLEWARP,
  MEASURE_WHOLE,
  MEASURE_WHOLEX,
  MEASURE_WHOLEY
DEFVARSIDEND

DEFGETNOFDEP
DEFGETDEP

DEFUPDATEROUTINESSTART
  GETPOINTER(_computeMatrices),
  GETPOINTER(_createShadowMap),
  GETPOINTER(_createShadowMapFBO),
  GETPOINTER(_allocDVTextures),
  GETPOINTER(_allocWarpingTextures),
DEFUPDATEROUTINESEND

DEFUPDATEROUTINEIDSTART
  COMPUTEMATRICES   ,
  CREATESHADOWMAP   ,
  CREATESHADOWMAPFBO,
  ALLOCDVTEXTURES     ,
  ALLOCWARPINGTEXTURES,
DEFUPDATEROUTINEIDEND

DEFVAR2UPDATESTART
  FOCUSPOINT,getMask(COMPUTEMATRICES),
  LIGHT     ,getMask(COMPUTEMATRICES),
  FOVY      ,getMask(COMPUTEMATRICES),
  NEAR      ,getMask(COMPUTEMATRICES),
  FAR       ,getMask(COMPUTEMATRICES),
  RESOLUTION,getMask(CREATESHADOWMAP,ALLOCWARPINGTEXTURES),
  SHADOWMASK,getMask(CREATESHADOWMAPFBO),
DEFVAR2UPDATEEND

DEFUPDATE

void NavyMapping::createShadowMask(){
  GETGPUGAUGE(MEASURE_WHOLE)->begin();
  {
    GETGPUGAUGE(MEASURE_CVS)->begin();
    this->_computeViewSamples->setViewSamples(this->_viewSamples);
    this->_computeViewSamples->setPosition(GETTEXTURE(GBUFFER_POSITION));
    this->_computeViewSamples->setMvp(glm::value_ptr(this->_lightProjection*this->_lightView));
    (*this->_computeViewSamples)();
    GETGPUGAUGE(MEASURE_CVS)->end();

    GETGPUGAUGE(MEASURE_DV)->begin();
    this->_createDesiredView->setViewSamples(this->_viewSamples);
    this->_createDesiredView->setDesiredView(this->_desiredView);
    (*this->_createDesiredView)();
    GETGPUGAUGE(MEASURE_DV)->end();

    GETGPUGAUGE(MEASURE_WHOLEWARP)->begin();
    this->_wholeWarp->setViewSamples(this->_viewSamples);
    this->_wholeWarp->setDesiredView(this->_desiredView);
    this->_wholeWarp->setResolution(GETUINT(RESOLUTION));
    this->_wholeWarp->setWindow(GETUINT(SMOOTH_WINDOW));
    this->_wholeWarp->setFactor(GETFLOAT(WARP_FACTOR));
    this->_wholeWarp->setFastSmooth(GETBOOL(USE_FAST_SMOOTH));
    this->_wholeWarp->setSmoothX(this->_smoothX);
    this->_wholeWarp->setSmoothY(this->_smoothY);
    this->_wholeWarp->setMeasureCountMap(GETGPUGAUGE(MEASURE_COUNTMAP));
    this->_wholeWarp->setMeasureWholeX  (GETGPUGAUGE(MEASURE_WHOLEX  ));
    this->_wholeWarp->setMeasureUnwarpX (GETGPUGAUGE(MEASURE_UNWARPX ));
    this->_wholeWarp->setMeasureWholeY  (GETGPUGAUGE(MEASURE_WHOLEY  ));
    this->_wholeWarp->setMeasureIntegrateX(GETGPUGAUGE(MEASURE_INTEGRATEX));
    this->_wholeWarp->setMeasureOffsetX   (GETGPUGAUGE(MEASURE_OFFSETX   ));
    this->_wholeWarp->setMeasureSmoothX   (GETGPUGAUGE(MEASURE_SMOOTHX   ));
    this->_wholeWarp->setMeasureIntegrateY(GETGPUGAUGE(MEASURE_INTEGRATEY));
    this->_wholeWarp->setMeasureOffsetY   (GETGPUGAUGE(MEASURE_OFFSETY   ));
    this->_wholeWarp->setMeasureSmoothY   (GETGPUGAUGE(MEASURE_SMOOTHY   ));
    (*this->_wholeWarp)();
    /*
    if(GETFLOAT(WARP_FACTOR)>0.f){

      GETGPUGAUGE(MEASURE_COUNTMAP)->begin();
      this->_createCountMap->setViewSamples(this->_viewSamples);
      this->_createCountMap->setDesiredView(this->_desiredView);
      this->_createCountMap->setCountMap(this->_countMapX);
      this->_createCountMap->setResolution(GETUINT(RESOLUTION));
      (*this->_createCountMap)();
      GETGPUGAUGE(MEASURE_COUNTMAP)->end();

      GETGPUGAUGE(MEASURE_WHOLEX)->begin();
      this->_warp->setCountMap(this->_countMapX);
      this->_warp->setSmooth(this->_smoothX);
      this->_warp->setWindow(GETUINT(SMOOTH_WINDOW));
      this->_warp->setResolution(GETUINT(RESOLUTION));
      this->_warp->setFactor(GETFLOAT(WARP_FACTOR));
      this->_warp->setFastSmooth(GETBOOL(USE_FAST_SMOOTH));
      this->_warp->setMeasureIntegrate(GETGPUGAUGE(MEASURE_INTEGRATEX));
      this->_warp->setMeasureOffset(GETGPUGAUGE(MEASURE_OFFSETX));
      this->_warp->setMeasureSmooth(GETGPUGAUGE(MEASURE_SMOOTHX));
      (*this->_warp)();
      GETGPUGAUGE(MEASURE_WHOLEX)->end();

      GETGPUGAUGE(MEASURE_UNWARPX)->begin();
      this->_unwarp->setViewSamples(this->_viewSamples);
      this->_unwarp->setFactor(GETFLOAT(WARP_FACTOR));
      this->_unwarp->setResolution(GETUINT(RESOLUTION));
      this->_unwarp->setCountMapY(this->_countMapY);
      this->_unwarp->setDesiredView(this->_desiredView);
      this->_unwarp->setSmoothX(this->_smoothX);
      this->_unwarp->setSmoothY(this->_smoothY);
      (*this->_unwarp)();
      GETGPUGAUGE(MEASURE_UNWARPX)->end();

      GETGPUGAUGE(MEASURE_WHOLEY)->begin();
      this->_warp->setCountMap(this->_countMapY);
      this->_warp->setSmooth(this->_smoothY);
      this->_warp->setWindow(GETUINT(SMOOTH_WINDOW));
      this->_warp->setResolution(GETUINT(RESOLUTION));
      this->_warp->setFactor(GETFLOAT(WARP_FACTOR));
      this->_warp->setFastSmooth(GETBOOL(USE_FAST_SMOOTH));
      this->_warp->setMeasureIntegrate(GETGPUGAUGE(MEASURE_INTEGRATEY));
      this->_warp->setMeasureOffset(GETGPUGAUGE(MEASURE_OFFSETY));
      this->_warp->setMeasureSmooth(GETGPUGAUGE(MEASURE_SMOOTHY));
      (*this->_warp)();
      GETGPUGAUGE(MEASURE_WHOLEY)->end();
    }*/
    GETGPUGAUGE(MEASURE_WHOLEWARP)->end();
    //this->_unwarpAll();

    GETGPUGAUGE(MEASURE_CREATESHADOWMAP)->begin();
    this->_createNVMap();
    GETGPUGAUGE(MEASURE_CREATESHADOWMAP)->end();

    GETGPUGAUGE(MEASURE_CREATESHADOWMASK)->begin();
    this->_createNVMask();
    GETGPUGAUGE(MEASURE_CREATESHADOWMASK)->end();

  }
  GETGPUGAUGE(MEASURE_WHOLE)->end();
}

void NavyMapping::createShadowMask(GLuint mask){
  this->_shadowMaskFBO->attachColorTexture(GL_COLOR_ATTACHMENT0,mask);
  this->createShadowMask();
}

void NavyMapping::_deleteDVTextures(){
  deleteSetNull(this->_viewSamples);
  delete this->_desiredView;
}
void NavyMapping::_deleteWarpingTextures(){
  deleteSetNull(
      //this->_countMapX        ,
      //this->_countMapY        ,
      this->_smoothX          ,
      this->_smoothY          ,
      this->_uall             );
}

void NavyMapping::_allocDVTextures(){
  this->_deleteDVTextures();

  this->_viewSamples = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_RG32F,1,GETUVEC2(WINDOWSIZE).x,GETUVEC2(WINDOWSIZE).y);

  this->_desiredView = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_RGBA32F,1,1,1);
}

void NavyMapping::_allocWarpingTextures(){
  this->_deleteWarpingTextures();

  auto create2DSquareTex=[](GLenum format,unsigned size){
    return new ge::gl::TextureObject(GL_TEXTURE_2D,format,1,size,size);
  };
  auto setTexParam=[](ge::gl::TextureObject*t){
    t->texParameteri(GL_TEXTURE_WRAP_S    ,GL_CLAMP_TO_EDGE);
    t->texParameteri(GL_TEXTURE_WRAP_T    ,GL_CLAMP_TO_EDGE);
    t->texParameteri(GL_TEXTURE_MIN_FILTER,GL_LINEAR       );
    t->texParameteri(GL_TEXTURE_MAG_FILTER,GL_LINEAR)      ;
  };

  //this->_countMapX = create2DSquareTex(GL_R32UI,GETUINT(RESOLUTION));
  //this->_countMapY = create2DSquareTex(GL_R32UI,GETUINT(RESOLUTION));

  this->_smoothX = create2DSquareTex(GL_R32F,GETUINT(RESOLUTION));
  this->_smoothY = create2DSquareTex(GL_R32F,GETUINT(RESOLUTION));
  setTexParam(this->_smoothX);
  setTexParam(this->_smoothY);

  this->_uall = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32UI,1,GETUINT(RESOLUTION),GETUINT(RESOLUTION));
}

NavyMapping::NavyMapping(simulation::SimulationData*data):simulation::SimulationObject(data){
  this->_simulationData->registerUser(this);
  this->_emptyVAO      = new ge::gl::VertexArrayObject();
  this->_computeMatrices   ();
  this->_createShadowMap   ();
  this->_createShadowMapFBO();


  std::string dir=GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/";
  std::string dvnv=ge::gl::ShaderObject::include(dir+"dv.vp")+ge::gl::ShaderObject::include(dir+"nv.vp");

  //NAVY MAP
  this->_computeViewSamples = new ComputeViewSamples(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/",
      NULL,
      NULL,
      GETUVEC2(WINDOWSIZE).x,
      GETUVEC2(WINDOWSIZE).y,
      NULL,
      GETUINT(VS_SIZE_X),
      GETUINT(VS_SIZE_Y));

  this->_createDesiredView = new CreateDesiredView(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/",
      GETUVEC2(WINDOWSIZE).x,
      GETUVEC2(WINDOWSIZE).y,
      NULL,
      NULL,
      GETUINT(FDV_SIZE_X),
      GETUINT(FDV_SIZE_Y));

  /*
  this->_createCountMap = new CreateCountMap(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/",
      NULL,
      NULL,
      NULL,
      GETUINT(RESOLUTION),
      GETUVEC2(WINDOWSIZE).x,
      GETUVEC2(WINDOWSIZE).y,
      GETUINT(COUNTMAP_SIZE_X),
      GETUINT(COUNTMAP_SIZE_Y));

  this->_warp = new Warp(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/",
      GETUINT(RESOLUTION),
      GETUINT(SMOOTH_WINDOW),
      GETFLOAT(WARP_FACTOR),
      GETUINT(INTEGRATE_SIZE_X),
      GETUINT(OFFSET_SIZE_X),
      GETUINT(OFFSET_SIZE_Y),
      GETUINT(SMOOTH_SIZE_X),
      GETUINT(SMOOTH_SIZE_Y),
      GETUINT(INTEGRATE_SIZE_X),
      GETUINT(SMOOTH_SIZE_X),
      GETUINT(SMOOTH_SIZE_Y));

  this->_unwarp = new Unwarp(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/",
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      GETFLOAT(WARP_FACTOR),
      GETUINT(RESOLUTION),
      GETUVEC2(WINDOWSIZE).x,
      GETUVEC2(WINDOWSIZE).y,
      GETUINT(FDV_SIZE_X),
      GETUINT(FDV_SIZE_Y));
      */
  this->_wholeWarp = new WholeWarp(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/",
      GETUVEC2(WINDOWSIZE).x,
      GETUVEC2(WINDOWSIZE).y,
      GETUINT(RESOLUTION),
      GETUINT(SMOOTH_WINDOW),
      GETFLOAT(WARP_FACTOR),
      GETUINT(COUNTMAP_SIZE_X),
      GETUINT(COUNTMAP_SIZE_Y),
      GETUINT(FDV_SIZE_X),
      GETUINT(FDV_SIZE_Y),
      GETUINT(INTEGRATE_SIZE_X),
      GETUINT(OFFSET_SIZE_X),
      GETUINT(OFFSET_SIZE_Y),
      GETUINT(SMOOTH_SIZE_X),
      GETUINT(SMOOTH_SIZE_Y),
      GETUINT(INTEGRATE_SIZE_X),
      GETUINT(SMOOTH_SIZE_X),
      GETUINT(SMOOTH_SIZE_Y));

  this->_drawGridProgram = new ge::gl::ProgramObject(
      dir+"drawgrid.vp",dir+"drawgrid.cp",dir+"drawgrid.ep",dvnv,dir+"drawgrid.fp");

  this->_uallProgram = new ge::gl::ProgramObject(
      dir+"uall.comp",this->_simulationData->define("nv.program.COUNTMAP")+dvnv);

  this->_createNVMapProgram  = new ge::gl::ProgramObject(
      dir+"createNVTS.vp",
      dir+"createNVTS.cp",
      dvnv,
      dir+"createNVTS.ep",
      dvnv,
      dir+"createNVTS.fp");

  this->_createNVMaskProgram = new ge::gl::ProgramObject(
      dir+"createNVMask.vp",
      dir+"createNVMask.gp",
      dir+"createNVMask.fp",
      dvnv);

  this->_allocDVTextures();
  this->_allocWarpingTextures();
}

void NavyMapping::_setNVParam(ge::gl::ProgramObject*prog){
  prog->set("useWarping",GETFLOAT(WARP_FACTOR)>0.f);
  //prog->set("useWarping",GETBOOL(USE_WARPING));
  this->_desiredView->bindImage(2,0);
  this->_smoothX->bind(GL_TEXTURE3);
  this->_smoothY->bind(GL_TEXTURE4);
}

NavyMapping::~NavyMapping(){
  this->_simulationData->unregisterUser(this);
  delete this->_shadowMap;
  delete this->_fbo;
  delete this->_shadowMaskFBO;
  delete this->_emptyVAO;

  delete this->_computeViewSamples;
  delete this->_createDesiredView;
  //delete this->_createCountMap;
  //delete this->_warp;
  //delete this->_unwarp;
  delete this->_wholeWarp;
  //delete this->_unwarpProgram;
  delete this->_drawGridProgram;
  delete this->_uallProgram;
  delete this->_createNVMapProgram;
  delete this->_createNVMaskProgram;

  this->_deleteDVTextures();
  this->_deleteWarpingTextures();
}

void NavyMapping::_createShadowMap(){
  if(this->_shadowMap)delete this->_shadowMap;
  this->_shadowMap = createSM(GETUINT(RESOLUTION));
  if(!this->_fbo)this->_fbo=new ge::gl::FramebufferObject();
  this->_fbo->attachDepthTexture(this->_shadowMap->getId());
}

void NavyMapping::_computeMatrices(){
  this->setMatrices(glm::perspective(GETFLOAT(FOVY),1.f,GETFLOAT(NEAR),GETFLOAT(FAR)),
      glm::lookAt(glm::vec3(GETLIGHT->position),GETVEC3(FOCUSPOINT),glm::vec3(0.f,1.f,0.f)));
}

void NavyMapping::_createShadowMapFBO(){
  if(!this->_shadowMaskFBO)this->_shadowMaskFBO=new ge::gl::FramebufferObject();
  this->_shadowMaskFBO->attachColorTexture(GL_COLOR_ATTACHMENT0,GETTEXTURE(SHADOWMASK)->getId());
}

void NavyMapping::setMatrices(glm::mat4 lp,glm::mat4 lv){
  this->_lightView       = lv;
  this->_lightProjection = lp;
  this->_bpv=
    glm::scale(glm::mat4(1.),glm::vec3(.5))*
    glm::translate(glm::mat4(1.),glm::vec3(1.))*
    this->_lightProjection*
    this->_lightView;
}

void NavyMapping::_unwarpAll(){
  glm::uvec2 winSize=GETUVEC2(WINDOWSIZE);

  unsigned data[]={1};

  glClearTexImage(this->_uall->getId(),0,GL_RED_INTEGER,GL_UNSIGNED_INT,data);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  this->_viewSamples                   ->bindImage(0,0);
  this->_uall                          ->bindImage(1,0);
  this->_uallProgram->use();
  this->_uallProgram->set("shadowMapSize",GETUINT(RESOLUTION));
  this->_uallProgram->set("windowSize",winSize.x,winSize.y);

  this->_setNVParam(this->_uallProgram);

  unsigned workSizex=getDispatchSize(winSize.x,GETUINT(FDV_SIZE_X));
  unsigned workSizey=getDispatchSize(winSize.y,GETUINT(FDV_SIZE_Y));
  glDispatchCompute(workSizex,workSizey,1);

}

void NavyMapping::drawGrid(float x,float y,float sx,float sy){
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glViewport(
      x *GETUVEC2(WINDOWSIZE).x,
      y *GETUVEC2(WINDOWSIZE).y,
      sx*GETUVEC2(WINDOWSIZE).x,
      sy*GETUVEC2(WINDOWSIZE).y);
  this->_drawGridProgram->use();
  this->_drawGridProgram->set("shadowMapSize",GETUINT(RESOLUTION));


  this->_setNVParam(this->_drawGridProgram);
  this->_emptyVAO->bind();
  glPatchParameteri(GL_PATCH_VERTICES,1);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glDrawArrays(GL_PATCHES,0,1);
  this->_emptyVAO->unbind();
  glViewport(0,0,GETUVEC2(WINDOWSIZE).x,GETUVEC2(WINDOWSIZE).y);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}

void NavyMapping::_createNVMap(){
  glm::mat4 mvp=this->_lightProjection*this->_lightView;

  this->_createNVMapProgram->use();
  this->_createNVMapProgram->set("mvp",1,GL_FALSE,glm::value_ptr(mvp));
  this->_createNVMapProgram->set("shadowMapSize",GETUINT(RESOLUTION));
  this->_createNVMapProgram->set("tessFactor",(unsigned)GETUINT(TESS_FACTOR));
  this->_createNVMapProgram->set("cullTriangles",GETBOOL(CULL_TRIANGLES));

  this->_setNVParam(this->_createNVMapProgram);

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
  //glDrawArrays(GL_TRIANGLES,0,this->_adjacency->NumTriangles*3);
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

void NavyMapping::_createNVMask(){
  glm::mat4 mvp = this->_lightProjection*this->_lightView;

  this->_createNVMaskProgram->use();
  this->_createNVMaskProgram->set("mvp",1,GL_FALSE,glm::value_ptr(mvp));
  this->_createNVMaskProgram->set("shadowMapSize",GETUINT(RESOLUTION));

  this->_setNVParam(this->_createNVMaskProgram);

  GETTEXTURE(GBUFFER_POSITION)->bind(GL_TEXTURE0);
  this->_shadowMap->bind(GL_TEXTURE1);


  this->_shadowMaskFBO->bind();
  this->_emptyVAO->bind();
  glDrawArrays(GL_POINTS,0,1);
  this->_emptyVAO->unbind();
  this->_shadowMaskFBO->unbind();
}

