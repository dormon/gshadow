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
    glm::mat4 mvp=this->_lightProjection*this->_lightView;
    this->_createWarping->setPosition(GETTEXTURE(GBUFFER_POSITION));
    this->_createWarping->setMvp(glm::value_ptr(mvp));
    this->_createWarping->setDesiredView(this->_desiredView);
    this->_createWarping->setResolution(GETUINT(RESOLUTION));
    this->_createWarping->setWindow(GETUINT(SMOOTH_WINDOW));
    this->_createWarping->setFactor(GETFLOAT(WARP_FACTOR));
    this->_createWarping->setFastSmooth(GETBOOL(USE_FAST_SMOOTH));
    this->_createWarping->setSmoothX(this->_smoothX);
    this->_createWarping->setSmoothY(this->_smoothY);
    this->_createWarping->setMeasureComputeViewSamples(GETGPUGAUGE(MEASURE_CVS       ));
    this->_createWarping->setMeasureCreateDesiredView (GETGPUGAUGE(MEASURE_DV        ));
    this->_createWarping->setMeasureWholeWarp         (GETGPUGAUGE(MEASURE_WHOLEWARP ));
    this->_createWarping->setMeasureCountMap          (GETGPUGAUGE(MEASURE_COUNTMAP  ));
    this->_createWarping->setMeasureWholeX            (GETGPUGAUGE(MEASURE_WHOLEX    ));
    this->_createWarping->setMeasureUnwarpX           (GETGPUGAUGE(MEASURE_UNWARPX   ));
    this->_createWarping->setMeasureWholeY            (GETGPUGAUGE(MEASURE_WHOLEY    ));
    this->_createWarping->setMeasureIntegrateX        (GETGPUGAUGE(MEASURE_INTEGRATEX));
    this->_createWarping->setMeasureOffsetX           (GETGPUGAUGE(MEASURE_OFFSETX   ));
    this->_createWarping->setMeasureSmoothX           (GETGPUGAUGE(MEASURE_SMOOTHX   ));
    this->_createWarping->setMeasureIntegrateY        (GETGPUGAUGE(MEASURE_INTEGRATEY));
    this->_createWarping->setMeasureOffsetY           (GETGPUGAUGE(MEASURE_OFFSETY   ));
    this->_createWarping->setMeasureSmoothY           (GETGPUGAUGE(MEASURE_SMOOTHY   ));
    (*this->_createWarping)();

    //this->_unwarpAll();

    GETGPUGAUGE(MEASURE_CREATESHADOWMAP)->begin();
    this->_createNavyShadowMap->setShadowMap(this->_shadowMap);
    this->_createNavyShadowMap->setSmoothX(this->_smoothX);
    this->_createNavyShadowMap->setSmoothY(this->_smoothY);
    this->_createNavyShadowMap->setDesiredView(this->_desiredView);
    this->_createNavyShadowMap->setMvp(glm::value_ptr(mvp));
    this->_createNavyShadowMap->setResolution(GETUINT(RESOLUTION));
    this->_createNavyShadowMap->setTessFactor(GETUINT(TESS_FACTOR));
    this->_createNavyShadowMap->setCullTriangles(GETBOOL(CULL_TRIANGLES));
    this->_createNavyShadowMap->setLineToSM(GETBOOL(LINE_TO_SM));
    this->_createNavyShadowMap->setPolygonOffsetFactor(GETFLOAT(FACTOR));
    this->_createNavyShadowMap->setPolygonOffsetUnits(GETFLOAT(UNITS));
    this->_createNavyShadowMap->setFactor(GETFLOAT(WARP_FACTOR));
    (*this->_createNavyShadowMap)();
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
  delete this->_desiredView;
}
void NavyMapping::_deleteWarpingTextures(){
  deleteSetNull(
      this->_smoothX          ,
      this->_smoothY          ,
      this->_uall             );
}

void NavyMapping::_allocDVTextures(){
  this->_deleteDVTextures();
  this->_desiredView = createDesiredViewTexture();
}

void NavyMapping::_allocWarpingTextures(){
  this->_deleteWarpingTextures();

  this->_smoothX = createSmoothTexture(GETUINT(RESOLUTION));
  this->_smoothY = createSmoothTexture(GETUINT(RESOLUTION));

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
  this->_createWarping = new CreateWarping(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/",
      GETUVEC2(WINDOWSIZE).x,
      GETUVEC2(WINDOWSIZE).y,
      GETUINT(RESOLUTION),
      GETUINT(SMOOTH_WINDOW),
      GETFLOAT(WARP_FACTOR),
      "",
      GETUINT(VS_SIZE_X),
      GETUINT(VS_SIZE_Y),
      GETUINT(FDV_SIZE_X),
      GETUINT(FDV_SIZE_Y),
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

  this->_createNavyShadowMap = new CreateNavyShadowMap(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/",
      GETUVEC2(WINDOWSIZE).x,
      GETUVEC2(WINDOWSIZE).y,
      GETVAO(SCENEVAO),
      GETFASTADJACENCY->getNofTriangles());

  this->_drawGridProgram = new ge::gl::ProgramObject(
      dir+"drawgrid.vp",dir+"drawgrid.cp",dir+"drawgrid.ep",dvnv,dir+"drawgrid.fp");

  this->_uallProgram = new ge::gl::ProgramObject(
      dir+"uall.comp",this->_simulationData->define("nv.program.COUNTMAP")+dvnv);

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
  
  
  //this->_desiredView->bindImage(2,0);
  this->_desiredView->bind(GL_TEXTURE2);
  this->_smoothX->bind(GL_TEXTURE3);
  this->_smoothY->bind(GL_TEXTURE4);
}

NavyMapping::~NavyMapping(){
  this->_simulationData->unregisterUser(this);
  delete this->_shadowMap;
  delete this->_shadowMaskFBO;
  delete this->_emptyVAO;

  delete this->_createWarping;
  delete this->_createNavyShadowMap;

  delete this->_drawGridProgram;
  delete this->_uallProgram;
  delete this->_createNVMaskProgram;

  this->_deleteDVTextures();
  this->_deleteWarpingTextures();
}

void NavyMapping::_createShadowMap(){
  if(this->_shadowMap)delete this->_shadowMap;
  this->_shadowMap = createSM(GETUINT(RESOLUTION));
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

/*
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
*/

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

