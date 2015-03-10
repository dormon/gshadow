#include"CComputeSides.hpp"

CComputeSides::CComputeSides(SAdjecency*Adjacency,
    unsigned WorkGroupSize,bool CullSides){
  this->_workGroupSize=WorkGroupSize;
  this->_adjacency=Adjacency;
  unsigned NumVer=2+1+this->_adjacency->MaxOpposite;

  //input
  this->_input=new ge::gl::BufferObject(sizeof(float)*4*NumVer*this->_adjacency->NumEdges);

  float*Ptr=(float*)this->_input->map();
  for(unsigned e=0;e<this->_adjacency->NumEdges;++e){
    //A
    for(int k=0;k<3;++k)
      Ptr[(e*NumVer+0)*4+k]=this->_adjacency->Points[this->_adjacency->Edges[e][0]*3+k];
    Ptr[(e*NumVer+0)*4+3]=1;

    //B
    for(int k=0;k<3;++k)
      Ptr[(e*NumVer+1)*4+k]=this->_adjacency->Points[this->_adjacency->Edges[e][1]*3+k];
    Ptr[(e*NumVer+1)*4+3]=1;

    //N
    Ptr[(e*NumVer+2)*4+0]=this->_adjacency->EdgeOppositeNum[e];
    for(int k=1;k<4;++k)
      Ptr[(e*NumVer+2)*4+k]=0;

    unsigned NumCap=0;
    for(unsigned o=0;o<this->_adjacency->EdgeOppositeNum[e];++o){
      if(	this->_adjacency->EdgeOpposite[e][o]>this->_adjacency->Edges[e][0]&&
          this->_adjacency->EdgeOpposite[e][o]>this->_adjacency->Edges[e][1]){
        for(int k=0;k<3;++k)
          Ptr[(e*NumVer+3+NumCap)*4+k]=
            this->_adjacency->Points[this->_adjacency->EdgeOpposite[e][o]*3+k];
        Ptr[(e*NumVer+3+NumCap)*4+3]=1;
        NumCap++;
      }
    }
    Ptr[(e*NumVer+2)*4+1]=NumCap;
    for(unsigned o=0;o<this->_adjacency->EdgeOppositeNum[e];++o){
      if(!(	this->_adjacency->EdgeOpposite[e][o]>this->_adjacency->Edges[e][0]&&
            this->_adjacency->EdgeOpposite[e][o]>this->_adjacency->Edges[e][1])){
        for(int k=0;k<3;++k)
          Ptr[(e*NumVer+3+NumCap)*4+k]=
            this->_adjacency->Points[this->_adjacency->EdgeOpposite[e][o]*3+k];
        Ptr[(e*NumVer+3+NumCap)*4+3]=0;
        NumCap++;
      }
    }
    for(unsigned o=NumCap;o<this->_adjacency->MaxOpposite;++o)
      for(int k=0;k<4;++k)
        Ptr[(e*NumVer+3+o)*4+k]=0;
  }
  this->_input->unmap();

  this->_output=new ge::gl::BufferObject(
      sizeof(float)*4*4*this->_adjacency->NumEdges*this->_adjacency->MaxOpposite,
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
      ge::gl::ShaderObject::define("MAX_MULTIPLICITY",(int)this->_adjacency->MaxOpposite)+
      (CullSides?ge::gl::ShaderObject::define("CULL_SIDE"):"")+
      "");

  this->_drawProgram=new ge::gl::ProgramObject(
      ShaderDir+"methods/ComputeSidesSOE/draw.vp",
      ShaderDir+"methods/ComputeSidesSOE/draw.cp",
      ShaderDir+"methods/ComputeSidesSOE/draw.ep",
      ShaderDir+"methods/ComputeSidesSOE/draw.fp");
}

CComputeSides::~CComputeSides(){
  this->_input  ->unbindBase(GL_SHADER_STORAGE_BUFFER,0);
  this->_output ->unbindBase(GL_SHADER_STORAGE_BUFFER,1);
  this->_counter->unbindBase(GL_SHADER_STORAGE_BUFFER,2);

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
  this->_computeProgram->set("NumEdge",this->_adjacency->NumEdges);
  this->_computeProgram->set("LightPosition",1,glm::value_ptr(Light->position));
  this->_computeProgram->set("mvp",1,GL_FALSE,mvp);
  this->_input  ->bindBase(GL_SHADER_STORAGE_BUFFER,0);
  this->_output ->bindBase(GL_SHADER_STORAGE_BUFFER,1);
  this->_counter->bindBase(GL_SHADER_STORAGE_BUFFER,2);
 
  unsigned WorkSize=
      (this->_adjacency->NumEdges/this->_workGroupSize)+
      ((this->_adjacency->NumEdges%this->_workGroupSize)?1:0);

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

