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
    this->_computeViewSamples();
    GETGPUGAUGE(MEASURE_CVS)->end();

    GETGPUGAUGE(MEASURE_DV)->begin();
    this->_fastCreateDV();
    GETGPUGAUGE(MEASURE_DV)->end();

    GETGPUGAUGE(MEASURE_WHOLEWARP)->begin();
    if(GETFLOAT(WARP_FACTOR)>0.f){

      GETGPUGAUGE(MEASURE_COUNTMAP)->begin();
      this->_fastCreateCountMap();
      GETGPUGAUGE(MEASURE_COUNTMAP)->end();

      GETGPUGAUGE(MEASURE_WHOLEX)->begin();
      {
        GETGPUGAUGE(MEASURE_INTEGRATEX)->begin();
        this->_integrate(this->_integratedX,this->_integratedXCount,this->_countMapX);
        GETGPUGAUGE(MEASURE_INTEGRATEX)->end();

        GETGPUGAUGE(MEASURE_OFFSETX)->begin();
        this->_createOffset(this->_offsetX,this->_integratedX,this->_integratedXCount);
        GETGPUGAUGE(MEASURE_OFFSETX)->end();

        GETGPUGAUGE(MEASURE_SMOOTHX)->begin();
        if(!GETBOOL(USE_FAST_SMOOTH)){
          this->_smooth(this->_smoothX,this->_offsetX,this->_integratedXCount);
        }else{
          this->_integrateOffset(this->_integratedOffsetX,this->_offsetX);
          this->_smoothUsingIntegratedOffset(this->_smoothX,this->_integratedOffsetX);
        }
        GETGPUGAUGE(MEASURE_SMOOTHX)->end();
      }
      GETGPUGAUGE(MEASURE_WHOLEX)->end();

      GETGPUGAUGE(MEASURE_UNWARPX)->begin();
      this->_unwarp();
      GETGPUGAUGE(MEASURE_UNWARPX)->end();

      GETGPUGAUGE(MEASURE_WHOLEY)->begin();
      {
        GETGPUGAUGE(MEASURE_INTEGRATEY)->begin();
        this->_integrate(this->_integratedY,this->_integratedYCount,this->_countMapY);
        GETGPUGAUGE(MEASURE_INTEGRATEY)->end();

        GETGPUGAUGE(MEASURE_OFFSETY)->begin();
        this->_createOffset(this->_offsetY,this->_integratedY,this->_integratedYCount);
        GETGPUGAUGE(MEASURE_OFFSETY)->end();

        GETGPUGAUGE(MEASURE_SMOOTHY)->begin();
        if(!GETBOOL(USE_FAST_SMOOTH)){
          this->_smooth(this->_smoothY,this->_offsetY,this->_integratedYCount);
        }else{
          this->_integrateOffset(this->_integratedOffsetY,this->_offsetY);
          this->_smoothUsingIntegratedOffset(this->_smoothY,this->_integratedOffsetY);
        }
        GETGPUGAUGE(MEASURE_SMOOTHY)->end();
      }
      GETGPUGAUGE(MEASURE_WHOLEY)->end();
    }
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
  for(unsigned i=0;i<this->_dvsTex.size();++i)delete this->_dvsTex[i];
  this->_dvsTex.clear();
  this->_dvsWorkSize.clear();
}
void NavyMapping::_deleteWarpingTextures(){
  deleteSetNull(
      this->_countMapX        ,
      this->_countMapY        ,
      this->_integratedX      ,
      this->_integratedY      ,
      this->_integratedXCount ,
      this->_integratedYCount ,
      this->_offsetX          ,
      this->_offsetY          ,
      this->_smoothX          ,
      this->_smoothY          ,
      this->_integratedOffsetX,
      this->_integratedOffsetY,
      this->_uall             );
}

void NavyMapping::_allocDVTextures(){
  this->_deleteDVTextures();

  this->_viewSamples = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_RG32F,1,GETUVEC2(WINDOWSIZE).x,GETUVEC2(WINDOWSIZE).y);

  unsigned size[2]={
    getDispatchSize(GETUVEC2(WINDOWSIZE).x,GETUINT(FDV_SIZE_X)),
    getDispatchSize(GETUVEC2(WINDOWSIZE).y,GETUINT(FDV_SIZE_Y)),
  };
  while(size[0]!=1||size[1]!=1){
    this->_dvsWorkSize.push_back(glm::uvec2(size[0],size[1]));
    this->_dvsTex.push_back(new ge::gl::TextureObject(GL_TEXTURE_2D,GL_RGBA32F,1,size[0],size[1]));
    size[0]=getDispatchSize(size[0],GETUINT(FDV_SIZE_X));
    size[1]=getDispatchSize(size[1],GETUINT(FDV_SIZE_Y));
  }
  this->_dvsWorkSize.push_back(glm::uvec2(1u,1u));
  this->_dvsTex.push_back(new ge::gl::TextureObject(GL_TEXTURE_2D,GL_RGBA32F,1,1,1));
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

  this->_countMapX = create2DSquareTex(GL_R32UI,GETUINT(RESOLUTION));
  this->_countMapY = create2DSquareTex(GL_R32UI,GETUINT(RESOLUTION));

  this->_integratedX = create2DSquareTex(GL_R32UI,GETUINT(RESOLUTION));
  this->_integratedY = create2DSquareTex(GL_R32UI,GETUINT(RESOLUTION));
  this->_integratedXCount = new ge::gl::TextureObject(GL_TEXTURE_1D,GL_R32UI,1,GETUINT(RESOLUTION));
  this->_integratedYCount = new ge::gl::TextureObject(GL_TEXTURE_1D,GL_R32UI,1,GETUINT(RESOLUTION));

  this->_offsetX = create2DSquareTex(GL_R32F,GETUINT(RESOLUTION));
  this->_offsetY = create2DSquareTex(GL_R32F,GETUINT(RESOLUTION));
  setTexParam(this->_offsetX);
  setTexParam(this->_offsetY);

  this->_smoothX = create2DSquareTex(GL_R32F,GETUINT(RESOLUTION));
  this->_smoothY = create2DSquareTex(GL_R32F,GETUINT(RESOLUTION));
  setTexParam(this->_smoothX);
  setTexParam(this->_smoothY);

  this->_integratedOffsetX = create2DSquareTex(GL_R32F,GETUINT(RESOLUTION));
  this->_integratedOffsetY = create2DSquareTex(GL_R32F,GETUINT(RESOLUTION));
  setTexParam(this->_integratedOffsetX);
  setTexParam(this->_integratedOffsetY);

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
  this->_viewSamplesProgram = new ge::gl::ProgramObject(
      dir+"viewSamples.comp",this->_simulationData->define("nv.program.VS"));

  this->_fastdv0Program = new ge::gl::ProgramObject(
      dir+"fastdv0.comp",this->_simulationData->define("nv.program.FDV"));

  this->_fastdvProgram  = new ge::gl::ProgramObject(
      dir+"fastdv.comp" ,this->_simulationData->define("nv.program.FDV"));

  this->_fastCreateCountMapProgram = new ge::gl::ProgramObject(
      dir+"countMap.comp",this->_simulationData->define("nv.program.COUNTMAP")+dvnv);

  this->_integrateProgram = new ge::gl::ProgramObject(
      dir+"integrate.comp",this->_simulationData->define("nv.program.INTEGRATE"));

  this->_offsetProgram = new ge::gl::ProgramObject(
      dir+"offset.comp",this->_simulationData->define("nv.program.OFFSET"));

  this->_smoothProgram = new ge::gl::ProgramObject(
      dir+"smooth.comp",this->_simulationData->define("nv.program.SMOOTH"));

  this->_integrateOffsetProgram = new ge::gl::ProgramObject(
      dir+"integrateoffset.comp",this->_simulationData->define("nv.program.INTEGRATEOFFSET"));

  this->_smoothUsingIntegratedOffsetProgram = new ge::gl::ProgramObject(
      dir+"smoothUsingIntegratedOffset.comp",this->_simulationData->define("nv.program.SMOOTH"));

  this->_unwarpProgram   = new ge::gl::ProgramObject(
      dir+"unwarp.comp",this->_simulationData->define("nv.program.COUNTMAP")+dvnv);

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
  this->_dvsTex[this->_dvsTex.size()-1]->bindImage(2,0);
  this->_smoothX->bind(GL_TEXTURE3);
  this->_smoothY->bind(GL_TEXTURE4);
}

NavyMapping::~NavyMapping(){
  this->_simulationData->unregisterUser(this);
  delete this->_shadowMap;
  delete this->_fbo;
  delete this->_shadowMaskFBO;
  delete this->_emptyVAO;

  delete this->_viewSamplesProgram;
  delete this->_fastdv0Program;
  delete this->_fastdvProgram;
  delete this->_fastCreateCountMapProgram;
  delete this->_integrateProgram;
  delete this->_smoothProgram;
  delete this->_unwarpProgram;
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


void NavyMapping::_computeViewSamples(){
  float data[]={2,2};
  glClearTexImage(this->_viewSamples->getId(),0,GL_RG,GL_FLOAT,data);

  glm::uvec2 winSize=GETUVEC2(WINDOWSIZE);
  glm::mat4 mvp=this->_lightProjection*this->_lightView;

  GETTEXTURE(GBUFFER_POSITION)->bind(GL_TEXTURE0);
  this->_viewSamples->bindImage(1,0);
  this->_viewSamplesProgram->use();
  this->_viewSamplesProgram->set("windowSize",winSize.x,winSize.y);
  this->_viewSamplesProgram->set("mvp",1,GL_FALSE,(const float*)glm::value_ptr(mvp));
  unsigned workSizex=getDispatchSize(winSize.x,GETUINT(VS_SIZE_X));
  unsigned workSizey=getDispatchSize(winSize.y,GETUINT(VS_SIZE_Y));
  glDispatchCompute(workSizex,workSizey,1);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void NavyMapping::_fastCreateDV(){
  glm::uvec2 winSize=GETUVEC2(WINDOWSIZE);
  this->_fastdv0Program->use();
  this->_fastdv0Program->set("windowSize",winSize.x,winSize.y);
  this->_viewSamples->bindImage(0,0);
  this->_dvsTex[0]->bindImage(1,0);
  glDispatchCompute(this->_dvsWorkSize[0].x,this->_dvsWorkSize[0].y,1);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  this->_fastdvProgram->use();
  for(unsigned i=0;i<this->_dvsTex.size()-1;++i){
    this->_dvsTex[i+0]->bindImage(0,0);
    this->_dvsTex[i+1]->bindImage(1,0);
    this->_fastdvProgram->set("windowSize",this->_dvsWorkSize[i].x,this->_dvsWorkSize[i].y);
    glDispatchCompute(this->_dvsWorkSize[i+1].x,this->_dvsWorkSize[i+1].y,1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  }
}

void NavyMapping::_fastCreateCountMap(){
  glm::uvec2 winSize=GETUVEC2(WINDOWSIZE);

  unsigned data[]={1};
  glClearTexImage(this->_countMapX->getId(),0,GL_RED_INTEGER,GL_UNSIGNED_INT,data);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  this->_viewSamples                   ->bindImage(0,0);
  this->_countMapX                     ->bindImage(1,0);
  this->_dvsTex[this->_dvsTex.size()-1]->bindImage(2,0);
  this->_fastCreateCountMapProgram->use();
  this->_fastCreateCountMapProgram->set("shadowMapSize",GETUINT(RESOLUTION));
  this->_fastCreateCountMapProgram->set("windowSize",winSize.x,winSize.y);

  unsigned workSizex=getDispatchSize(winSize.x,GETUINT(FDV_SIZE_X));
  unsigned workSizey=getDispatchSize(winSize.y,GETUINT(FDV_SIZE_Y));
  glDispatchCompute(workSizex,workSizey,1);
}

void NavyMapping::_integrate(
    ge::gl::TextureObject*integral,
    ge::gl::TextureObject*integralCount,
    ge::gl::TextureObject*countMap){
  countMap     ->bindImage(0,0);
  integral     ->bindImage(1,0);
  integralCount->bindImage(2,0);
  this->_integrateProgram->use();
  this->_integrateProgram->set("shadowMapSize",GETUINT(RESOLUTION));
  unsigned workSizex=getDispatchSize(GETUINT(RESOLUTION),GETUINT(INTEGRATE_SIZE_X));
  glDispatchCompute(workSizex,1,1);
}


void NavyMapping::_createOffset(
    ge::gl::TextureObject*offset,
    ge::gl::TextureObject*integral,
    ge::gl::TextureObject*integralCount){
  offset       ->bindImage(0,0);
  integral     ->bindImage(1,0);
  integralCount->bindImage(2,0);
  this->_offsetProgram->use();
  this->_offsetProgram->set("shadowMapSize",GETUINT(RESOLUTION));
  unsigned workSizex=getDispatchSize(GETUINT(RESOLUTION),GETUINT(OFFSET_SIZE_X));
  unsigned workSizey=getDispatchSize(GETUINT(RESOLUTION),GETUINT(OFFSET_SIZE_Y));
  glDispatchCompute(workSizex,workSizey,1);
}

void NavyMapping::_smooth(
    ge::gl::TextureObject*smooth,
    ge::gl::TextureObject*offset,
    ge::gl::TextureObject*integralCount){
  smooth       ->bindImage(0,0);
  offset       ->bindImage(1,0);
  integralCount->bindImage(2,0);
  this->_smoothProgram->use();
  this->_smoothProgram->set("shadowMapSize",GETUINT(RESOLUTION));
  this->_smoothProgram->set("smoothWindowSize",GETUINT(SMOOTH_WINDOW));
  this->_smoothProgram->set("warpFactor",GETFLOAT(WARP_FACTOR));
  unsigned workSizex=getDispatchSize(GETUINT(RESOLUTION),GETUINT(SMOOTH_SIZE_X));
  unsigned workSizey=getDispatchSize(GETUINT(RESOLUTION),GETUINT(SMOOTH_SIZE_Y));
  glDispatchCompute(workSizex,workSizey,1);
}

void NavyMapping::_integrateOffset(
    ge::gl::TextureObject*integratedOffset,
    ge::gl::TextureObject*offset){
  integratedOffset->bindImage(0,0);
  offset          ->bindImage(1,0);
  this->_integrateOffsetProgram->use();
  this->_integrateOffsetProgram->set("shadowMapSize",GETUINT(RESOLUTION));
  unsigned workSizex=getDispatchSize(GETUINT(RESOLUTION),GETUINT(INTEGRATE_SIZE_X));
  glDispatchCompute(workSizex,1,1);
}
void NavyMapping::_smoothUsingIntegratedOffset(
    ge::gl::TextureObject*smooth,
    ge::gl::TextureObject*integratedOffset){
  smooth          ->bindImage(0,0);
  integratedOffset->bindImage(1,0);
  this->_smoothUsingIntegratedOffsetProgram->use();
  this->_smoothUsingIntegratedOffsetProgram->set("shadowMapSize",GETUINT(RESOLUTION));
  this->_smoothUsingIntegratedOffsetProgram->set("smoothWindowSize",GETUINT(SMOOTH_WINDOW));
  this->_smoothUsingIntegratedOffsetProgram->set("warpFactor",GETFLOAT(WARP_FACTOR));
  unsigned workSizex=getDispatchSize(GETUINT(RESOLUTION),GETUINT(SMOOTH_SIZE_X));
  unsigned workSizey=getDispatchSize(GETUINT(RESOLUTION),GETUINT(SMOOTH_SIZE_Y));
  glDispatchCompute(workSizex,workSizey,1);
}


void NavyMapping::_unwarp(){
  glm::uvec2 winSize=GETUVEC2(WINDOWSIZE);

  unsigned data[]={1};
  glClearTexImage(this->_countMapY->getId(),0,GL_RED_INTEGER,GL_UNSIGNED_INT,data);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  this->_viewSamples                   ->bindImage(0,0);
  this->_countMapY                     ->bindImage(1,0);
  this->_unwarpProgram->use();
  this->_unwarpProgram->set("shadowMapSize",GETUINT(RESOLUTION));
  this->_unwarpProgram->set("windowSize",winSize.x,winSize.y);

  this->_setNVParam(this->_unwarpProgram);

  unsigned workSizex=getDispatchSize(winSize.x,GETUINT(FDV_SIZE_X));
  unsigned workSizey=getDispatchSize(winSize.y,GETUINT(FDV_SIZE_Y));
  glDispatchCompute(workSizex,workSizey,1);
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

