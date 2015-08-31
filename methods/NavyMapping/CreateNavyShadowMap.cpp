#include"CreateNavyShadowMap.h"

CreateNavyShadowMap::CreateNavyShadowMap(
    std::string dir,
    unsigned width,
    unsigned height,
    ge::gl::VertexArrayObject*sceneVAO,
    unsigned nofTriangles,
    std::string dpProjFile){
  this->_width  = width ;
  this->_height = height;

  this->_vao = sceneVAO;
  this->_nofTriangles = nofTriangles;

  this->_useDP = dpProjFile!="";

  this->_program  = new ge::gl::ProgramObject(
      dir+"createNVTS.vp",
      (this->_useDP?ge::gl::ShaderObject::include(dpProjFile):""),
      dir+"createNVTS.cp",
      ge::gl::ShaderObject::include(dir+"dv.vp")+
      ge::gl::ShaderObject::include(dir+"nv.vp"),
      dir+"createNVTS.ep",
      ge::gl::ShaderObject::include(dir+"dv.vp")+
      ge::gl::ShaderObject::include(dir+"nv.vp"),
      dir+"createNVTS.fp",
      (this->_useDP?ge::gl::ShaderObject::define("USE_PARABOLOID"):""));

  this->_fbo = new ge::gl::FramebufferObject();
}

CreateNavyShadowMap::~CreateNavyShadowMap(){
  delete this->_program;
  delete this->_fbo;
}

void CreateNavyShadowMap::setShadowMap(ge::gl::TextureObject*shadowMap,GLenum target){
  /*
  glNamedFramebufferTextureLayer(
      this->_fbo->getId(),
      GL_DEPTH_ATTACHMENT,
      shadowMap->getId(),
      0,
      target-GL_TEXTURE_CUBE_MAP_POSITIVE_X);
   //   */
  /*
  this->_fbo->bind();
  glFramebufferTexture2D(
      GL_FRAMEBUFFER,
      GL_DEPTH_ATTACHMENT,
      target,
      shadowMap->getId(),
      0);
  this->_fbo->unbind();
  */

  if(target==GL_TEXTURE_2D)
    glNamedFramebufferTexture(this->_fbo->getId(),GL_DEPTH_ATTACHMENT,shadowMap->getId(),0);
  else
    glNamedFramebufferTextureLayer(this->_fbo->getId(),GL_DEPTH_ATTACHMENT,shadowMap->getId(),0,target-GL_TEXTURE_CUBE_MAP_POSITIVE_X);
  float v=1;
  glClearNamedFramebufferfv(this->_fbo->getId(),GL_DEPTH,0,&v);
  
  if(!this->_fbo->check())std::cerr<<"framebuffer je divny!!!!!!"<<std::endl;
}

void CreateNavyShadowMap::setSmoothX(ge::gl::TextureObject*smoothX){
  this->_smoothX = smoothX;
}

void CreateNavyShadowMap::setSmoothY(ge::gl::TextureObject*smoothY){
  this->_smoothY = smoothY;
}

void CreateNavyShadowMap::setDesiredView(ge::gl::TextureObject*desiredView){
  this->_desiredView = desiredView;
}

void CreateNavyShadowMap::setMvp(const float*mvp){
  this->_mvp = mvp;
}

void CreateNavyShadowMap::setResolution(unsigned resolution){
  this->_resolution = resolution;
}

void CreateNavyShadowMap::setTessFactor(unsigned tessFactor){
  this->_tessFactor = tessFactor;
}

void CreateNavyShadowMap::setCullTriangles(bool cull){
  this->_cullTriangles = cull;
}

void CreateNavyShadowMap::setLineToSM(bool lineToSM){
  this->_lineToSM = lineToSM;
}

void CreateNavyShadowMap::setPolygonOffsetFactor(float polygonOffsetFactor){
  this->_polygonOffsetFactor = polygonOffsetFactor;
}

void CreateNavyShadowMap::setPolygonOffsetUnits(float polygonOffsetUnits){
  this->_polygonOffsetUnits = polygonOffsetUnits;
}

void CreateNavyShadowMap::setFactor(float factor){
  this->_factor = factor;
}

void CreateNavyShadowMap::setNear(float near){
  this->_near = near;
}
void CreateNavyShadowMap::setFar(float far){
  this->_far = far;
}

void CreateNavyShadowMap::operator()(){
  this->_program->use();
  this->_program->set("mvp",1,GL_FALSE,this->_mvp          );
  this->_program->set("shadowMapSize" ,this->_resolution   );
  this->_program->set("tessFactor"    ,this->_tessFactor   );
  this->_program->set("cullTriangles" ,this->_cullTriangles);
  this->_program->set("useWarping",this->_factor>0.f);

  if(this->_useDP){
    this->_program->set("near",this->_near);
    this->_program->set("far",this->_far);
  }

  //this->_desiredView->bindImage(2,0);
  this->_desiredView->bind(GL_TEXTURE2);
  this->_smoothX->bind(GL_TEXTURE3);
  this->_smoothY->bind(GL_TEXTURE4);

  this->_fbo->bind();
  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
  glViewport(0,0,this->_resolution,this->_resolution);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);
  //glClear(GL_DEPTH_BUFFER_BIT);

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(this->_polygonOffsetFactor,this->_polygonOffsetUnits);

  this->_vao->bind();

  glPatchParameteri(GL_PATCH_VERTICES,3);
  if(this->_lineToSM)glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glDrawArrays(GL_PATCHES,0,this->_nofTriangles*3);
  if(this->_lineToSM)glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  this->_vao->unbind();

  glViewport(0,0,this->_width,this->_height);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glDisable(GL_POLYGON_OFFSET_FILL);
  this->_fbo->unbind();
}
