#include"NavyDualParaboloid.h"

#include<geCore/dtemplates.h>

#undef ___
#define ___

#include"../ShadowMapping/createsm.h"

#undef CLASSNAME
#define CLASSNAME NavyDualParaboloid
#include"../ShadowMethodMacro.h"
#include"../CubeShadowMapping/createCubeShadowMapTexture.h"

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

  "nv.program.smoothWindowSize",
  "nv.use_fast_smooth",
  "nv.program.warpFactor",
  "nv.program.NVMAP.TESS_FACTOR",
  "nv.useWarping",
  "nv.drawLinesToSM",
  "nv.cullTriangles",

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
  FACTOR,
  UNITS,
  WINDOWSIZE,
  FASTADJACENCY,
  LIGHT,
  GBUFFER_POSITION,
  SHADOWMASK,

  SMOOTH_WINDOW,
  USE_FAST_SMOOTH,
  WARP_FACTOR,
  TESS_FACTOR,
  USE_WARPING,
  LINE_TO_SM,
  CULL_TRIANGLES,

  MEASURE_CREATESHADOWMAP,
  MEASURE_CREATESHADOWMASK
DEFVARSIDEND

DEFGETNOFDEP
DEFGETDEP

DEFUPDATEROUTINESSTART
  GETPOINTER(_computeMatrices   ),
  GETPOINTER(_createShadowMap   ),
  GETPOINTER(_createShadowMaskFBO),
DEFUPDATEROUTINESEND

DEFUPDATEROUTINEIDSTART
  COMPUTEMATRICES   ,
  CREATESHADOWMAP   ,
  CREATESHADOWMASKFBO,
DEFUPDATEROUTINEIDEND

DEFVAR2UPDATESTART
  FOCUSPOINT,getMask(COMPUTEMATRICES   ),
  LIGHT     ,getMask(COMPUTEMATRICES   ),
  FOVY      ,getMask(COMPUTEMATRICES   ),
  NEAR      ,getMask(COMPUTEMATRICES   ),
  FAR       ,getMask(COMPUTEMATRICES   ),
  RESOLUTION,getMask(CREATESHADOWMAP   ),
  SHADOWMASK,getMask(CREATESHADOWMASKFBO),
DEFVAR2UPDATEEND

DEFUPDATE

void NavyDualParaboloid::_createShadowMaskFBO(){
  if(!this->_shadowMaskFBO)this->_shadowMaskFBO=new ge::gl::FramebufferObject();
  this->_shadowMaskFBO->attachColorTexture(GL_COLOR_ATTACHMENT0,GETTEXTURE(SHADOWMASK)->getId());
}



void NavyDualParaboloid::_computeMatrices(){
  glm::vec3 lp=glm::vec3(GETLIGHT->position);
  for(unsigned i=0;i<2;++i)
    this->_lightView[i]=glm::lookAt(lp,lp+glm::vec3(-1.f+2.f*i,0.f,0.f),glm::vec3(0.f,1.f,0.f));
}


NavyDualParaboloid::NavyDualParaboloid(simulation::SimulationData*data):simulation::SimulationObject(data){
  this->_simulationData->registerUser(this);
  this->_emptyVAO      = new ge::gl::VertexArrayObject();
  
  std::string dir=GETSTRING(SHADERDIRECTORY)+"methods/NavyDualParaboloid/";
  this->_createShadowMask   = new ge::gl::ProgramObject(
      dir+"createShadowMask.vp",//napsat
      dir+"createShadowMask.gp",
      dir+"createShadowMask.fp",
      ge::gl::ShaderObject::include(GETSTRING(SHADERDIRECTORY)+"methods/DualParaboloid/dpProj.vp")+
      ge::gl::ShaderObject::include(GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/dv.vp")+
      ge::gl::ShaderObject::include(GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/nv.vp")+"");
  this->_createShadowMap();
  this->_computeMatrices();
  this->_createShadowMaskFBO();

  for(unsigned i=0;i<2;++i){
    this->_desiredView[i]=createDesiredViewTexture();
    this->_smoothX    [i]=createSmoothTexture(GETUINT(RESOLUTION));
    this->_smoothY    [i]=createSmoothTexture(GETUINT(RESOLUTION));
  }

  this->_createWarping = new CreateWarping(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/",
      GETUVEC2(WINDOWSIZE).x,
      GETUVEC2(WINDOWSIZE).y,
      GETUINT(RESOLUTION),
      GETUINT(SMOOTH_WINDOW),
      GETFLOAT(WARP_FACTOR),
      GETSTRING(SHADERDIRECTORY)+"methods/DualParaboloid/dpProj.vp");

  this->_createNavyShadowMap = new CreateNavyShadowMap(
      GETSTRING(SHADERDIRECTORY)+"methods/NavyMapping/",
      GETUVEC2(WINDOWSIZE).x,
      GETUVEC2(WINDOWSIZE).y,
      GETVAO(SCENEVAO),
      GETFASTADJACENCY->getNofTriangles(),
      GETSTRING(SHADERDIRECTORY)+"methods/DualParaboloid/dpProj.vp");
}

NavyDualParaboloid::~NavyDualParaboloid(){
  delete this->_createShadowMask;
  for(unsigned side=0;side<2;++side){
    delete this->_shadowMap[side];
    delete this->_desiredView[side];
    delete this->_smoothX[side];
    delete this->_smoothY[side];
  }
  delete this->_createWarping;
  delete this->_createNavyShadowMap;
}

void NavyDualParaboloid::_createShadowMap(){
  for(unsigned i=0;i<2;++i){
    if(this->_shadowMap[i])delete this->_shadowMap[i];
    this->_shadowMap[i] = createSM(GETUINT(RESOLUTION));
  }
}

void NavyDualParaboloid::createShadowMap(){
  for(unsigned i=0;i<2;++i){
    glm::mat4 mvp=this->_lightView[i];
    this->_createWarping->setPosition   (GETTEXTURE(GBUFFER_POSITION));
    this->_createWarping->setMvp        (glm::value_ptr(mvp)         );
    this->_createWarping->setDesiredView(this->_desiredView[i]       );
    this->_createWarping->setResolution (GETUINT(RESOLUTION)         );
    this->_createWarping->setWindow     (GETUINT(SMOOTH_WINDOW)      );
    this->_createWarping->setFactor     (GETFLOAT(WARP_FACTOR)       );
    this->_createWarping->setFastSmooth (GETBOOL(USE_FAST_SMOOTH)    );
    this->_createWarping->setSmoothX    (this->_smoothX[i]           );
    this->_createWarping->setSmoothY    (this->_smoothY[i]           );
    this->_createWarping->setNear       (GETFLOAT(NEAR)              );
    this->_createWarping->setFar        (GETFLOAT(FAR)               );
    // TODO USE dpProj

    /*
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
       */
    (*this->_createWarping)();

    this->_createNavyShadowMap->setShadowMap          (this->_shadowMap[i]);
    this->_createNavyShadowMap->setSmoothX            (this->_smoothX[i]               );
    this->_createNavyShadowMap->setSmoothY            (this->_smoothY[i]               );
    this->_createNavyShadowMap->setDesiredView        (this->_desiredView[i]           );
    this->_createNavyShadowMap->setMvp                (glm::value_ptr(mvp)             );
    this->_createNavyShadowMap->setResolution         (GETUINT(RESOLUTION)             );
    this->_createNavyShadowMap->setTessFactor         (GETUINT(TESS_FACTOR)            );
    this->_createNavyShadowMap->setCullTriangles      (GETBOOL(CULL_TRIANGLES)         );
    this->_createNavyShadowMap->setLineToSM           (GETBOOL(LINE_TO_SM)             );
    this->_createNavyShadowMap->setPolygonOffsetFactor(GETFLOAT(FACTOR)                );
    this->_createNavyShadowMap->setPolygonOffsetUnits (GETFLOAT(UNITS)                 );
    this->_createNavyShadowMap->setFactor             (GETFLOAT(WARP_FACTOR)           );
    this->_createNavyShadowMap->setNear               (GETFLOAT(NEAR)                  );
    this->_createNavyShadowMap->setFar                (GETFLOAT(FAR)                   );
    // TODO USE dpProj
    (*this->_createNavyShadowMap)();
  }
  /*
     float data[4];
     for(unsigned i=0;i<6;++i){
     glGetTextureImage(this->_desiredView[i]->getId(),0,GL_RGBA,GL_FLOAT,sizeof(float)*4,data);
     std::cerr<<"face"<<i<<": "<<data[0]<<" - "<<data[1]<<" "<<data[2]<<" - "<<data[3]<<std::endl;
     }*/

}

void NavyDualParaboloid::createShadowMask(GLuint mask){
  this->_shadowMaskFBO->attachColorTexture(GL_COLOR_ATTACHMENT0,mask);
  this->createShadowMask();
}

void NavyDualParaboloid::createShadowMask(){
  GETGPUGAUGE(MEASURE_CREATESHADOWMAP)->begin();
  this->createShadowMap();
  GETGPUGAUGE(MEASURE_CREATESHADOWMAP)->end();

  GETGPUGAUGE(MEASURE_CREATESHADOWMASK)->begin();
  this->_createShadowMask->use();
  this->_createShadowMask->set("near",GETFLOAT(NEAR));
  this->_createShadowMask->set("far",GETFLOAT(FAR));
  this->_createShadowMask->set("lightPosition",1,glm::value_ptr(GETLIGHT->position));
  float data[2*16];
  std::memcpy(data   ,glm::value_ptr(this->_lightView[0]),sizeof(float)*16);
  std::memcpy(data+16,glm::value_ptr(this->_lightView[1]),sizeof(float)*16);
  this->_createShadowMask->set("v",2,GL_FALSE,data);

  GETTEXTURE(GBUFFER_POSITION)->bind(GL_TEXTURE0);
  this->_shadowMap[0]->bind(GL_TEXTURE1);
  this->_shadowMap[1]->bind(GL_TEXTURE2);
  for(unsigned i=0;i<2;++i){
    this->_desiredView[i]->bind(GL_TEXTURE3+i);
    this->_smoothX[i]->bind(GL_TEXTURE5+i);
    this->_smoothY[i]->bind(GL_TEXTURE7+i);
  }

  this->_createShadowMask->set("useWarping",GETFLOAT(WARP_FACTOR)>0.f);
  this->_shadowMaskFBO->bind();
  this->_emptyVAO->bind();
  glDrawArrays(GL_POINTS,0,1);
  this->_emptyVAO->unbind();
  this->_shadowMaskFBO->unbind();
  GETGPUGAUGE(MEASURE_CREATESHADOWMASK)->end();
}
