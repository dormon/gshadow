#include"CComputeSides.hpp"
#include"../../app/fastAdjacency.h"

#include<geGL/UniformCommands.h>
#include<geGL/ProgramCommands.h>

CComputeSides::CComputeSides(Adjacency*ad,
    unsigned WorkGroupSize,bool CullSides){
  this->_workGroupSize=WorkGroupSize;
  this->_adjacency=ad;
  //input
  this->_maxMult  = ad->getMaxMultiplicity();
  this->_nofEdges = ad->getNofEdges();

  unsigned NumVer=2+1+this->_maxMult;

  this->_input=new ge::gl::BufferObject(sizeof(float)*4*NumVer*this->_nofEdges);

  float*Ptr=(float*)this->_input->map();

  for(unsigned e=0;e<this->_nofEdges;++e){
    //A
    for(int k=0;k<3;++k)
      Ptr[(e*NumVer+0)*4+k]=ad->getVertices()[ad->getEdge(e,0)+k];
    Ptr[(e*NumVer+0)*4+3]=1;
    //B
    for(int k=0;k<3;++k)
      Ptr[(e*NumVer+1)*4+k]=ad->getVertices()[ad->getEdge(e,1)+k];
    Ptr[(e*NumVer+1)*4+3]=1;
    //N
    Ptr[(e*NumVer+2)*4+0]=ad->getNofOpposite(e);
    for(int k=1;k<4;++k)
      Ptr[(e*NumVer+2)*4+k]=0;
    //O
    for(unsigned o=0;o<ad->getNofOpposite(e);++o){
      for(int k=0;k<3;++k)
        Ptr[(e*NumVer+3+o)*4+k]=ad->getVertices()[ad->getOpposite(e,o)+k];
      Ptr[(e*NumVer+3+o)*4+3]=1;
    }
    for(unsigned o=ad->getNofOpposite(e);o<this->_maxMult;++o){
      for(int k=0;k<4;++k)
        Ptr[(e*NumVer+3+o)*4+k]=0;
    }
  }
  this->_input->unmap();

  this->_output=new ge::gl::BufferObject(
      sizeof(float)*4*4*this->_nofEdges*this->_maxMult,
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
      ge::gl::ShaderObject::define("MAX_MULTIPLICITY",(int)this->_maxMult)+
      (CullSides?ge::gl::ShaderObject::define("CULL_SIDE"):"")+
      "");

  std::string dir=ShaderDir+"methods/ComputeSidesSOE/draw.";
  this->_drawProgram=new ge::gl::ProgramObject(dir+"vp",dir+"cp",dir+"ep",dir+"fp");

  this->_computeList = new ge::gl::CommandList(false);
  this->_computeList->add(new ge::gl::UseProgram(this->_computeProgram->getId()));
  this->_computeList->add(new ge::gl::Uniform<1,GLuint>(this->_computeProgram,"NumEdge",this->_nofEdges));
  this->_lightUniformCommand = std::dynamic_pointer_cast<ge::gl::UniformV<4,GLfloat>,ge::gl::Command>(*this->_computeList->add(new ge::gl::UniformV<4,GLfloat>(this->_computeProgram,"LightPosition",NULL)));
  //this->_computeList->add(new ge::gl::UniformV<4,GLfloat>(this->_computeProgram,"LightPosition",glm::value_ptr(Light->position)));
  //
 // ge::gl::UniformData<3,GLfloat>*dada=new ge::gl::UniformData<3,GLfloat>();
  float add;
  ge::gl::UniformDataRef<3,GLfloat>*dadaref=new ge::gl::UniformDataRef<3,GLfloat>(
      (float*)&add,
      (float*)&add,
      (float*)&add);
  dadaref->data[0]=NULL;

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


  this->_lightUniformCommand->set(glm::value_ptr(Light->position));
  this->_computeList->apply();
  //this->_computeProgram->use();
  //this->_computeProgram->set("NumEdge",this->_nofEdges);
  //this->_computeProgram->set("LightPosition",1,glm::value_ptr(Light->position));
  this->_computeProgram->set("mvp",1,GL_FALSE,(const float*)mvp);
  this->_computeProgram->bindSSBO("IBuffer",this->_input  );
  this->_computeProgram->bindSSBO("OBuffer",this->_output );
  this->_computeProgram->bindSSBO("Counter",this->_counter);

  unsigned WorkSize=
    (this->_nofEdges/this->_workGroupSize)+
    ((this->_nofEdges%this->_workGroupSize)?1:0);

  glDispatchCompute(WorkSize,1,1);

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

