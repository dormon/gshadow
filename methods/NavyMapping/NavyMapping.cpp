#include"NavyMapping.h"

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
  "nv.program.VS.WORKGROUP_SIZE_X",
  "nv.program.VS.WORKGROUP_SIZE_Y",
  "nv.program.FDV.WORKGROUP_SIZE_X",
  "nv.program.FDV.WORKGROUP_SIZE_Y",
  "nv.program.DV.WORKGROUP_SIZE_X",
  "nv.program.DV.WORKGROUP_SIZE_Y",
  "nv.program.COUNTMAP.WORKGROUP_SIZE_X",
  "nv.program.COUNTMAP.WORKGROUP_SIZE_Y",
  "nv.program.INTEGRATE.WORKGROUP_SIZE_X",
  "nv.program.OFFSET.WORKGROUP_SIZE_X",
  "nv.program.OFFSET.WORKGROUP_SIZE_Y",
  "nv.program.SMOOTH.WORKGROUP_SIZE_X",
  "nv.program.SMOOTH.WORKGROUP_SIZE_Y",
  "nv.program.SMOOTH.WINDOW_SIZE",
  "nv.program.ISO.WORKGROUP_SIZE_X",
  "nv.program.ISO.GRID_X",
  "nv.program.ISO.GRID_Y",
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
  VS_SIZE_X,
  VS_SIZE_Y,
  FDV_SIZE_X,
  FDV_SIZE_Y,
  DV_SIZE_X,
  DV_SIZE_Y,
  COUNTMAP_SIZE_X,
  COUNTMAP_SIZE_Y,
  INTEGRATE_SIZE_X,
  OFFSET_SIZE_X,
  OFFSET_SIZE_Y,
  SMOOTH_SIZE_X,
  SMOOTH_SIZE_Y,
  SMOOTH_WINDOW,
  ISO_SIZE_X,
  ISO_GRID_X,
  ISO_GRID_Y,
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
  //this->_createDV();
  this->_computeViewSamples();
  this->_fastCreateDV();
  this->_fastCreateCountMap();
  //this->_createCountMap();
  this->_integrate(this->_integratedX,this->_integratedXCount,this->_countMapX);
  this->_createOffset(this->_offsetX,this->_integratedX,this->_integratedXCount);
  this->_smooth(this->_smoothX,this->_offsetX,this->_integratedXCount);
  this->_unwarp();
  this->_integrate(this->_integratedY,this->_integratedYCount,this->_countMapY);
  this->_createOffset(this->_offsetY,this->_integratedY,this->_integratedYCount);
  this->_smooth(this->_smoothY,this->_offsetY,this->_integratedYCount);
  this->_unwarpAll();


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

void NavyMapping::createShadowMask(GLuint mask){
  this->_shadowMaskFBO->attachColorTexture(GL_COLOR_ATTACHMENT0,mask);
  this->createShadowMask();
}


NavyMapping::NavyMapping(simulation::SimulationData*data):simulation::SimulationObject(data){
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

  //NAVY MAP
  this->_viewSamples = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_RG32F,1,
      GETUVEC2(WINDOWSIZE).x,GETUVEC2(WINDOWSIZE).y);
  this->_viewSamplesProgram = new ge::gl::ProgramObject(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/viewSamples.comp",
      this->_simulationData->define("nv.program.VS"));

  this->_fastdv0Program = new ge::gl::ProgramObject(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/fastdv0.comp",
      this->_simulationData->define("nv.program.FDV"));
  this->_fastdvProgram = new ge::gl::ProgramObject(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/fastdv.comp",
      this->_simulationData->define("nv.program.FDV"));

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

  this->_countMapX = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32UI,1,GETUINT(RESOLUTION),GETUINT(RESOLUTION));
  this->_countMapY = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32UI,1,GETUINT(RESOLUTION),GETUINT(RESOLUTION));

  this->_fastCreateCountMapProgram = new ge::gl::ProgramObject(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/countMap.comp",
      this->_simulationData->define("nv.program.COUNTMAP")+
      ge::gl::ShaderObject::include(GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/dv.vp"));

  this->_integratedX = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32UI,1,GETUINT(RESOLUTION),GETUINT(RESOLUTION));
  this->_integratedY = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32UI,1,GETUINT(RESOLUTION),GETUINT(RESOLUTION));
  this->_integratedXCount = new ge::gl::TextureObject(GL_TEXTURE_1D,GL_R32UI,1,GETUINT(RESOLUTION));
  this->_integratedYCount = new ge::gl::TextureObject(GL_TEXTURE_1D,GL_R32UI,1,GETUINT(RESOLUTION));
  this->_integrateProgram = new ge::gl::ProgramObject(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/integrate.comp",
      this->_simulationData->define("nv.program.INTEGRATE"));

  this->_offsetX = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32F,1,GETUINT(RESOLUTION),GETUINT(RESOLUTION));
  this->_offsetX->texParameteri(GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  this->_offsetX->texParameteri(GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
  this->_offsetX->texParameteri(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  this->_offsetX->texParameteri(GL_TEXTURE_MAG_FILTER,GL_LINEAR);

  this->_offsetY = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32F,1,GETUINT(RESOLUTION),GETUINT(RESOLUTION));
  this->_offsetY->texParameteri(GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  this->_offsetY->texParameteri(GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
  this->_offsetY->texParameteri(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  this->_offsetY->texParameteri(GL_TEXTURE_MAG_FILTER,GL_LINEAR);


  this->_offsetProgram = new  ge::gl::ProgramObject(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/offset.comp",
      this->_simulationData->define("nv.program.OFFSET"));

  this->_smoothX = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32F,1,GETUINT(RESOLUTION),GETUINT(RESOLUTION));
  this->_smoothX->texParameteri(GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  this->_smoothX->texParameteri(GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
  this->_smoothX->texParameteri(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  this->_smoothX->texParameteri(GL_TEXTURE_MAG_FILTER,GL_LINEAR);

  this->_smoothY = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32F,1,GETUINT(RESOLUTION),GETUINT(RESOLUTION));
  this->_smoothY->texParameteri(GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  this->_smoothY->texParameteri(GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
  this->_smoothY->texParameteri(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  this->_smoothY->texParameteri(GL_TEXTURE_MAG_FILTER,GL_LINEAR);


  this->_smoothProgram = new  ge::gl::ProgramObject(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/smooth.comp",
      this->_simulationData->define("nv.program.SMOOTH"));

  this->_unwarpProgram = new ge::gl::ProgramObject(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/unwarp.comp",
      this->_simulationData->define("nv.program.COUNTMAP")+
      ge::gl::ShaderObject::include(GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/dv.vp")+
      ge::gl::ShaderObject::include(GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/nv.vp"));

  this->_drawGridProgram = new ge::gl::ProgramObject(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/drawgrid.vp",
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/drawgrid.cp",
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/drawgrid.ep",
      ge::gl::ShaderObject::include(GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/dv.vp")+
      ge::gl::ShaderObject::include(GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/nv.vp"),
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/drawgrid.fp");

  this->_uall = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32UI,1,GETUINT(RESOLUTION),GETUINT(RESOLUTION));
  this->_uallProgram = new ge::gl::ProgramObject(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/uall.comp",
      this->_simulationData->define("nv.program.COUNTMAP")+
      ge::gl::ShaderObject::include(GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/dv.vp")+
      ge::gl::ShaderObject::include(GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/nv.vp"));






}

NavyMapping::~NavyMapping(){
  this->_simulationData->unregisterUser(this);
  delete this->_csm;
  delete this->_createShadowMask;
  delete this->_shadowMap;
  delete this->_fbo;
  delete this->_shadowMaskFBO;
  delete this->_emptyVAO;
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

void NavyMapping::CreateShadowMap(){
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

void NavyMapping::setMatrices(glm::mat4 lp,glm::mat4 lv){
  this->_lightView       = lv;
  this->_lightProjection = lp;
  this->_bpv=
    glm::scale(glm::mat4(1.),glm::vec3(.5))*
    glm::translate(glm::mat4(1.),glm::vec3(1.))*
    this->_lightProjection*
    this->_lightView;
}

ge::gl::TextureObject*NavyMapping::getShadowMap(){
  return this->_shadowMap;
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
  this->_viewSamplesProgram->set("mvp",1,GL_FALSE,glm::value_ptr(mvp));
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

  this->_fastdvProgram->use();
  for(unsigned i=0;i<this->_dvsTex.size()-1;++i){
    this->_dvsTex[i+0]->bindImage(0,0);
    this->_dvsTex[i+1]->bindImage(1,0);
    this->_fastdvProgram->set("windowSize",this->_dvsWorkSize[i].x,this->_dvsWorkSize[i].y);
    glDispatchCompute(this->_dvsWorkSize[i+1].x,this->_dvsWorkSize[i+1].y,1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  }
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

  unsigned workSizex=winSize.x/GETUINT(DV_SIZE_X)+1;
  unsigned workSizey=winSize.y/GETUINT(DV_SIZE_Y)+1;
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
  this->_dvsTex[this->_dvsTex.size()-1]->bindImage(2,0);
  this->_smoothX->bind(GL_TEXTURE3);
  this->_unwarpProgram->use();
  this->_unwarpProgram->set("shadowMapSize",GETUINT(RESOLUTION));
  this->_unwarpProgram->set("windowSize",winSize.x,winSize.y);

  unsigned workSizex=winSize.x/GETUINT(DV_SIZE_X)+1;
  unsigned workSizey=winSize.y/GETUINT(DV_SIZE_Y)+1;
  glDispatchCompute(workSizex,workSizey,1);
}

void NavyMapping::_unwarpAll(){
  glm::uvec2 winSize=GETUVEC2(WINDOWSIZE);

  unsigned data[]={1};

  glClearTexImage(this->_uall->getId(),0,GL_RED_INTEGER,GL_UNSIGNED_INT,data);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  this->_viewSamples                   ->bindImage(0,0);
  this->_uall                          ->bindImage(1,0);
  this->_dvsTex[this->_dvsTex.size()-1]->bindImage(2,0);
  this->_smoothX->bind(GL_TEXTURE3);
  this->_smoothY->bind(GL_TEXTURE4);
  this->_uallProgram->use();
  this->_uallProgram->set("shadowMapSize",GETUINT(RESOLUTION));
  this->_uallProgram->set("windowSize",winSize.x,winSize.y);

  unsigned workSizex=winSize.x/GETUINT(DV_SIZE_X)+1;
  unsigned workSizey=winSize.y/GETUINT(DV_SIZE_Y)+1;
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

  this->_dvsTex[this->_dvsTex.size()-1]->bindImage(2,0);
  this->_smoothX->bind(GL_TEXTURE3);
  this->_smoothY->bind(GL_TEXTURE4);
  this->_emptyVAO->bind();
  glPatchParameteri(GL_PATCH_VERTICES,1);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glDrawArrays(GL_PATCHES,0,1);
  this->_emptyVAO->unbind();
  glViewport(0,0,GETUVEC2(WINDOWSIZE).x,GETUVEC2(WINDOWSIZE).y);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}

