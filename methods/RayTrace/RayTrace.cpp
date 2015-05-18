#include"RayTrace.h"

#include"../ShadowMapping/createsm.h"

#define CLASSNAME RayTrace
#include"../ShadowMethodMacro.h"

DEFVARSSTART
  "shaderDirectory",
  "sceneVAO",
  "sceneVBO",
  "window.size",
  "fastAdjacency",
  "light",
  "gbuffer.position",
  "shadowMask",
  "raytrace.program.RAY.WORKGROUP_SIZE_X",
  "raytrace.program.RAY.WORKGROUP_SIZE_Y",
DEFVARSEND

DEFVARSIDSTART
  SHADERDIRECTORY=0,
  SCENEVAO,
  SCENEVBO,
  WINDOWSIZE,
  FASTADJACENCY,
  LIGHT,
  GBUFFER_POSITION,
  SHADOWMASK,
  RAYTRACE_WGSX,
  RAYTRACE_WGSY
DEFVARSIDEND

DEFGETNOFDEP
DEFGETDEP

void RayTrace::update(){
  //TODO
}

RayTrace::RayTrace(simulation::SimulationData*data):simulation::SimulationObject(data){
  this->_nofTriangles = GETFASTADJACENCY->getNofTriangles();
  std::string dir=GETSTRING(SHADERDIRECTORY)+"/methods/RayTrace/";
  this->_rayProgram = new ge::gl::ProgramObject(
      dir+"raytrace.comp");
  this->_triangles=new ge::gl::BufferObject(sizeof(float)*3*3*this->_nofTriangles);
  float*out = (float*)this->_triangles->map();
  float*in  = (float*)GETBUFFER(SCENEVBO)->map();

  for(unsigned t=0;t<this->_nofTriangles;++t){
    for(unsigned v=0;v<3;++v){
      for(unsigned k=0;k<3;++k){
        out[(t*3+v)*3+k]=in[(t*3+v)*6+k];
      }
    }
  }
  GETBUFFER(SCENEVBO)->unmap();
  this->_triangles->unmap();
}

RayTrace::~RayTrace(){
  delete this->_rayProgram;
  delete this->_triangles;
}

void RayTrace::createShadowMask(){
  glm::uvec2 winSize = GETUVEC2(WINDOWSIZE);
  this->_rayProgram->use();
  this->_rayProgram->set("lightPosition",1,glm::value_ptr(GETLIGHT->position));
  this->_rayProgram->set("nofTriangles",this->_nofTriangles);
  this->_rayProgram->set("windowSize",winSize.x,winSize.y);
  GETTEXTURE(SHADOWMASK)->bindImage(0,0);
  GETTEXTURE(GBUFFER_POSITION)->bindImage(1,0);
  //GETBUFFER(SCENEVBO)->bindBase(GL_SHADER_STORAGE_BUFFER,0);
  this->_triangles->bindBase(GL_SHADER_STORAGE_BUFFER,0);
  glDispatchCompute(
      winSize.x/GETUINT(RAYTRACE_WGSX)+1,
      winSize.y/GETUINT(RAYTRACE_WGSY)+1,
      1);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  //TODO
}

void RayTrace::createShadowMask(GLuint mask){
  //TODO
}

