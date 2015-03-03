#include"CubeShadowMapping.h"

bool operator!=(const CubeShadowMappingTemplate&a,CubeShadowMappingTemplate&b){
  return
    (a.resolution    != b.resolution   )||
    (a.screenSize[0] != b.screenSize[0])||
    (a.screenSize[1] != b.screenSize[1])||
    (a.near          != b.near         )||
    (a.far           != b.far          );
}

CubeShadowMapping::CubeShadowMapping(
    SAdjecency*adjacency,
    ge::gl::VertexArrayObject*sceneVAO,
    CubeShadowMappingTemplate Template
    ){
  this->_adjacency     = adjacency;
  this->_sceneVAO      = sceneVAO;
  this->_resolution    = Template.resolution;
  this->_screenSize[0] = Template.screenSize[0];
  this->_screenSize[1] = Template.screenSize[1];
  this->_near          = Template.near;
  this->_far           = Template.far;
  this->_emptyVAO      = new ge::gl::VertexArrayObject();
  this->_cubeShadowMap = new ge::gl::TextureObject(
      GL_TEXTURE_CUBE_MAP,
      GL_DEPTH_COMPONENT24,
      1,
      this->_resolution,
      this->_resolution);
  float ones[]={1,1,1,1};
  this->_cubeShadowMap->texParameteri (GL_TEXTURE_MIN_FILTER  ,GL_NEAREST             );
  this->_cubeShadowMap->texParameteri (GL_TEXTURE_MAG_FILTER  ,GL_NEAREST             );
  this->_cubeShadowMap->texParameteri (GL_TEXTURE_WRAP_S      ,GL_CLAMP_TO_BORDER     );
  this->_cubeShadowMap->texParameteri (GL_TEXTURE_WRAP_T      ,GL_CLAMP_TO_BORDER     );
  //this->_cubeShadowMap->texParameteri (GL_TEXTURE_COMPARE_FUNC,GL_LEQUAL              );
  //this->_cubeShadowMap->texParameteri (GL_TEXTURE_COMPARE_MODE,GL_COMPARE_R_TO_TEXTURE);
  this->_cubeShadowMap->texParameterfv(GL_TEXTURE_BORDER_COLOR,ones                   );

  GLenum cubeMapSides[]={
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z
  };
  unsigned numCubeMapSides=sizeof(cubeMapSides)/sizeof(GLenum);
  for(unsigned side=0;side<numCubeMapSides;++side){
    this->_fbo[side]=new ge::gl::FramebufferObject();
    //TODO po oprave gpu engine predelat
    this->_fbo[side]->bind();
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        cubeMapSides[side],
        this->_cubeShadowMap->getId(),
        0);
    this->_fbo[side]->unbind();
  }
  this->_createProgram = new ge::gl::ProgramObject(
			ShaderDir+"methods/CubeShadowMapping/create.vp",
			ShaderDir+"methods/CubeShadowMapping/create.fp");
  this->_drawProgram   = new ge::gl::ProgramObject(
			ShaderDir+"methods/CubeShadowMapping/draw.vp",
      ShaderDir+"methods/CubeShadowMapping/draw.gp",
			ShaderDir+"methods/CubeShadowMapping/draw.fp");

  //table 8.19
  //{x,y,z}
  float axes[]={
    +0,+0,+1, +0,-1,+0, +1,+0,+0,
    +0,+0,-1, +0,-1,+0, -1,+0,+0,
    +1,+0,+0, +0,+0,-1, +0,+1,+0,//TODO
    +1,+0,+0, +0,+0,+1, +0,-1,+0,//TODO
    +1,+0,+0, +0,+0,-1, +0,+0,+1,//TODO
    +1,+0,+0, +1,+0,+0, +0,+0,-1 //TODO
  };
  
  for(unsigned side=0;side<numCubeMapSides;++side){
    this->_lightViewMatrix[side]=glm::mat4(1.f);
    for(unsigned axis=0;axis<3;++axis)
      for(unsigned k=0;k<3;++k)
        this->_lightViewMatrix[side][axis][k]=axes[(side*3+axis)*3+k];
    glm::mat4 biasMatrix=
      glm::scale    (glm::mat4(1.f),glm::vec3(.5f))*
      glm::translate(glm::mat4(1.f),glm::vec3(1.f));
    this->_lightProjectionMatrix=glm::perspective(glm::pi<float>()/2.f,1.f,this->_near,this->_far);
    this->_bpvMatrix[side]=biasMatrix*this->_lightProjectionMatrix*this->_lightViewMatrix[side];
  }
}

CubeShadowMapping::~CubeShadowMapping(){
  delete this->_createProgram;
  delete this->_drawProgram;
  delete this->_cubeShadowMap;
  for(unsigned side=0;side<6;++side)delete this->_fbo[side];
}

void CubeShadowMapping::createShadowMap(float*M,simulation::Light*light){
  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
  glViewport(0,0,this->_resolution,this->_resolution);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2.5,10);

  this->_createProgram->use();
  glm::mat4 modelMatrix=
    glm::translate(
        glm::mat4(1.0f),
        glm::vec3(
          -light->position[0],
          -light->position[1],
          -light->position[2]));
  this->_createProgram->set("m",1,GL_FALSE,glm::value_ptr(modelMatrix));

  for(unsigned side=0;side<6;++side){
    this->_createProgram->set("v",1,GL_FALSE,glm::value_ptr(this->_lightViewMatrix[side]));
    this->_createProgram->set("p",1,GL_FALSE,glm::value_ptr(this->_lightProjectionMatrix));
    this->_fbo[side]->bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    this->_sceneVAO->bind();
    glDrawArrays(GL_TRIANGLES,0,this->_adjacency->NumTriangles*3);
    this->_sceneVAO->unbind();
    this->_fbo[side]->unbind();
  }
  glViewport(0,0,this->_screenSize[0],this->_screenSize[1]);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glDisable(GL_POLYGON_OFFSET_FILL);
}

void CubeShadowMapping::drawShadowed(float*pos,simulation::Light*light){
  this->_cubeShadowMap->bind(GL_TEXTURE5);
  this->_drawProgram->use();
  this->_drawProgram->set("La",0.f,0.f,0.f);
  this->_drawProgram->set("Ld",1,glm::value_ptr(light->diffuse));
  this->_drawProgram->set("Ls",1,glm::value_ptr(light->specular));
  this->_drawProgram->set("LightPosition",1,glm::value_ptr(light->position));
  this->_drawProgram->set("CameraPosition",-pos[0],-pos[1],-pos[2]);

  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE,GL_ONE);
  glBlendEquation(GL_FUNC_ADD);
  glDepthFunc(GL_ALWAYS);

  this->_emptyVAO->bind();
  glDrawArrays(GL_POINTS,0,1);
  this->_emptyVAO->unbind();

  glDepthFunc(GL_LESS);
  glDisable(GL_STENCIL_TEST);
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
}

