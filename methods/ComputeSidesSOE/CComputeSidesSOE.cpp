#include"CComputeSidesSOE.hpp"
#include<sstream>

#include<stdlib.h>

/*

CComputeSidesSOE::CComputeSidesSOE(SAdjecency*Adjacency,
    unsigned WorkGroupSize,bool CullSides){
  //std::cerr<<"################################## new compute sides SOE"<<std::endl;

  this->Adjacency=Adjacency;
  unsigned NumVer=2+1+this->Adjacency->MaxOpposite;

  //input
  this->Input=new ge::gl::BufferObject(
      sizeof(float)*4*NumVer*this->Adjacency->NumEdges,
      NULL,GL_STATIC_DRAW);

  float*Ptr=(float*)this->Input->map();
  for(unsigned e=0;e<this->Adjacency->NumEdges;++e){
    //A
    for(int k=0;k<3;++k)
      Ptr[(e*NumVer+0)*4+k]=this->Adjacency->Points[this->Adjacency->Edges[e][0]*3+k];
    Ptr[(e*NumVer+0)*4+3]=1;

    //B
    for(int k=0;k<3;++k)
      Ptr[(e*NumVer+1)*4+k]=this->Adjacency->Points[this->Adjacency->Edges[e][1]*3+k];
    Ptr[(e*NumVer+1)*4+3]=1;

    //N
    Ptr[(e*NumVer+2)*4+0]=this->Adjacency->EdgeOppositeNum[e];
    for(int k=1;k<4;++k)
      Ptr[(e*NumVer+2)*4+k]=0;

    unsigned NumCap=0;
    for(unsigned o=0;o<this->Adjacency->EdgeOppositeNum[e];++o){
      if(	this->Adjacency->EdgeOpposite[e][o]>this->Adjacency->Edges[e][0]&&
          this->Adjacency->EdgeOpposite[e][o]>this->Adjacency->Edges[e][1]){
        for(int k=0;k<3;++k)
          Ptr[(e*NumVer+3+NumCap)*4+k]=
            this->Adjacency->Points[this->Adjacency->EdgeOpposite[e][o]*3+k];
        Ptr[(e*NumVer+3+NumCap)*4+3]=1;
        NumCap++;
      }
    }
    Ptr[(e*NumVer+2)*4+1]=NumCap;
    for(unsigned o=0;o<this->Adjacency->EdgeOppositeNum[e];++o){
      if(!(	this->Adjacency->EdgeOpposite[e][o]>this->Adjacency->Edges[e][0]&&
            this->Adjacency->EdgeOpposite[e][o]>this->Adjacency->Edges[e][1])){
        for(int k=0;k<3;++k)
          Ptr[(e*NumVer+3+NumCap)*4+k]=
            this->Adjacency->Points[this->Adjacency->EdgeOpposite[e][o]*3+k];
        Ptr[(e*NumVer+3+NumCap)*4+3]=0;
        NumCap++;
      }
    }
    for(unsigned o=NumCap;o<this->Adjacency->MaxOpposite;++o)
      for(int k=0;k<4;++k)
        Ptr[(e*NumVer+3+o)*4+k]=0;

  }
  this->Input->unmap();

  //output
  this->Output=new ge::gl::BufferObject(
      sizeof(float)*4*4*
        this->Adjacency->NumEdges*this->Adjacency->MaxOpposite,
      NULL,GL_DYNAMIC_COPY);
  this->Output->clear(GL_R32F,GL_RED,GL_FLOAT);

  //counter
  this->Counter=new ge::gl::BufferObject(
      sizeof(unsigned)*4*this->Adjacency->MaxOpposite,
      NULL,GL_STATIC_DRAW);

  unsigned*CPtr=(unsigned*)this->Counter->map();
  for(unsigned i=0;i<this->Adjacency->MaxOpposite;++i){
    CPtr[i*4+0]=0;
    CPtr[i*4+1]=1;
    CPtr[i*4+2]=i*this->Adjacency->NumEdges*4;
    CPtr[i*4+3]=0;
  }
  this->Counter->unmap();


  //vao
  this->VAO=new ge::gl::VertexArrayObject();
  this->VAO->bind();
  this->Output->bind(GL_ARRAY_BUFFER);
  this->Counter->bind(GL_DRAW_INDIRECT_BUFFER);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,
      sizeof(float)*4,(GLvoid*)(sizeof(float)*0));
  this->VAO->unbind();

  try{
    this->ComputeProgram=new ge::gl::ProgramObject(
        ShaderDir+"methods/ComputeSidesSOE/computesoe.comp",
        (CullSides?ge::gl::ShaderObject::define("CULLSIDES"):"")+
        ge::gl::ShaderObject::define("MAXMULTIPLICITY",this->Adjacency->MaxOpposite)+
        ge::gl::ShaderObject::define("WORKGROUPSIZE",WorkGroupSize));
  }catch(std::string&e){
    std::cerr<<e<<std::endl;
  }
  std::cerr<<"JOJO\n";
  this->DrawProgram=new ge::gl::ProgramObject(
      ShaderDir+"methods/ComputeSidesSOE/draw.vp",
      ShaderDir+"methods/ComputeSidesSOE/draw.cp",
      ShaderDir+"methods/ComputeSidesSOE/draw.ep",
      ShaderDir+"methods/ComputeSidesSOE/draw.fp");
  std::cerr<<"NENE\n";
}

CComputeSidesSOE::~CComputeSidesSOE(){
  this->Input->clear(GL_R32F,GL_RED,GL_FLOAT);
  this->Output->clear(GL_R32F,GL_RED,GL_FLOAT);
  this->Counter->clear(GL_R32UI,GL_RED_INTEGER,GL_UNSIGNED_INT);

  this->Input->unbindBase(GL_SHADER_STORAGE_BUFFER,0);
  this->Output->unbindBase(GL_SHADER_STORAGE_BUFFER,1);
  this->Counter->unbindBase(GL_SHADER_STORAGE_BUFFER,2);

  delete this->VAO;
  delete this->Input;
  delete this->Output;
  delete this->Counter;
  delete this->ComputeProgram;
  delete this->DrawProgram;
}

void CComputeSidesSOE::ComputeSides(float*mvp,simulation::Light*Light){
  for(unsigned i=0;i<this->Adjacency->MaxOpposite;++i)
    this->Counter->clear(GL_R32UI,sizeof(unsigned)*4*i,sizeof(unsigned),
        GL_RED_INTEGER,GL_UNSIGNED_INT);

  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  this->ComputeProgram->use();
  this->ComputeProgram->set("NumEdge",this->Adjacency->NumEdges);

  this->ComputeProgram->set("LightPosition",1,glm::value_ptr(Light->position));
  this->ComputeProgram->set("mvp",1,GL_FALSE,(const float*)mvp);

  this->Input->bindBase(GL_SHADER_STORAGE_BUFFER,0);
  this->Output->bindBase(GL_SHADER_STORAGE_BUFFER,1);
  this->Counter->bindBase(GL_SHADER_STORAGE_BUFFER,2);
  
  unsigned WorkSize=
      (this->Adjacency->NumEdges/this->ComputeProgram->workGroupSize[0])+
      ((this->Adjacency->NumEdges%this->ComputeProgram->workGroupSize[0])?1:0);

  glDispatchCompute(WorkSize,1,1);
 
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void CComputeSidesSOE::DrawSides(float*mvp,simulation::Light*Light){
  this->DrawProgram->use();
  this->DrawProgram->set("mvp",1,GL_FALSE,mvp);
  this->VAO->bind();
  this->Counter->bind(GL_DRAW_INDIRECT_BUFFER);
  glPatchParameteri(GL_PATCH_VERTICES,4);
  for(unsigned i=0;i<this->Adjacency->MaxOpposite;++i){
    glStencilOpValueAMD(GL_FRONT_AND_BACK,i+1);
    glDrawArraysIndirect(GL_PATCHES,(GLvoid*)(sizeof(unsigned)*4*i));
  }
  this->VAO->unbind();
  glStencilOpValueAMD(GL_FRONT_AND_BACK,1);
}

*/
