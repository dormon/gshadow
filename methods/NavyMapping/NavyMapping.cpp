#include"NavyMapping.h"

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
  "window.size",
  "adjacency",
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
  "nv.program.warpFactor",
  "nv.program.NVMAP.TESS_FACTOR",
  "nv.drawLinesToSM",
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
  WARP_FACTOR,
  TESS_FACTOR,
  LINE_TO_SM,
  MEASURE_CREATESHADOWMAP,
  MEASURE_CREATESHADOWMASK
DEFVARSIDEND

DEFGETNOFDEP
DEFGETDEP

void NavyMapping::update(){
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

void NavyMapping::createShadowMask(){
  this->_computeViewSamples();
  this->_fastCreateDV();
  if(GETFLOAT(WARP_FACTOR)>0.f){
    this->_fastCreateCountMap();
    this->_integrate(this->_integratedX,this->_integratedXCount,this->_countMapX);
    this->_createOffset(this->_offsetX,this->_integratedX,this->_integratedXCount);
    this->_smooth(this->_smoothX,this->_offsetX,this->_integratedXCount);
    this->_unwarp();
    this->_integrate(this->_integratedY,this->_integratedYCount,this->_countMapY);
    this->_createOffset(this->_offsetY,this->_integratedY,this->_integratedYCount);
    this->_smooth(this->_smoothY,this->_offsetY,this->_integratedYCount);
  }
  //this->_unwarpAll();

  GETGPUGAUGE(MEASURE_CREATESHADOWMAP)->begin();
  this->_createNVMap();
  GETGPUGAUGE(MEASURE_CREATESHADOWMAP)->end();

  GETGPUGAUGE(MEASURE_CREATESHADOWMASK)->begin();
  this->_createNVMask();
  GETGPUGAUGE(MEASURE_CREATESHADOWMASK)->end();
}

void NavyMapping::createShadowMask(GLuint mask){
  this->_shadowMaskFBO->attachColorTexture(GL_COLOR_ATTACHMENT0,mask);
  this->createShadowMask();
}


NavyMapping::NavyMapping(simulation::SimulationData*data):simulation::SimulationObject(data){
  this->_simulationData->registerUser(this);
  this->_shadowMap     = NULL;
  this->_fbo           = NULL;
  this->_shadowMaskFBO = NULL;
  this->_emptyVAO      = new ge::gl::VertexArrayObject();
  this->_computeMatrices   ();
  this->_createShadowMap   ();
  this->_createShadowMapFBO();

  std::string dir=GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/";
  std::string dvnv=ge::gl::ShaderObject::include(dir+"dv.vp")+ge::gl::ShaderObject::include(dir+"nv.vp");

  //NAVY MAP
  this->_viewSamples = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_RG32F,1,GETUVEC2(WINDOWSIZE).x,GETUVEC2(WINDOWSIZE).y);
  this->_viewSamplesProgram = new ge::gl::ProgramObject(dir+"viewSamples.comp",this->_simulationData->define("nv.program.VS"));

  this->_fastdv0Program = new ge::gl::ProgramObject(dir+"fastdv0.comp",this->_simulationData->define("nv.program.FDV"));
  this->_fastdvProgram  = new ge::gl::ProgramObject(dir+"fastdv.comp" ,this->_simulationData->define("nv.program.FDV"));

  unsigned size[2]={
    this->_getDiv(GETUVEC2(WINDOWSIZE).x,GETUINT(FDV_SIZE_X)),
    this->_getDiv(GETUVEC2(WINDOWSIZE).y,GETUINT(FDV_SIZE_Y))
  };
  while(size[0]!=1||size[1]!=1){
    this->_dvsWorkSize.push_back(glm::uvec2(size[0],size[1]));
    this->_dvsTex.push_back(new ge::gl::TextureObject(GL_TEXTURE_2D,GL_RGBA32F,1,size[0],size[1]));
    size[0]=this->_getDiv(size[0],GETUINT(FDV_SIZE_X));
    size[1]=this->_getDiv(size[1],GETUINT(FDV_SIZE_Y));
  }
  this->_dvsWorkSize.push_back(glm::uvec2(1u,1u));
  this->_dvsTex.push_back(new ge::gl::TextureObject(GL_TEXTURE_2D,GL_RGBA32F,1,1,1));

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

  this->_fastCreateCountMapProgram = new ge::gl::ProgramObject(dir+"countMap.comp",this->_simulationData->define("nv.program.COUNTMAP")+dvnv);

  this->_integratedX = create2DSquareTex(GL_R32UI,GETUINT(RESOLUTION));
  this->_integratedY = create2DSquareTex(GL_R32UI,GETUINT(RESOLUTION));
  this->_integratedXCount = new ge::gl::TextureObject(GL_TEXTURE_1D,GL_R32UI,1,GETUINT(RESOLUTION));
  this->_integratedYCount = new ge::gl::TextureObject(GL_TEXTURE_1D,GL_R32UI,1,GETUINT(RESOLUTION));
  this->_integrateProgram = new ge::gl::ProgramObject(dir+"integrate.comp",this->_simulationData->define("nv.program.INTEGRATE"));


  this->_offsetX = create2DSquareTex(GL_R32F,GETUINT(RESOLUTION));
  this->_offsetY = create2DSquareTex(GL_R32F,GETUINT(RESOLUTION));
  setTexParam(this->_offsetX);
  setTexParam(this->_offsetY);
  this->_offsetProgram = new ge::gl::ProgramObject(dir+"offset.comp",this->_simulationData->define("nv.program.OFFSET"));

  this->_smoothX = create2DSquareTex(GL_R32F,GETUINT(RESOLUTION));
  this->_smoothY = create2DSquareTex(GL_R32F,GETUINT(RESOLUTION));
  setTexParam(this->_smoothX);
  setTexParam(this->_smoothY);
  this->_smoothProgram = new ge::gl::ProgramObject(dir+"smooth.comp",this->_simulationData->define("nv.program.SMOOTH"));

  this->_unwarpProgram   = new ge::gl::ProgramObject(dir+"unwarp.comp",this->_simulationData->define("nv.program.COUNTMAP")+dvnv);
  this->_drawGridProgram = new ge::gl::ProgramObject(dir+"drawgrid.vp",dir+"drawgrid.cp",dir+"drawgrid.ep",dvnv,dir+"drawgrid.fp");

  this->_uall = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32UI,1,GETUINT(RESOLUTION),GETUINT(RESOLUTION));
  this->_uallProgram = new ge::gl::ProgramObject(dir+"uall.comp",this->_simulationData->define("nv.program.COUNTMAP")+dvnv);

  std::string shader=dir+"createNVTS";
  this->_createNVMapProgram  = new ge::gl::ProgramObject(shader+".vp",shader+".cp",dvnv,shader+".ep",dvnv,shader+".fp");
  shader=dir+"createNVMask";
  this->_createNVMaskProgram = new ge::gl::ProgramObject(shader+".vp",shader+".gp",shader+".fp",dvnv);
}

void NavyMapping::_setNVParam(ge::gl::ProgramObject*prog){
  prog->set("useWarping",GETFLOAT(WARP_FACTOR)>0.f);
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

  delete this->_viewSamples;
  delete this->_viewSamplesProgram;

  delete this->_fastdv0Program;
  delete this->_fastdvProgram;

  for(unsigned i=0;i<this->_dvsTex.size();++i)
    delete this->_dvsTex[i];

  delete this->_countMapX;
  delete this->_countMapY;
  delete this->_fastCreateCountMapProgram;

  delete this->_integratedX;
  delete this->_integratedY;
  delete this->_integratedXCount;
  delete this->_integratedYCount;
  delete this->_integrateProgram;

  delete this->_offsetX;
  delete this->_offsetY;

  delete this->_smoothProgram;
  delete this->_unwarpProgram;
  delete this->_drawGridProgram;
  delete this->_uall;
  delete this->_uallProgram;
  delete this->_createNVMapProgram;
  delete this->_createNVMaskProgram;
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
  unsigned workSizex=winSize.x/GETUINT(VS_SIZE_X)+1;
  unsigned workSizey=winSize.y/GETUINT(VS_SIZE_Y)+1;
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

/*
  NDormon::GpuPerfApi*gpa=(NDormon::GpuPerfApi*)GETOBJECT(GPA);
  std::cerr<<gpa->getResults([](void*A){
      NavyMapping*_this= ((NavyMapping*)A);
        glDispatchCompute(_this->_dvsWorkSize[0].x,_this->_dvsWorkSize[0].y,1);
      },this)<<std::endl;
  */
  //return;

  this->_fastdvProgram->use();
  for(unsigned i=0;i<this->_dvsTex.size()-1;++i){
    this->_dvsTex[i+0]->bindImage(0,0);
    this->_dvsTex[i+1]->bindImage(1,0);
    this->_fastdvProgram->set("windowSize",this->_dvsWorkSize[i].x,this->_dvsWorkSize[i].y);
    glDispatchCompute(this->_dvsWorkSize[i+1].x,this->_dvsWorkSize[i+1].y,1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  }

  /*
  NDormon::GpuPerfApi*gpa=(NDormon::GpuPerfApi*)GETOBJECT(GPA);
  std::cerr<<gpa->getResults([](void*A){
    NavyMapping*_this=(NavyMapping*)A;
    _this->_fastdvProgram->use();
    for(unsigned i=0;i<_this->_dvsTex.size()-1;++i){
      _this->_dvsTex[i+0]->bindImage(0,0);
      _this->_dvsTex[i+1]->bindImage(1,0);
      _this->_fastdvProgram->set("windowSize",_this->_dvsWorkSize[i].x,_this->_dvsWorkSize[i].y);
      glDispatchCompute(_this->_dvsWorkSize[i+1].x,_this->_dvsWorkSize[i+1].y,1);
      glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    }
  },this)<<std::endl;
  */

  /*float data[4]={0,0,0,0};
    glBindTexture(GL_TEXTURE_2D,this->_dvsTex[this->_dvsTex.size()-1]->getId());
    glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_FLOAT,data);
    std::cerr<<"data0: "<<data[0]<<std::endl;
    std::cerr<<"data1: "<<data[1]<<std::endl;
    std::cerr<<"data2: "<<data[2]<<std::endl;
    std::cerr<<"data3: "<<data[3]<<std::endl;
    */
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

  unsigned workSizex=winSize.x/GETUINT(FDV_SIZE_X)+1;
  unsigned workSizey=winSize.y/GETUINT(FDV_SIZE_Y)+1;
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
  unsigned workSizex=GETUINT(RESOLUTION)/GETUINT(INTEGRATE_SIZE_X)+1;
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
  unsigned workSizex=GETUINT(RESOLUTION)/GETUINT(OFFSET_SIZE_X)+1;
  unsigned workSizey=GETUINT(RESOLUTION)/GETUINT(OFFSET_SIZE_Y)+1;
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
  unsigned workSizex=GETUINT(RESOLUTION)/GETUINT(SMOOTH_SIZE_X)+1;
  unsigned workSizey=GETUINT(RESOLUTION)/GETUINT(SMOOTH_SIZE_Y)+1;
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

  unsigned workSizex=winSize.x/GETUINT(FDV_SIZE_X)+1;
  unsigned workSizey=winSize.y/GETUINT(FDV_SIZE_Y)+1;
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

  unsigned workSizex=winSize.x/GETUINT(FDV_SIZE_X)+1;
  unsigned workSizey=winSize.y/GETUINT(FDV_SIZE_Y)+1;
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
  this->_createNVMapProgram->set("tessFactor",GETUINT(TESS_FACTOR));

  this->_setNVParam(this->_createNVMapProgram);

  this->_fbo->bind();
  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
  glViewport(0,0,GETUINT(RESOLUTION),GETUINT(RESOLUTION));
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);
  glClear(GL_DEPTH_BUFFER_BIT);

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(15.5,10);

  GETVAO(SCENEVAO)->bind();
  //glDrawArrays(GL_TRIANGLES,0,this->_adjacency->NumTriangles*3);
  glPatchParameteri(GL_PATCH_VERTICES,3);
  if(GETBOOL(LINE_TO_SM))glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glDrawArrays(GL_PATCHES,0,GETADJACENCY->NumTriangles*3);//this->_adjacency->NumTriangles*3);
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

