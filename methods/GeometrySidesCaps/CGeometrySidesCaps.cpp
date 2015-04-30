#include"CGeometrySidesCaps.hpp"

#include"../../app/SilhouetteShaderGenerator.h"

CGeometrySidesCaps::CGeometrySidesCaps(
    SAdjecency*Adjacency,
    bool Deterministic,
    bool ReferenceEdge,
    bool Universal,
    bool UseVS2GSArray,
    bool UseVertexArray,
    bool UseStencilValueExport,
    bool CullSides,
    bool FrontCapToInfinity,
    unsigned Version){

  //std::cerr<<"################################## new GeometrySides sides and caps"<<std::endl;
  this->Adjacency=Adjacency;
  this->Universal=Universal;
  //generate VBO
  if(this->Universal){
    unsigned NumV=2+1+this->Adjacency->MaxOpposite;
    this->UVBO=new ge::gl::BufferObject(
        sizeof(float)*4*NumV*this->Adjacency->NumEdges,NULL,GL_STATIC_DRAW);
    float*Ptr=(float*)this->UVBO->map();
    for(unsigned e=0;e<this->Adjacency->NumEdges;++e){//loop over edges
      //A
      for(int k=0;k<3;++k)
        Ptr[(e*NumV+0)*4+k]=this->Adjacency->Points[this->Adjacency->Edges[e][0]*3+k];
      Ptr[(e*NumV+0)*4+3]=1;
      //B
      for(int k=0;k<3;++k)
        Ptr[(e*NumV+1)*4+k]=this->Adjacency->Points[this->Adjacency->Edges[e][1]*3+k];
      Ptr[(e*NumV+1)*4+3]=1;
      //N
      Ptr[(e*NumV+2)*4+0]=this->Adjacency->EdgeOppositeNum[e];
      for(int k=1;k<4;++k)
        Ptr[(e*NumV+2)*4+k]=0;
      //Oi
      unsigned o=0;
      for(;o<this->Adjacency->EdgeOppositeNum[e];++o){
        for(int k=0;k<3;++k)
          Ptr[(e*NumV+2+1+o)*4+k]=this->Adjacency->Points[this->Adjacency->EdgeOpposite[e][o]*3+k];
        if( this->Adjacency->EdgeOpposite[e][o]>this->Adjacency->Edges[e][0]&&
            this->Adjacency->EdgeOpposite[e][o]>this->Adjacency->Edges[e][1])
          Ptr[(e*NumV+2+1+o)*4+3]=1;
        else
          Ptr[(e*NumV+2+1+o)*4+3]=0;
      }
      //zeros
      for(;o<this->Adjacency->MaxOpposite;++o)
        for(int k=0;k<4;++k)Ptr[(e*NumV+2+1+o)*4+k]=0;
    }
    this->UVBO->unmap();
  }else{
    this->SVBO=new ge::gl::BufferObject*[this->Adjacency->MaxOpposite];
    for(unsigned m=0;m<this->Adjacency->MaxOpposite;++m){//loop over multiplicities
      if(this->Adjacency->ClassCard[m]==0)continue;//skip empty classes
      unsigned NumV=2+(m+1);
      this->SVBO[m]=new ge::gl::BufferObject(
          sizeof(float)*4*NumV*this->Adjacency->ClassCard[m],NULL,GL_STATIC_DRAW);
      float*Ptr=(float*)this->SVBO[m]->map();
      unsigned ec=0;
      for(unsigned e=0;e<this->Adjacency->NumEdges;++e){//loop over edges

        if(this->Adjacency->EdgeOppositeNum[e]!=m+1)continue;//this is different class
        //A
        for(int k=0;k<3;++k)
          Ptr[(ec*NumV+0)*4+k]=this->Adjacency->Points[this->Adjacency->Edges[e][0]*3+k];
        Ptr[(ec*NumV+0)*4+3]=1;
        //B
        for(int k=0;k<3;++k)
          Ptr[(ec*NumV+1)*4+k]=this->Adjacency->Points[this->Adjacency->Edges[e][1]*3+k];
        Ptr[(ec*NumV+1)*4+3]=1;
        //Oi
        unsigned o=0;
        for(;o<this->Adjacency->EdgeOppositeNum[e];++o){
          for(int k=0;k<3;++k)
            Ptr[(ec*NumV+2+o)*4+k]=this->Adjacency->Points[this->Adjacency->EdgeOpposite[e][o]*3+k];
          if( this->Adjacency->EdgeOpposite[e][o]>this->Adjacency->Edges[e][0]&&
              this->Adjacency->EdgeOpposite[e][o]>this->Adjacency->Edges[e][1])
            Ptr[(ec*NumV+2+o)*4+3]=1;
          else
            Ptr[(ec*NumV+2+o)*4+3]=0;
        }
        //zeros
        for(;o<this->Adjacency->MaxOpposite;++o)
          for(int k=0;k<4;++k)Ptr[(ec*NumV+2+1+o)*4+k]=0;
        ec++;
      }     
      this->SVBO[m]->unmap();
    }
  }

  //generate shaders
  NGeometrySilhouetteShader::STemplate t;
  t.Deterministic=Deterministic;
  t.ReferenceEdge=ReferenceEdge;
  t.Universal=this->Universal;
  t.UseVS2GSArray=UseVS2GSArray;
  t.UseVertexArray=UseVertexArray;
  t.UseLayouts=true;
  t.UseStencilValueExport=UseStencilValueExport;
  t.CCWIsFrontFace=true;
  t.FrontFaceInside=false;
  t.CullSides=CullSides;
  t.Visualize=false;
  t.FrontCapToInfinity=FrontCapToInfinity;
  t.GenerateSides=true;
  t.GenerateCaps=true;
  t.Matrix="mvp";
  t.Version=Version;
  t.Profile=CORE;
  t.MaxMultiplicity=this->Adjacency->MaxOpposite;
  t.LightUniform="LightPosition";
  t.VertexAttribName="Position";

  if(this->Universal){
    ge::gl::ShaderObject*UV=new ge::gl::ShaderObject(GL_VERTEX_SHADER,
        NGeometrySilhouetteShader::getVertex(t));
    ge::gl::ShaderObject*UG=new ge::gl::ShaderObject(GL_GEOMETRY_SHADER,
        NGeometrySilhouetteShader::getGeometry(t));
    ge::gl::ShaderObject*UF=new ge::gl::ShaderObject(GL_FRAGMENT_SHADER,
        NGeometrySilhouetteShader::getFragment(t));

    this->UProgram=new ge::gl::ProgramObject(3,UV,UG,UF);

    //delete UV;
    //delete UG;
    //delete UF;
  }else{
    this->SProgram=new ge::gl::ProgramObject*[this->Adjacency->MaxOpposite];
    for(unsigned m=0;m<this->Adjacency->MaxOpposite;++m){
      if(this->Adjacency->ClassCard[m]==0)continue;//skip empty classes
      t.MaxMultiplicity=m+1;

      ge::gl::ShaderObject*SV=new ge::gl::ShaderObject(GL_VERTEX_SHADER,
          NGeometrySilhouetteShader::getVertex(t));
      ge::gl::ShaderObject*SG=new ge::gl::ShaderObject(GL_GEOMETRY_SHADER,
          NGeometrySilhouetteShader::getGeometry(t));
      ge::gl::ShaderObject*SF=new ge::gl::ShaderObject(GL_FRAGMENT_SHADER,
          NGeometrySilhouetteShader::getFragment(t));

      this->SProgram[m]=new ge::gl::ProgramObject(3,SV,SG,SF);

      //delete SV;
      //delete SG;
      //delete SF;
    }
  }

  //generate VAO
  if(this->Universal){
    this->UVAO=new ge::gl::VertexArrayObject();
    this->UVAO->bind();
    this->UVBO->bind(GL_ARRAY_BUFFER);
    unsigned NumV=2+1+this->Adjacency->MaxOpposite;
    for(unsigned a=0;a<NumV;++a){
      glEnableVertexAttribArray(a);
      glVertexAttribPointer(a,4,GL_FLOAT,GL_FALSE,
          sizeof(float)*4*NumV,(GLvoid*)(sizeof(float)*4*a));
    }
    this->UVAO->unbind();
    this->UVBO->unbind(GL_ARRAY_BUFFER);
  }else{
    this->SVAO=new ge::gl::VertexArrayObject*[this->Adjacency->MaxOpposite];
    for(unsigned m=0;m<this->Adjacency->MaxOpposite;++m){
      if(this->Adjacency->ClassCard[m]==0)continue;//skip empty classes
      this->SVAO[m]=new ge::gl::VertexArrayObject();
      this->SVAO[m]->bind();
      this->SVBO[m]->bind(GL_ARRAY_BUFFER);
      unsigned NumV=2+(m+1);
      for(unsigned a=0;a<2+(m+1);++a){
        glEnableVertexAttribArray(a);
        glVertexAttribPointer(a,4,GL_FLOAT,GL_FALSE,
            sizeof(float)*4*NumV,(GLvoid*)(sizeof(float)*4*a));
      }
      this->SVAO[m]->unbind();
      this->SVBO[m]->unbind(GL_ARRAY_BUFFER);
    }
  }


}

CGeometrySidesCaps::~CGeometrySidesCaps(){

  //std::cerr<<"################################## delete GeometrySides sides"<<std::endl;
  if(this->Universal){
    delete this->UVBO;
    delete this->UVAO;
    delete this->UProgram;
  }else{
    for(unsigned m=0;m<this->Adjacency->MaxOpposite;++m){
      if(this->Adjacency->ClassCard[m]==0)continue;
      delete this->SVBO[m];
      delete this->SVAO[m];
      delete this->SProgram[m];
    }
    delete[]this->SVAO;
    delete[]this->SVBO;
  }
}

void CGeometrySidesCaps::DrawSidesCaps(float*mvp,simulation::Light*Light){
  if(this->Universal){
    this->UProgram->use();
    this->UProgram->set("mvp",1,GL_FALSE,(const float*)mvp);
    this->UProgram->set("LightPosition",1,glm::value_ptr(Light->position));
    this->UVAO->bind();
    glDrawArrays(GL_POINTS,0,this->Adjacency->NumEdges);
    this->UVAO->unbind();
  }else{
    for(unsigned m=0;m<this->Adjacency->MaxOpposite;++m){
      if(this->Adjacency->ClassCard[m]==0)continue;
      this->SProgram[m]->use();
      this->SProgram[m]->set("mvp",1,GL_FALSE,mvp);
      this->SProgram[m]->set("LightPosition",1,glm::value_ptr(Light->position));
      this->SVAO[m]->bind();
      glDrawArrays(GL_POINTS,0,this->Adjacency->ClassCard[m]);
      this->SVAO[m]->unbind();
    }
  }
}



