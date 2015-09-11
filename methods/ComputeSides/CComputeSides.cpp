#include"CComputeSides.hpp"
#include"../../app/fastAdjacency.h"

#include"AdjacencyToVBO.h"

CComputeSides::CComputeSides(Adjacency*ad,
    unsigned WorkGroupSize,bool CullSides){
  this->_workGroupSize=WorkGroupSize;
  this->_adjacency=ad;
  this->_nofEdges = ad->getNofEdges();

  AdjacencyToVBO(&this->_input,this->_adjacency)();

  this->_output=new ge::gl::BufferObject(
      sizeof(float)*4*4*this->_nofEdges*this->_adjacency->getMaxMultiplicity(),
      NULL,GL_DYNAMIC_COPY);
  this->_output->clear(GL_R32F,GL_RED,GL_FLOAT);

  this->_counter=new ge::gl::BufferObject(sizeof(unsigned)*4);
  unsigned*CPtr=(unsigned*)this->_counter->map();
  CPtr[0]=0;
  CPtr[1]=1;
  CPtr[2]=0;
  CPtr[3]=0;
  this->_counter->unmap();

  this->_vao=new ge::gl::VertexArrayObject();
  this->_vao->addAttrib(this->_output,0,4,GL_FLOAT,sizeof(float)*4,(GLvoid*)(sizeof(float)*0));

  this->_computeProgram = new ge::gl::ProgramObject(
      ShaderDir+"methods/ComputeSides/compute.comp",
      ge::gl::ShaderObject::define("WORKGROUP_SIZE_X",(int)WorkGroupSize)+
      ge::gl::ShaderObject::define("MAX_MULTIPLICITY",(int)this->_adjacency->getMaxMultiplicity())+
      (CullSides?ge::gl::ShaderObject::define("CULL_SIDE"):"")+
      "");

  std::string dir=ShaderDir+"methods/ComputeSidesSOE/draw.";
  this->_drawProgram=new ge::gl::ProgramObject(dir+"vp",dir+"cp",dir+"ep",dir+"fp");

}

CComputeSides::~CComputeSides(){
  delete this->_vao;
  delete this->_input;
  delete this->_output;
  delete this->_counter;
  delete this->_computeProgram;
  delete this->_drawProgram;
}

void CComputeSides::ComputeSides(float*mvp,simulation::Light*Light){
  this->_counter->clear(GL_R32UI,0,sizeof(unsigned),GL_RED_INTEGER,GL_UNSIGNED_INT);

  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  this->_computeProgram->use();
  this->_computeProgram->set("NumEdge",this->_nofEdges);
  this->_computeProgram->set("LightPosition",1,glm::value_ptr(Light->position));
  this->_computeProgram->set("mvp",1,GL_FALSE,(const float*)mvp);
  this->_computeProgram->bindSSBO("IBuffer",this->_input  );
  this->_computeProgram->bindSSBO("OBuffer",this->_output );
  this->_computeProgram->bindSSBO("Counter",this->_counter);

  glDispatchCompute(ge::core::getDispatchSize(this->_nofEdges,this->_workGroupSize),1,1);

  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void CComputeSides::DrawSides(float*mvp,simulation::Light*Light){
  this->_drawProgram->use();
  this->_drawProgram->set("mvp",1,GL_FALSE,mvp);
  this->_vao->bind();
  this->_counter->bind(GL_DRAW_INDIRECT_BUFFER);
  glPatchParameteri(GL_PATCH_VERTICES,4);
  glDrawArraysIndirect(GL_PATCHES,NULL);
  this->_vao->unbind();
}

