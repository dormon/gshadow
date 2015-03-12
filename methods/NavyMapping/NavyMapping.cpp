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
  "nv.program.DV.WORKGROUP_SIZE_X",
  "nv.program.DV.WORKGROUP_SIZE_Y",
  "nv.program.COUNTMAP.WORKGROUP_SIZE_X",
  "nv.program.COUNTMAP.WORKGROUP_SIZE_Y",
  "nv.program.INTEGRATE.WORKGROUP_SIZE_X",
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
  DV_SIZE_X,
  DV_SIZE_Y,
  COUNTMAP_SIZE_X,
  COUNTMAP_SIZE_Y,
  INTEGRATE_SIZE_X,
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
  this->_createCountMap();
  this->_integrate(this->_integratedCountMap,this->_integratedCountMapCount,this->_countMap,true);

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
  this->_dvTex = new ge::gl::TextureObject(GL_TEXTURE_1D,GL_R32UI,1,4);
  this->_dvProgram = new ge::gl::ProgramObject(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/createdv.comp",
      this->_simulationData->define("nv.program.DV"));
  this->_dvClearProgram = new ge::gl::ProgramObject(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/cleardv.comp",
      this->_simulationData->define("nv.program.DV"));

  this->_countMap = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32UI,1,GETUINT(RESOLUTION),GETUINT(RESOLUTION));
  this->_createCountMapProgram = new ge::gl::ProgramObject(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/createCountMap.comp",
      this->_simulationData->define("nv.program.COUNTMAP"));

  this->_integratedCountMap = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32UI,1,GETUINT(RESOLUTION),GETUINT(RESOLUTION));
  this->_integratedCountMapCount = new ge::gl::TextureObject(GL_TEXTURE_1D,GL_R32UI,1,GETUINT(RESOLUTION));
  this->_integrateProgram = new ge::gl::ProgramObject(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/integrate.comp",
      this->_simulationData->define("nv.program.INTEGRATE"));

  this->_isoX = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32F,1,GETUINT(RESOLUTION),GETUINT(ISO_GRID_X));
  this->_createIsoProgram = new ge::gl::ProgramObject(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/iso.comp",
      this->_simulationData->define("nv.program.ISO"));






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

void NavyMapping::_createDV(){
  glm::uvec2 winSize=GETUVEC2(WINDOWSIZE);

  this->_dvClearProgram->use();
  this->_dvClearProgram->set("shadowMapSize",GETUINT(RESOLUTION));
  this->_dvTex->bindImage(1,0);
  glDispatchCompute(1,1,1);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  GETTEXTURE(GBUFFER_POSITION)->bind(GL_TEXTURE0);
  this->_dvTex->bindImage(1,0);
  this->_dvProgram->use();
  this->_dvProgram->set("shadowMapSize",GETUINT(RESOLUTION));
  this->_dvProgram->set("windowSize",winSize.x,winSize.y);
  this->_dvProgram->set("bpv",1,GL_FALSE,glm::value_ptr(this->_bpv));
  unsigned workSizex=winSize.x/GETUINT(DV_SIZE_X)+1;
  unsigned workSizey=winSize.y/GETUINT(DV_SIZE_Y)+1;
  glDispatchCompute(workSizex,workSizey,1);

  /*
     unsigned data[4]={0,0,0,0};
     glBindTexture(GL_TEXTURE_1D,this->_dvTex->getId());
     glGetTexImage(GL_TEXTURE_1D,0,GL_RED_INTEGER,GL_UNSIGNED_INT,data);
     std::cerr<<"data0: "<<data[0]<<std::endl;
     std::cerr<<"data1: "<<data[1]<<std::endl;
     std::cerr<<"data2: "<<data[2]<<std::endl;
     std::cerr<<"data3: "<<data[3]<<std::endl;
     */

}

void NavyMapping::_createCountMap(){
  glm::uvec2 winSize=GETUVEC2(WINDOWSIZE);

  glClearTexImage(this->_countMap->getId(),0,GL_RED_INTEGER,GL_UNSIGNED_INT,NULL);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  GETTEXTURE(GBUFFER_POSITION)->bind(GL_TEXTURE0);
  this->_countMap->bindImage(1,0);
  this->_createCountMapProgram->use();
  this->_createCountMapProgram->set("shadowMapSize",GETUINT(RESOLUTION));
  this->_createCountMapProgram->set("windowSize",winSize.x,winSize.y);
  this->_createCountMapProgram->set("bpv",1,GL_FALSE,glm::value_ptr(this->_bpv));
  unsigned workSizex=winSize.x/GETUINT(DV_SIZE_X)+1;
  unsigned workSizey=winSize.y/GETUINT(DV_SIZE_Y)+1;
  glDispatchCompute(workSizex,workSizey,1);

}

void NavyMapping::_integrate(
    ge::gl::TextureObject*integral,
    ge::gl::TextureObject*integralCount,
    ge::gl::TextureObject*countMap,
    bool xDirection){
  countMap     ->bindImage(0,0);
  integral     ->bindImage(1,0);
  integralCount->bindImage(2,0);
  this->_integrateProgram->use();
  this->_integrateProgram->set("shadowMapSize",GETUINT(RESOLUTION));
  this->_integrateProgram->set("xAxis",xDirection);
  unsigned workSizex=GETUINT(RESOLUTION)/GETUINT(INTEGRATE_SIZE_X)+1;
  glDispatchCompute(workSizex,1,1);
}

void NavyMapping::_createIso(
    ge::gl::TextureObject*iso,
    ge::gl::TextureObject*integral,
    ge::gl::TextureObject*integralCount,
    bool xDirection){

}




/*
#define DRAWCM_BINDING_COUNTMAP 0
#define WRITECM_BINDING_POSITIONMAP 0
#define WRITECM_BINDING_COUNTMAP 1

#define PREFIXSUM_BINDING_COUNTMAP     0
#define PREFIXSUM_BINDING_PREFIXSUMMAP 1
#define PREFIXSUM_BINDING_GRID         2
#define PREFIXSUM_WORKGROUP_SIZE_X 64


#define DRAWPSM_BINDING_PREFIXSUMMAP 0

#define DRAWGRID_BINDING_SMOOTH 0

#define GRID_SIZE_X 4
#define GRID_SIZE_Y 4

#define UNWARPX_BINDING_POSITIONMAP     0
#define UNWARPX_BINDING_UNWARPXCOUNTMAP 1
#define UNWARPX_BINDING_SMOOTHEDGRIDX   2

#define DRAWUNWARPX_BINDING_UNWARPXCOUNTMAP 0

#define SMOOTHGRID_BINDING_GRID     0
#define SMOOTHGRID_BINDING_SMOOTH   1
#define SMOOTHGRID_WORKGROUP_SIZE_X 64
#define SMOOTHGRID_WORKGROUP_SIZE_Y (GRID_SIZE_X+1)

#define CREATENAVYMAP_BINDING_GRIDX 0
#define CREATENAVYMAP_BINDING_GRIDY 1

#define NAVYMAPPING_BINDING_GRIDX 6
#define NAVYMAPPING_BINDING_GRIDY 7


#define WALKING_WINDOW_SIZE 100

NavyMapping::NavyMapping(
unsigned Size,
unsigned*ScreenSize,
ge::gl::VertexArrayObject*sceneVAO,
SAdjecency*Adjacency){
std::cerr<<"AHOJ CONSTRUCTOR SHADOWMAPPING\n";
this->sceneVAO=sceneVAO;
this->ShadowMapSize=Size;
this->ScreenSize[0]=ScreenSize[0];
this->ScreenSize[1]=ScreenSize[1];
this->Adjacency=Adjacency;
float ones[]={1,1,1,1};
std::cerr<<"ShadowMapSize: "<<this->ShadowMapSize<<std::endl;
std::cerr<<"END"<<std::endl;

this->shadowMap=new ge::gl::TextureObject(GL_TEXTURE_2D,GL_DEPTH_COMPONENT24,1,
this->ShadowMapSize,this->ShadowMapSize);
this->shadowMap->texParameteri (GL_TEXTURE_MIN_FILTER  ,GL_NEAREST             );
this->shadowMap->texParameteri (GL_TEXTURE_MAG_FILTER  ,GL_NEAREST             );
this->shadowMap->texParameteri (GL_TEXTURE_WRAP_S      ,GL_CLAMP_TO_BORDER     );
this->shadowMap->texParameteri (GL_TEXTURE_WRAP_T      ,GL_CLAMP_TO_BORDER     );
this->shadowMap->texParameteri (GL_TEXTURE_COMPARE_FUNC,GL_LEQUAL              );
this->shadowMap->texParameteri (GL_TEXTURE_COMPARE_MODE,GL_COMPARE_R_TO_TEXTURE);
this->shadowMap->texParameterfv(GL_TEXTURE_BORDER_COLOR,ones                   );

this->fbo=new ge::gl::FramebufferObject();
this->fbo->attachDepthTexture(this->shadowMap->getId());

this->navyMap=new ge::gl::TextureObject(GL_TEXTURE_2D,GL_DEPTH_COMPONENT24,1,
this->ShadowMapSize,this->ShadowMapSize);
this->navyMap->texParameteri (GL_TEXTURE_MIN_FILTER  ,GL_NEAREST             );
this->navyMap->texParameteri (GL_TEXTURE_MAG_FILTER  ,GL_NEAREST             );
this->navyMap->texParameteri (GL_TEXTURE_WRAP_S      ,GL_CLAMP_TO_BORDER     );
this->navyMap->texParameteri (GL_TEXTURE_WRAP_T      ,GL_CLAMP_TO_BORDER     );
this->navyMap->texParameteri (GL_TEXTURE_COMPARE_FUNC,GL_LEQUAL              );
this->navyMap->texParameteri (GL_TEXTURE_COMPARE_MODE,GL_COMPARE_R_TO_TEXTURE);
this->navyMap->texParameterfv(GL_TEXTURE_BORDER_COLOR,ones                   );

this->nfbo=new ge::gl::FramebufferObject();
this->nfbo->attachDepthTexture(this->navyMap->getId());

this->CSM = new ge::gl::ProgramObject(
    ShaderDir+"methods/ShadowMapping/createshadowmap.vp",
    ShaderDir+"methods/ShadowMapping/createshadowmap.fp");

this->CNM = new ge::gl::ProgramObject(
    ShaderDir+"methods/NavyMapping/createNavyMap.vp",
    ge::gl::ShaderObject::define("SHADOWMAP_SIZE",(int)this->ShadowMapSize   )+
    ge::gl::ShaderObject::define("BINDING_GRIDX" ,CREATENAVYMAP_BINDING_GRIDX)+
    ge::gl::ShaderObject::define("BINDING_GRIDY" ,CREATENAVYMAP_BINDING_GRIDY)+
    ge::gl::ShaderObject::define("GRID_SIZE_X"   ,GRID_SIZE_X                )+
    ge::gl::ShaderObject::define("GRID_SIZE_Y"   ,GRID_SIZE_Y                )+
    "",
    ShaderDir+"methods/NavyMapping/createNavyMap.fp",
    "");

this->SM = new ge::gl::ProgramObject(
    ShaderDir+"methods/ShadowMapping/shadowmapping.vp",
    ShaderDir+"methods/ShadowMapping/shadowmapping.gp",
    ShaderDir+"methods/ShadowMapping/shadowmapping.fp");

try{
  this->NSM = new ge::gl::ProgramObject(
      ShaderDir+"methods/NavyMapping/navyMapping.vp",
      ShaderDir+"methods/NavyMapping/navyMapping.gp",
      ShaderDir+"methods/NavyMapping/navyMapping.fp",
      ge::gl::ShaderObject::define("SHADOWMAP_SIZE",(int)this->ShadowMapSize )+
      ge::gl::ShaderObject::define("BINDING_GRIDX" ,NAVYMAPPING_BINDING_GRIDX)+
      ge::gl::ShaderObject::define("BINDING_GRIDY" ,NAVYMAPPING_BINDING_GRIDY)+
      ge::gl::ShaderObject::define("GRID_SIZE_X"   ,GRID_SIZE_X              )+
      ge::gl::ShaderObject::define("GRID_SIZE_Y"   ,GRID_SIZE_Y              )+
      "");
}catch(std::string&e){
  std::cerr<<e<<std::endl;
}


this->DSM = new ge::gl::ProgramObject(
    ShaderDir+"methods/ShadowMapping/drawsm.vp",
    ShaderDir+"methods/ShadowMapping/drawsm.gp",
    ShaderDir+"methods/ShadowMapping/drawsm.fp");

this->WCM = new ge::gl::ProgramObject(
    ShaderDir+"methods/NavyMapping/writecm.vp",
    ShaderDir+"methods/NavyMapping/writecm.gp",
    ShaderDir+"methods/NavyMapping/writecm.fp",
    ge::gl::ShaderObject::define("WRITECM_BINDING_POSITIONMAP",WRITECM_BINDING_POSITIONMAP)+
    ge::gl::ShaderObject::define("WRITECM_BINDING_COUNTMAP"   ,WRITECM_BINDING_COUNTMAP   )+
    "");
std::cerr<<"jojo\n";

this->prefixSumXProgram = new ge::gl::ProgramObject(
    ShaderDir+"methods/NavyMapping/prefixSum.comp",
    ge::gl::ShaderObject::define("PREFIXSUM_BINDING_COUNTMAP"    ,PREFIXSUM_BINDING_COUNTMAP    )+
    ge::gl::ShaderObject::define("PREFIXSUM_BINDING_PREFIXSUMMAP",PREFIXSUM_BINDING_PREFIXSUMMAP)+
    ge::gl::ShaderObject::define("PREFIXSUM_BINDING_GRID"        ,PREFIXSUM_BINDING_GRID        )+
    ge::gl::ShaderObject::define("SHADOWMAP_SIZE"                ,(int)this->ShadowMapSize      )+
    ge::gl::ShaderObject::define("WORKGROUP_SIZE"                ,PREFIXSUM_WORKGROUP_SIZE_X    )+
    "");

this->prefixSumProgram = new ge::gl::ProgramObject(
    ShaderDir+"methods/NavyMapping/prefixSum.comp",
    ge::gl::ShaderObject::define("PREFIXSUM_BINDING_COUNTMAP"    ,PREFIXSUM_BINDING_COUNTMAP    )+
    ge::gl::ShaderObject::define("PREFIXSUM_BINDING_PREFIXSUMMAP",PREFIXSUM_BINDING_PREFIXSUMMAP)+
    ge::gl::ShaderObject::define("PREFIXSUM_BINDING_GRID"        ,PREFIXSUM_BINDING_GRID        )+
    ge::gl::ShaderObject::define("SHADOWMAP_SIZE"                ,(int)this->ShadowMapSize      )+
    ge::gl::ShaderObject::define("WORKGROUP_SIZE"                ,PREFIXSUM_WORKGROUP_SIZE_X    )+
    "");



this->DPSM = new ge::gl::ProgramObject(
    ShaderDir+"methods/NavyMapping/drawpsm.vp",
    ShaderDir+"methods/NavyMapping/drawpsm.gp",
    ShaderDir+"methods/NavyMapping/drawpsm.fp",
    ge::gl::ShaderObject::define("DRAWPSM_BINDING_PREFIXSUMMAP",(int)DRAWPSM_BINDING_PREFIXSUMMAP)+
    ge::gl::ShaderObject::define("SHADOWMAP_SIZE"              ,(int)this->ShadowMapSize         )+
    "");

this->drawGridProgram = new ge::gl::ProgramObject(
    ShaderDir+"methods/NavyMapping/drawGrid.vp",
    ge::gl::ShaderObject::define("SHADOWMAP_SIZE",(int)this->ShadowMapSize)+
    ge::gl::ShaderObject::define("GRID_SIZE_X"   ,GRID_SIZE_X             )+
    ge::gl::ShaderObject::define("BINDING_SMOOTH",DRAWGRID_BINDING_SMOOTH )+
    "",
    ShaderDir+"methods/NavyMapping/drawGrid.fp",
    "");

std::cerr<<"jojasdo\n";
this->smoothGridProgram = new ge::gl::ProgramObject(
    ShaderDir+"methods/NavyMapping/smoothGrid.comp",
    ge::gl::ShaderObject::define("BINDING_GRID"       ,SMOOTHGRID_BINDING_GRID    )+
    ge::gl::ShaderObject::define("BINDING_SMOOTH"     ,SMOOTHGRID_BINDING_SMOOTH  )+
    ge::gl::ShaderObject::define("SHADOWMAP_SIZE"     ,(int)this->ShadowMapSize   )+
    ge::gl::ShaderObject::define("WORKGROUP_SIZE_X"   ,SMOOTHGRID_WORKGROUP_SIZE_X)+
    ge::gl::ShaderObject::define("WALKING_WINDOW_SIZE",WALKING_WINDOW_SIZE        )+
    "");

std::cerr<<"jojo2\n";
this->unwarpXProgram = new ge::gl::ProgramObject(
    ShaderDir+"methods/NavyMapping/unwarpx.vp",
    ShaderDir+"methods/NavyMapping/unwarpx.gp",
    ShaderDir+"methods/NavyMapping/unwarpx.fp",
    ge::gl::ShaderObject::define("UNWARPX_BINDING_POSITIONMAP"    ,UNWARPX_BINDING_POSITIONMAP    )+
    ge::gl::ShaderObject::define("UNWARPX_BINDING_SMOOTHEDGRIDX"  ,UNWARPX_BINDING_SMOOTHEDGRIDX  )+
    ge::gl::ShaderObject::define("UNWARPX_BINDING_UNWARPXCOUNTMAP",UNWARPX_BINDING_UNWARPXCOUNTMAP)+
    ge::gl::ShaderObject::define("GRID_SIZE_X"                    ,GRID_SIZE_X                    )+
    ge::gl::ShaderObject::define("SHADOWMAP_SIZE"                 ,(int)this->ShadowMapSize       )+
    "");

std::cerr<<"jojoasssdsd\n";
this->countMap = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32UI,1,
    this->ShadowMapSize,this->ShadowMapSize);
this->countMap->texParameteri(GL_TEXTURE_MAG_FILTER,GL_NEAREST);
this->countMap->texParameteri(GL_TEXTURE_MIN_FILTER,GL_NEAREST);

this->prefixSumXMap = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32UI,1,
    this->ShadowMapSize,this->ShadowMapSize);
this->prefixSumXMap->texParameteri(GL_TEXTURE_MAG_FILTER,GL_NEAREST);
this->prefixSumXMap->texParameteri(GL_TEXTURE_MIN_FILTER,GL_NEAREST);

this->prefixSumYMap = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32UI,1,
    this->ShadowMapSize,this->ShadowMapSize);
this->prefixSumYMap->texParameteri(GL_TEXTURE_MAG_FILTER,GL_NEAREST);
this->prefixSumYMap->texParameteri(GL_TEXTURE_MIN_FILTER,GL_NEAREST);


this->gridX = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_RG32UI,1,this->ShadowMapSize,GRID_SIZE_X+1);
this->gridX->texParameteri(GL_TEXTURE_MAG_FILTER,GL_NEAREST);
this->gridX->texParameteri(GL_TEXTURE_MIN_FILTER,GL_NEAREST);

this->gridY = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_RG32UI,1,this->ShadowMapSize,GRID_SIZE_Y+1);
this->gridY->texParameteri(GL_TEXTURE_MAG_FILTER,GL_NEAREST);
this->gridY->texParameteri(GL_TEXTURE_MIN_FILTER,GL_NEAREST);


this->unwarpXCountMap = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32UI,1,this->ShadowMapSize,this->ShadowMapSize);
this->unwarpXCountMap->texParameteri(GL_TEXTURE_MAG_FILTER,GL_NEAREST);
this->unwarpXCountMap->texParameteri(GL_TEXTURE_MIN_FILTER,GL_NEAREST);

this->smoothedGridX = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32F,1,this->ShadowMapSize,GRID_SIZE_X+1);
this->smoothedGridX->texParameteri(GL_TEXTURE_MAG_FILTER,GL_NEAREST);
this->smoothedGridX->texParameteri(GL_TEXTURE_MIN_FILTER,GL_NEAREST);

this->smoothedGridY = new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32F,1,this->ShadowMapSize,GRID_SIZE_Y+1);
this->smoothedGridY->texParameteri(GL_TEXTURE_MAG_FILTER,GL_NEAREST);
this->smoothedGridY->texParameteri(GL_TEXTURE_MIN_FILTER,GL_NEAREST);


glGenVertexArrays(1,&this->EmptyVAO);
glBindVertexArray(this->EmptyVAO);
glBindVertexArray(0);
}
ge::gl::TextureObject*NavyMapping::getCountMap(){
  return this->countMap;
}

NavyMapping::~NavyMapping(){
  glDeleteVertexArrays(1,&this->EmptyVAO);
  delete this->fbo;
  delete this->shadowMap;
  delete this->SM;
  delete this->CSM;
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

void NavyMapping::setFocus(simulation::Light*Light,float*FocusPoint,float Fovy){
  this->_lightProjection=glm::perspective(Fovy,1.f,1.f,10000.f);
  this->_lightView=glm::lookAt(
      glm::vec3(Light->position),
      glm::vec3(FocusPoint[0],FocusPoint[1],FocusPoint[2]),
      glm::vec3(0.f,1.f,0.f));
  this->_bpv=
    glm::scale(glm::mat4(1.),glm::vec3(.5))*
    glm::translate(glm::mat4(1.),glm::vec3(1.))*
    this->_lightProjection*
    this->_lightView;
}

void NavyMapping::createShadowMap(float*M){
  this->CSM->use();
  CSM->set("m",1,GL_FALSE,M);
  CSM->set("v",1,GL_FALSE,(float*)glm::value_ptr(this->_lightView));
  CSM->set("p",1,GL_FALSE,(float*)glm::value_ptr(this->_lightProjection));

  //glBindFramebuffer(GL_FRAMEBUFFER,this->FBO);
  this->fbo->bind();
  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
  glViewport(0,0,this->ShadowMapSize,this->ShadowMapSize);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);
  glClear(GL_DEPTH_BUFFER_BIT);

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(5.5,10);

  this->sceneVAO->bind();
  glDrawArrays(GL_TRIANGLES,0,this->Adjacency->NumTriangles*3);
  this->sceneVAO->unbind();

  glViewport(0,0,this->ScreenSize[0],this->ScreenSize[1]);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glDisable(GL_POLYGON_OFFSET_FILL);
  //glBindFramebuffer(GL_FRAMEBUFFER,0);
  this->fbo->unbind();
}

void NavyMapping::createNavyMap(float*M){
  this->CNM->use();
  this->CNM->set("m",1,GL_FALSE,M);
  this->CNM->set("v",1,GL_FALSE,(float*)glm::value_ptr(this->_lightView));
  this->CNM->set("p",1,GL_FALSE,(float*)glm::value_ptr(this->_lightProjection));
  this->smoothedGridX->bindImage(CREATENAVYMAP_BINDING_GRIDX,0);
  this->smoothedGridY->bindImage(CREATENAVYMAP_BINDING_GRIDY,0);

  //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  this->nfbo->bind();
  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
  glViewport(0,0,this->ShadowMapSize,this->ShadowMapSize);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);
  glClear(GL_DEPTH_BUFFER_BIT);

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(5.5,10);

  this->sceneVAO->bind();
  glDrawArrays(GL_TRIANGLES,0,this->Adjacency->NumTriangles*3);
  this->sceneVAO->unbind();

  glViewport(0,0,this->ScreenSize[0],this->ScreenSize[1]);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glDisable(GL_POLYGON_OFFSET_FILL);
  //glBindFramebuffer(GL_FRAMEBUFFER,0);
  this->nfbo->unbind();
  //glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

}


void NavyMapping::drawShadowed(float*Pos,simulation::Light*Light){
  this->shadowMap->bind(GL_TEXTURE5);
  this->SM->use();
  this->SM->set("La",0.f,0.f,0.f);
  this->SM->set("Ld",1,glm::value_ptr(Light->diffuse));
  this->SM->set("Ls",1,glm::value_ptr(Light->specular));
  this->SM->set("LightPosition",1,glm::value_ptr(Light->position));
  this->SM->set("BPV",1,GL_FALSE,glm::value_ptr(this->_bpv));
  this->SM->set("CameraPosition",-Pos[0],-Pos[1],-Pos[2]);

  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE,GL_ONE);
  glBlendEquation(GL_FUNC_ADD);
  glDepthFunc(GL_ALWAYS);

  glBindVertexArray(this->EmptyVAO);
  glDrawArrays(GL_POINTS,0,1);
  glBindVertexArray(0);


  glDepthFunc(GL_LESS);
  glDisable(GL_STENCIL_TEST);
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);

}

void NavyMapping::drawNavyShadowed(bool wrap,float*pos,simulation::Light*light){
  this->navyMap->bind(GL_TEXTURE5);
  this->smoothedGridX->bindImage(NAVYMAPPING_BINDING_GRIDX,0);
  this->smoothedGridY->bindImage(NAVYMAPPING_BINDING_GRIDY,0);
  this->NSM->use();
  this->NSM->set("La",0.f,0.f,0.f);
  this->NSM->set("Ld",1,glm::value_ptr(light->diffuse));
  this->NSM->set("Ls",1,glm::value_ptr(light->specular));
  this->NSM->set("LightPosition",1,glm::value_ptr(light->position));
  this->NSM->set("BPV",1,GL_FALSE,glm::value_ptr(this->_bpv));
  this->NSM->set("v",1,GL_FALSE,(float*)glm::value_ptr(this->_lightView));
  this->NSM->set("p",1,GL_FALSE,(float*)glm::value_ptr(this->_lightProjection));


  this->NSM->set("CameraPosition",-pos[0],-pos[1],-pos[2]);
  this->NSM->set("wrap",wrap);

  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE,GL_ONE);
  glBlendEquation(GL_FUNC_ADD);
  glDepthFunc(GL_ALWAYS);

  glBindVertexArray(this->EmptyVAO);
  glDrawArrays(GL_POINTS,0,1);
  glBindVertexArray(0);

  glDepthFunc(GL_LESS);
  glDisable(GL_STENCIL_TEST);
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
}


void NavyMapping::drawShadowMap(bool navyMap,float x,float y,float w){
  glViewport(
      x*this->ScreenSize[0],
      y*this->ScreenSize[1],
      w*this->ScreenSize[0],
      w*this->ScreenSize[1]);
  glBindVertexArray(this->EmptyVAO);
  this->DSM->use();
  if(navyMap)this->navyMap->bind(GL_TEXTURE9);
  else this->shadowMap->bind(GL_TEXTURE9);
  glDrawArrays(GL_POINTS,0,1);
  glBindVertexArray(0);
  glViewport(0,0,this->ScreenSize[0],this->ScreenSize[1]);
}

void NavyMapping::writeViewSamples(GLuint positionTexture){

  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
  glDepthMask(GL_FALSE);
  glClearTexImage(this->countMap->getId(),0,GL_RED_INTEGER,GL_UNSIGNED_INT,NULL);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  glViewport(0,0,this->ScreenSize[0],this->ScreenSize[1]);
  glBindVertexArray(this->EmptyVAO);
  this->WCM->use();
  this->WCM->set("BPV",1,GL_FALSE,glm::value_ptr(this->_bpv));
  this->WCM->set("shadowMapSize",(unsigned)this->ShadowMapSize);
  this->countMap->bindImage(WRITECM_BINDING_COUNTMAP,0);
  glActiveTexture(GL_TEXTURE0+WRITECM_BINDING_POSITIONMAP);
  glBindTexture(GL_TEXTURE_2D,positionTexture);
  glDrawArrays(GL_POINTS,0,1);
  glBindVertexArray(0);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  glDepthMask(GL_TRUE);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);

}

void NavyMapping::prefixSum(){
  glClearTexImage(this->prefixSumXMap->getId(),0,GL_RED_INTEGER,GL_UNSIGNED_INT,NULL);
  glClearTexImage(this->gridX        ->getId(),0,GL_RED_INTEGER,GL_UNSIGNED_INT,NULL);
  glMemoryBarrier(GL_ALL_BARRIER_BITS);
  this->prefixSumXProgram->use();
  this->countMap     ->bindImage(PREFIXSUM_BINDING_COUNTMAP    ,0);
  this->prefixSumXMap->bindImage(PREFIXSUM_BINDING_PREFIXSUMMAP,0);
  this->gridX        ->bindImage(PREFIXSUM_BINDING_GRID        ,0);
  glDispatchCompute(this->ShadowMapSize/PREFIXSUM_WORKGROUP_SIZE_X,1,1);
  glMemoryBarrier(GL_ALL_BARRIER_BITS);

}

void NavyMapping::drawPrefixSum(float x,float y,float w){
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glViewport(
      x*this->ScreenSize[0],
      y*this->ScreenSize[1],
      w*this->ScreenSize[0],
      w*this->ScreenSize[1]);
  glBindVertexArray(this->EmptyVAO);
  this->DPSM->use();
  this->prefixSumXMap->bindImage(DRAWPSM_BINDING_PREFIXSUMMAP,0);
  glDrawArrays(GL_POINTS,0,1);
  glBindVertexArray(0);
  glViewport(0,0,this->ScreenSize[0],this->ScreenSize[1]);
}

void NavyMapping::smoothGrid(ge::gl::TextureObject*smooth,ge::gl::TextureObject*grid){
  glClearTexImage(smooth->getId(),0,GL_RED,GL_FLOAT,NULL);
  glMemoryBarrier(GL_ALL_BARRIER_BITS);
  glFinish();
  grid  ->bindImage(SMOOTHGRID_BINDING_GRID  ,0);
  smooth->bindImage(SMOOTHGRID_BINDING_SMOOTH,0);
  glDispatchCompute(this->ShadowMapSize/SMOOTHGRID_WORKGROUP_SIZE_X,GRID_SIZE_X+1,1);
  glMemoryBarrier(GL_ALL_BARRIER_BITS);
  glFinish();
}
void NavyMapping::smoothGrid(bool xAxis){
  this->smoothGridProgram->use();
  //this->smoothGridProgram->set("gridSize",GRID_SIZE_X*(xAxis)+GRID_SIZE_Y*(!xAxis)+1);
  if(xAxis)this->smoothGrid(this->smoothedGridX,this->gridX);
  else     this->smoothGrid(this->smoothedGridY,this->gridY);
}

void NavyMapping::drawGrid(float x,float y,float w,bool xAxe){
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glViewport(
      x*this->ScreenSize[0],
      y*this->ScreenSize[1],
      w*this->ScreenSize[0],
      w*this->ScreenSize[1]);
  glBindVertexArray(this->EmptyVAO);

  this->drawGridProgram->use();
  this->drawGridProgram->set("xAxe",xAxe);
  if(xAxe)this->smoothedGridX->bindImage(DRAWGRID_BINDING_SMOOTH,0);
  else    this->smoothedGridY->bindImage(DRAWGRID_BINDING_SMOOTH,0);

  for(int l=0;l<=GRID_SIZE_X*(xAxe)+GRID_SIZE_Y*(!xAxe);++l){
    this->drawGridProgram->set("lineId",l);
    glDrawArrays(GL_LINE_STRIP,0,this->ShadowMapSize);
  }
  glBindVertexArray(0);
  glViewport(0,0,this->ScreenSize[0],this->ScreenSize[1]);
}

void NavyMapping::unwarpX(GLuint positionTexture){
  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
  glDepthMask(GL_FALSE);
  glClearTexImage(this->unwarpXCountMap->getId(),0,GL_RED_INTEGER,GL_UNSIGNED_INT,NULL);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  glViewport(0,0,this->ScreenSize[0],this->ScreenSize[1]);
  glBindVertexArray(this->EmptyVAO);
  this->unwarpXProgram->use();
  this->unwarpXProgram->set("BPV",1,GL_FALSE,glm::value_ptr(this->_bpv));
  this->unwarpXCountMap->bindImage(UNWARPX_BINDING_UNWARPXCOUNTMAP,0);
  this->smoothedGridX  ->bindImage(UNWARPX_BINDING_SMOOTHEDGRIDX  ,0);
  glActiveTexture(GL_TEXTURE0+UNWARPX_BINDING_POSITIONMAP);
  glBindTexture(GL_TEXTURE_2D,positionTexture);
  glDrawArrays(GL_POINTS,0,1);
  glBindVertexArray(0);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  glDepthMask(GL_TRUE);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
}


void NavyMapping::prefixSum(ge::gl::TextureObject*grid,ge::gl::TextureObject*prefix,ge::gl::TextureObject*count){
  glClearTexImage(grid  ->getId(),0,GL_RED_INTEGER,GL_UNSIGNED_INT,NULL);
  glClearTexImage(prefix->getId(),0,GL_RED_INTEGER,GL_UNSIGNED_INT,NULL);
  glMemoryBarrier(GL_ALL_BARRIER_BITS);
  this->prefixSumProgram->use();
  count ->bindImage(PREFIXSUM_BINDING_COUNTMAP    ,0);
  prefix->bindImage(PREFIXSUM_BINDING_PREFIXSUMMAP,0);
  grid  ->bindImage(PREFIXSUM_BINDING_GRID        ,0);
  glDispatchCompute(this->ShadowMapSize/PREFIXSUM_WORKGROUP_SIZE_X,1,1);
  glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

void NavyMapping::prefixSum(bool xAxis){
  if(xAxis)this->prefixSum(this->gridX,this->prefixSumXMap,this->countMap       );
  else     this->prefixSum(this->gridY,this->prefixSumYMap,this->unwarpXCountMap);
}
*/
