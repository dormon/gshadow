#include"CShadowMapping.hpp"

enum shadowMapVariableNames{
  SHADERDIRECTORY=0,
  SCENEVAO,
  EMPTYVAO,
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
  MEASURE_SHADOWMAP_CREATESHADOWMAP,
  MEASURE_SHADOWMAP_CREATESHADOWMASK
};

const std::string shadowMapVariables[]={
  "shaderDirectory",
  "sceneVAO",
  "emptyVAO",
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
  "measure.shadowMap.createShadowMap",
  "measure.shadowMap.createShadowMask"
};

unsigned CShadowMapping::getNofDependentVariables(){
  return sizeof(shadowMapVariables)/sizeof(std::string);
}
std::string CShadowMapping::getDependentVariable(unsigned id){
  return shadowMapVariables[id];
}
void CShadowMapping::update(){
  if(
      this->_changed[shadowMapVariables[FOCUSPOINT]]||
      this->_changed[shadowMapVariables[LIGHT     ]]||
      this->_changed[shadowMapVariables[FOVY      ]]||
      this->_changed[shadowMapVariables[NEAR      ]]||
      this->_changed[shadowMapVariables[FAR       ]]){
    this->_computeMatrices();
    this->_changed[shadowMapVariables[FOCUSPOINT]]=false;
    this->_changed[shadowMapVariables[LIGHT     ]]=false;
    this->_changed[shadowMapVariables[FOVY      ]]=false;
    this->_changed[shadowMapVariables[NEAR      ]]=false;
    this->_changed[shadowMapVariables[FAR       ]]=false;
  }
  if(this->_changed[shadowMapVariables[WINDOWSIZE]]){
    this->_setWindowSize();
    this->_changed[shadowMapVariables[WINDOWSIZE]]=false;
  }
  if(
      this->_changed[shadowMapVariables[ADJACENCY]]||
      this->_changed[shadowMapVariables[SCENEVAO ]]){
    this->_adjacency = (SAdjecency*)this->_simulationData->getObject(shadowMapVariables[ADJACENCY ],NULL);
    this->_changed[shadowMapVariables[ADJACENCY]]=false;
    this->_changed[shadowMapVariables[SCENEVAO ]]=false;
  }
  if(this->_changed[shadowMapVariables[RESOLUTION]]){
    this->_createShadowMap();
    this->_changed[shadowMapVariables[RESOLUTION]]=false;
  }
  if(this->_changed[shadowMapVariables[GBUFFER_POSITION]]){
    this->_position=(ge::gl::TextureObject*)this->_simulationData->getObject(shadowMapVariables[GBUFFER_POSITION],NULL);
    this->_changed[shadowMapVariables[GBUFFER_POSITION]]=false;
  }
  if(this->_changed[shadowMapVariables[SHADOWMASK]]){
    this->_createShadowMapFBO();
    this->_changed[shadowMapVariables[SHADOWMASK]]=false;
  }
  if(this->_changed[shadowMapVariables[MEASURE_SHADOWMAP_CREATESHADOWMAP]]){
    this->_setMeasureCreateShadowMap();
    this->_changed[shadowMapVariables[MEASURE_SHADOWMAP_CREATESHADOWMAP]]=false;
  }
}

void CShadowMapping::createShadowMask(){
  if(this->_createrShadowMapGauge)this->_measureCreateShadowMap->begin();
  this->CreateShadowMap();
  if(this->_createrShadowMapGauge){
    this->_measureCreateShadowMap->end();
    this->_createrShadowMapGauge->insert(this->_measureCreateShadowMap->getui64()/1000000000.f);
  }

  if(this->_createrShadowMaskGauge)this->_measureCreateShadowMask->begin();
  this->_createShadowMask->use();
  this->_createShadowMask->set("BPV",1,GL_FALSE,glm::value_ptr(this->_bpv));

  this->_position->bind(GL_TEXTURE0);
  this->_shadowMap->bind(GL_TEXTURE1);

  this->_shadowMaskFBO->bind();
  this->_emptyVAO->bind();
  glDrawArrays(GL_POINTS,0,1);
  this->_emptyVAO->unbind();
  this->_shadowMaskFBO->unbind();
  if(this->_createrShadowMaskGauge){
    this->_measureCreateShadowMask->end();
    this->_createrShadowMaskGauge->insert(this->_measureCreateShadowMask->getui64()/1000000000.f);
  }

}

CShadowMapping::CShadowMapping(simulation::SimulationData*data):simulation::SimulationObject(data){
  this->_simulationData->registerUser(this);
  this->_shadowMap = NULL;
  this->_fbo       = NULL;
  this->_csm       = NULL;
  this->_dsm       = NULL;
  this->_shadowMap = NULL;
  this->_fbo       = NULL;
  this->_adjacency = NULL;
  this->_sceneVAO  = NULL;
  this->_shadowMap = NULL;
  this->_shadowMask    = NULL;
  this->_position      = NULL;
  this->_shadowMaskFBO = NULL;
  this->_measureCreateShadowMap  = NULL;
  this->_measureCreateShadowMask = NULL;
  this->_createrShadowMapGauge  = NULL;
  this->_createrShadowMaskGauge = NULL;

  //std::cerr<<"NEW SHADOW MAPPING\n";
  this->_sceneVAO  = (ge::gl::VertexArrayObject*)this->_simulationData->getObject(shadowMapVariables[SCENEVAO        ],NULL);
  this->_emptyVAO  = (ge::gl::VertexArrayObject*)this->_simulationData->getObject(shadowMapVariables[EMPTYVAO        ],NULL);
  this->_adjacency = (SAdjecency*)               this->_simulationData->getObject(shadowMapVariables[ADJACENCY       ],NULL);
  this->_position  = (ge::gl::TextureObject*)    this->_simulationData->getObject(shadowMapVariables[GBUFFER_POSITION],NULL);


  //std::cerr<<"NEW SHADOW MAPPING MIDDLE\n";
  this->_setMeasureCreateShadowMap();
  this->_setMeasureCreateShadowMask();
  this->_computeMatrices   ();
  this->_setWindowSize     ();
  this->_createShadowMap   ();
  this->_createShadowMapFBO();

  this->_csm = new ge::gl::ProgramObject(
      this->_simulationData->getString(shadowMapVariables[SHADERDIRECTORY],"")+"methods/ShadowMapping/createshadowmap.vp",
      this->_simulationData->getString(shadowMapVariables[SHADERDIRECTORY],"")+"methods/ShadowMapping/createshadowmap.fp");

  this->_createShadowMask = new ge::gl::ProgramObject(
      this->_simulationData->getString(shadowMapVariables[SHADERDIRECTORY],"")+"methods/ShadowMapping/createShadowMask.vp",
      this->_simulationData->getString(shadowMapVariables[SHADERDIRECTORY],"")+"methods/ShadowMapping/createShadowMask.gp",
      this->_simulationData->getString(shadowMapVariables[SHADERDIRECTORY],"")+"methods/ShadowMapping/createShadowMask.fp");

  //std::cerr<<"NEW SHADOW MAPPING END\n";
  //for(unsigned i=0;i<sizeof(shadowMapVariables)/sizeof(std::string);++i)
  //  this->_changed.insert(std::pair<std::string,bool>(shadowMapVariables[i],false));
}

void CShadowMapping::_setMeasureCreateShadowMap(){
  this->_createrShadowMapGauge = this->_simulationData->getGauge(shadowMapVariables[MEASURE_SHADOWMAP_CREATESHADOWMAP],NULL);
  if(this->_createrShadowMapGauge){
    if(!this->_measureCreateShadowMap)
      this->_measureCreateShadowMap = new ge::gl::AsynchronousQueryObject(
          GL_TIME_ELAPSED,GL_QUERY_RESULT_NO_WAIT,ge::gl::AsynchronousQueryObject::UINT64);
  }
}
void CShadowMapping::_setMeasureCreateShadowMask(){
  this->_createrShadowMaskGauge = this->_simulationData->getGauge(shadowMapVariables[MEASURE_SHADOWMAP_CREATESHADOWMASK],NULL);
  if(this->_createrShadowMaskGauge){
    if(!this->_measureCreateShadowMask)
      this->_measureCreateShadowMask = new ge::gl::AsynchronousQueryObject(
          GL_TIME_ELAPSED,GL_QUERY_RESULT_NO_WAIT,ge::gl::AsynchronousQueryObject::UINT64);
  }
}


void CShadowMapping::_createShadowMap(){
  this->_shadowMapSize = this->_simulationData->getInt   (shadowMapVariables[RESOLUTION],1024);
  float ones[]={1,1,1,1};
  if(this->_shadowMap)delete this->_shadowMap;
  this->_shadowMap=new ge::gl::TextureObject(GL_TEXTURE_2D,GL_DEPTH_COMPONENT24,1,this->_shadowMapSize,this->_shadowMapSize);
  this->_shadowMap->texParameteri (GL_TEXTURE_MIN_FILTER  ,GL_NEAREST             );
  this->_shadowMap->texParameteri (GL_TEXTURE_MAG_FILTER  ,GL_NEAREST             );
  this->_shadowMap->texParameteri (GL_TEXTURE_WRAP_S      ,GL_CLAMP_TO_BORDER     );
  this->_shadowMap->texParameteri (GL_TEXTURE_WRAP_T      ,GL_CLAMP_TO_BORDER     );
  this->_shadowMap->texParameteri (GL_TEXTURE_COMPARE_FUNC,GL_LEQUAL              );
  this->_shadowMap->texParameteri (GL_TEXTURE_COMPARE_MODE,GL_COMPARE_R_TO_TEXTURE);
  this->_shadowMap->texParameterfv(GL_TEXTURE_BORDER_COLOR,ones                   );

  if(!this->_fbo)this->_fbo=new ge::gl::FramebufferObject();
  this->_fbo->attachDepthTexture(this->_shadowMap->getId());
}

void CShadowMapping::_setWindowSize(){
  this->_screenSize[0] = this->_simulationData->getIVec2("window.size",glm::ivec2(1024,1024)).x;
  this->_screenSize[1] = this->_simulationData->getIVec2("window.size",glm::ivec2(1024,1024)).y;
}

void CShadowMapping::_computeMatrices(){
  this->_focusPoint   =                     this->_simulationData->getVec3  (shadowMapVariables[FOCUSPOINT],glm::vec3(0.0));
  this->_light        = (simulation::Light*)this->_simulationData->getLight (shadowMapVariables[LIGHT     ],NULL          );
  this->_fovy         =                     this->_simulationData->getFloat (shadowMapVariables[FOVY      ],45.f          );
  this->_near         =                     this->_simulationData->getFloat (shadowMapVariables[NEAR      ],1.f           );
  this->_far          =                     this->_simulationData->getFloat (shadowMapVariables[FAR       ],10000.f       );

  this->_lightProjection = glm::perspective(this->_fovy,1.f,this->_near,this->_far);
  this->_lightView       = glm::lookAt(
      glm::vec3(this->_light->position),
      glm::vec3(this->_focusPoint),
      glm::vec3(0.f,1.f,0.f));
  this->_bpv=
    glm::scale(glm::mat4(1.),glm::vec3(.5))*
    glm::translate(glm::mat4(1.),glm::vec3(1.))*
    _lightProjection*
    _lightView;
}

void CShadowMapping::_createShadowMapFBO(){
  this->_shadowMask=(ge::gl::TextureObject*)this->_simulationData->getObject(shadowMapVariables[SHADOWMASK],NULL);
  if(!this->_shadowMaskFBO)this->_shadowMaskFBO=new ge::gl::FramebufferObject();
  this->_shadowMaskFBO->attachColorTexture(GL_COLOR_ATTACHMENT0,this->_shadowMask->getId());
}


CShadowMapping::~CShadowMapping(){
  this->_simulationData->unregisterUser(this);
  delete this->_csm;
  delete this->_createShadowMask;
  delete this->_shadowMap;
  delete this->_fbo;
  delete this->_shadowMaskFBO;
}

void CShadowMapping::CreateShadowMap(){
  this->_csm->use();
  this->_csm->set("v",1,GL_FALSE,(float*)glm::value_ptr(this->_lightView));
  this->_csm->set("p",1,GL_FALSE,(float*)glm::value_ptr(this->_lightProjection));

  //glBindFramebuffer(GL_FRAMEBUFFER,this->FBO);
  this->_fbo->bind();
  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
  glViewport(0,0,this->_shadowMapSize,this->_shadowMapSize);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);
  glClear(GL_DEPTH_BUFFER_BIT);

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2.5,10);

  this->_sceneVAO->bind();
  glDrawArrays(GL_TRIANGLES,0,this->_adjacency->NumTriangles*3);
  this->_sceneVAO->unbind();

  glViewport(0,0,this->_screenSize[0],this->_screenSize[1]);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glDisable(GL_POLYGON_OFFSET_FILL);
  //glBindFramebuffer(GL_FRAMEBUFFER,0);
  this->_fbo->unbind();
}


void CShadowMapping::drawShadowMap(float x,float y,float w){
  glViewport(
      x*this->_screenSize[0],
      y*this->_screenSize[1],
      w*this->_screenSize[0],
      w*this->_screenSize[1]);
  this->_emptyVAO->bind();
  this->_dsm->use();
  this->_shadowMap->bind(GL_TEXTURE9);
  glDrawArrays(GL_POINTS,0,1);
  this->_emptyVAO->unbind();
  glViewport(0,0,this->_screenSize[0],this->_screenSize[1]);
}

void CShadowMapping::setMatrices(glm::mat4 lp,glm::mat4 lv){
  this->_lightView       = lv;
  this->_lightProjection = lp;
  this->_bpv=
    glm::scale(glm::mat4(1.),glm::vec3(.5))*
    glm::translate(glm::mat4(1.),glm::vec3(1.))*
    this->_lightProjection*
    this->_lightView;
}
