#include"NavyMapping.h"

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

  std::cerr<<"ahahsd"<<std::endl;

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

  this->DCM = new ge::gl::ProgramObject(
      ShaderDir+"methods/NavyMapping/drawcm.vp",
      ShaderDir+"methods/NavyMapping/drawcm.gp",
      ShaderDir+"methods/NavyMapping/drawcm.fp",
      ge::gl::ShaderObject::define("DRAWCM_BINDING_COUNTMAP",(int)DRAWCM_BINDING_COUNTMAP)+
      "");

  this->WCM = new ge::gl::ProgramObject(
      ShaderDir+"methods/NavyMapping/writecm.vp",
      ShaderDir+"methods/NavyMapping/writecm.gp",
      ShaderDir+"methods/NavyMapping/writecm.fp",
      ge::gl::ShaderObject::define("WRITECM_BINDING_POSITIONMAP",WRITECM_BINDING_POSITIONMAP)+
      ge::gl::ShaderObject::define("WRITECM_BINDING_COUNTMAP"   ,WRITECM_BINDING_COUNTMAP   )+
      "");

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

  this->smoothGridProgram = new ge::gl::ProgramObject(
      ShaderDir+"methods/NavyMapping/smoothGrid.comp",
      ge::gl::ShaderObject::define("BINDING_GRID"       ,SMOOTHGRID_BINDING_GRID    )+
      ge::gl::ShaderObject::define("BINDING_SMOOTH"     ,SMOOTHGRID_BINDING_SMOOTH  )+
      ge::gl::ShaderObject::define("SHADOWMAP_SIZE"     ,(int)this->ShadowMapSize   )+
      ge::gl::ShaderObject::define("WORKGROUP_SIZE_X"   ,SMOOTHGRID_WORKGROUP_SIZE_X)+
      ge::gl::ShaderObject::define("WALKING_WINDOW_SIZE",WALKING_WINDOW_SIZE        )+
      "");

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

  this->drawUnwarpXProgram = new ge::gl::ProgramObject(
      ShaderDir+"methods/NavyMapping/drawUnwarpX.vp",
      ShaderDir+"methods/NavyMapping/drawUnwarpX.gp",
      ShaderDir+"methods/NavyMapping/drawUnwarpX.fp",
      ge::gl::ShaderObject::define("DRAWUNWARPX_BINDING_UNWARPXCOUNTMAP",(int)DRAWUNWARPX_BINDING_UNWARPXCOUNTMAP)+
      ge::gl::ShaderObject::define("SHADOWMAP_SIZE"                     ,(int)this->ShadowMapSize                )+
      "");

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

NavyMapping::~NavyMapping(){
  glDeleteVertexArrays(1,&this->EmptyVAO);
  delete this->fbo;
  delete this->shadowMap;
  delete this->SM;
  delete this->CSM;
}

void NavyMapping::minProjection(glm::mat4 p,glm::mat4 v,glm::mat4 lp,glm::mat4 lv){
  /*lp=this->LightProjection;
  lv=this->LightView;
  for(unsigned i=0;i<8;++i){
    glm::vec4 corner = glm::vec4(-1.f+2.f*((i>>0)&1),-1.f+2.f*((i>>1)&1),-1.f+2.f*((i>>2)&1),1);
    glm::vec4 view   = glm::inverse(p)*corner;
    glm::vec4 world  = glm::inverse(v)*glm::vec4(view.x/view.w,view.y/view.w,view.z/view.w,1);
    glm::vec4 clip   = lp*lv*world;
    //std::cerr<<clip.x/clip.w<<" "<<clip.y/clip.w<<std::endl;
  }*/
}

void NavyMapping::setFocus(simulation::Light*Light,float*FocusPoint,float Fovy){
  this->LightProjection=glm::perspective(Fovy,1.f,1.f,10000.f);
  this->LightView=glm::lookAt(
      glm::vec3(Light->position),
      glm::vec3(FocusPoint[0],FocusPoint[1],FocusPoint[2]),
      glm::vec3(0.f,1.f,0.f));
  this->BPV=
    glm::scale(glm::mat4(1.),glm::vec3(.5))*
    glm::translate(glm::mat4(1.),glm::vec3(1.))*
    LightProjection*
    LightView;
}

void NavyMapping::createShadowMap(float*M){
  this->CSM->use();
  CSM->set("m",1,GL_FALSE,M);
  CSM->set("v",1,GL_FALSE,(float*)glm::value_ptr(this->LightView));
  CSM->set("p",1,GL_FALSE,(float*)glm::value_ptr(this->LightProjection));

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
  this->CNM->set("v",1,GL_FALSE,(float*)glm::value_ptr(this->LightView));
  this->CNM->set("p",1,GL_FALSE,(float*)glm::value_ptr(this->LightProjection));
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
  this->SM->set("BPV",1,GL_FALSE,glm::value_ptr(this->BPV));
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
  this->NSM->set("BPV",1,GL_FALSE,glm::value_ptr(this->BPV));
  this->NSM->set("v",1,GL_FALSE,(float*)glm::value_ptr(this->LightView));
  this->NSM->set("p",1,GL_FALSE,(float*)glm::value_ptr(this->LightProjection));


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

void NavyMapping::drawCountMap(float x,float y,float w){
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glViewport(
      x*this->ScreenSize[0],
      y*this->ScreenSize[1],
      w*this->ScreenSize[0],
      w*this->ScreenSize[1]);
  glBindVertexArray(this->EmptyVAO);
  this->DCM->use();
  this->DCM->set("shadowMapSize",(unsigned)this->ShadowMapSize);
  this->countMap->bindImage(DRAWCM_BINDING_COUNTMAP,0);
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
  this->WCM->set("BPV",1,GL_FALSE,glm::value_ptr(this->BPV));
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

/*
   void NavyMapping::smoothGridX(){
   glClearTexImage(this->smoothedGridX->getId(),0,GL_RED,GL_FLOAT,NULL);
   glMemoryBarrier(GL_ALL_BARRIER_BITS);
   this->smoothGridXProgram->use();
   this->gridX        ->bindImage(SMOOTHGRIDX_BINDING_GRIDX        ,0);
   this->smoothedGridX->bindImage(SMOOTHGRIDX_BINDING_SMOOTHEDGRIDX,0);
   glDispatchCompute(this->ShadowMapSize/SMOOTHGRIDX_WORKGROUP_SIZE_X,1,1);
   glMemoryBarrier(GL_ALL_BARRIER_BITS);
   }*/

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

/*
   void NavyMapping::drawGridX(float x,float y,float w){
   glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
   glViewport(
   x*this->ScreenSize[0],
   y*this->ScreenSize[1],
   w*this->ScreenSize[0],
   w*this->ScreenSize[1]);
   glBindVertexArray(this->EmptyVAO);

   this->drawGridXProgram->use();
   this->smoothedGridX->bindImage(GRIDX_BINDING_SMOOTHEDGRIDX,0);

   for(int l=0;l<=GRID_SIZE_X;++l){
   this->drawGridXProgram->set("lineId",l);
   glDrawArrays(GL_LINE_STRIP,0,this->ShadowMapSize);
   }
   glBindVertexArray(0);
   glViewport(0,0,this->ScreenSize[0],this->ScreenSize[1]);
   }*/

void NavyMapping::unwarpX(GLuint positionTexture){
  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
  glDepthMask(GL_FALSE);
  glClearTexImage(this->unwarpXCountMap->getId(),0,GL_RED_INTEGER,GL_UNSIGNED_INT,NULL);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  glViewport(0,0,this->ScreenSize[0],this->ScreenSize[1]);
  glBindVertexArray(this->EmptyVAO);
  this->unwarpXProgram->use();
  this->unwarpXProgram->set("BPV",1,GL_FALSE,glm::value_ptr(this->BPV));
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

void NavyMapping::drawUnwarpX(float x,float y,float w){
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glViewport(
      x*this->ScreenSize[0],
      y*this->ScreenSize[1],
      w*this->ScreenSize[0],
      w*this->ScreenSize[1]);
  glBindVertexArray(this->EmptyVAO);
  this->drawUnwarpXProgram->use();
  this->unwarpXCountMap->bindImage(DRAWUNWARPX_BINDING_UNWARPXCOUNTMAP,0);
  glDrawArrays(GL_POINTS,0,1);
  glBindVertexArray(0);
  glViewport(0,0,this->ScreenSize[0],this->ScreenSize[1]);
}

/*void NavyMapping::prefixSumY(){
  glClearTexImage(this->prefixSumYMap->getId(),0,GL_RED_INTEGER,GL_UNSIGNED_INT,NULL);
  glClearTexImage(this->gridY        ->getId(),0,GL_RED_INTEGER,GL_UNSIGNED_INT,NULL);
  glMemoryBarrier(GL_ALL_BARRIER_BITS);
  this->prefixSumYProgram->use();
  this->unwarpXCountMap->bindImage(PREFIXSUMY_BINDING_UNWARPXCOUNTMAP,0);
  this->prefixSumYMap  ->bindImage(PREFIXSUMY_BINDING_PREFIXSUMMAP   ,0);
  this->gridY          ->bindImage(PREFIXSUMY_BINDING_GRIDY          ,0);
  glDispatchCompute(this->ShadowMapSize/PREFIXSUMY_WORKGROUP_SIZE_X,1,1);
  glMemoryBarrier(GL_ALL_BARRIER_BITS);
  }*/

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

