#include"CVertexSides.hpp"

CVertexSides::CVertexSides(SAdjecency*Adjacency){
  this->Adjacency=Adjacency;

  unsigned NumSides=this->Adjacency->MaxOpposite;
  this->VBO=new ge::gl::BufferObject(
      sizeof(float)*4*(this->Adjacency->MaxOpposite+2)*6*NumSides*this->Adjacency->NumEdges,
      NULL,GL_STATIC_DRAW);

  float*Ptr=(float*)this->VBO->map();
  for(unsigned e=0;e<this->Adjacency->NumEdges;++e){//loop over edges
    for(unsigned s=0;s<NumSides;++s){//loop over sides
      for(unsigned v=0;v<6;++v){//loop over vertices of side
        unsigned Base=((e*NumSides+s)*6+v)*(this->Adjacency->MaxOpposite+2);

        Ptr[(Base+0)*4+0]=this->Adjacency->Points[this->Adjacency->Edges[e][0]*3+0];
        Ptr[(Base+0)*4+1]=this->Adjacency->Points[this->Adjacency->Edges[e][0]*3+1];
        Ptr[(Base+0)*4+2]=this->Adjacency->Points[this->Adjacency->Edges[e][0]*3+2];

        unsigned Num=this->Adjacency->MaxOpposite;

        if(this->Adjacency->EdgeOppositeNum[e]<this->Adjacency->MaxOpposite)
          Num=this->Adjacency->EdgeOppositeNum[e];
        Ptr[(Base+0)*4+3]=Num;

        Ptr[(Base+1)*4+0]=this->Adjacency->Points[this->Adjacency->Edges[e][1]*3+0];
        Ptr[(Base+1)*4+1]=this->Adjacency->Points[this->Adjacency->Edges[e][1]*3+1];
        Ptr[(Base+1)*4+2]=this->Adjacency->Points[this->Adjacency->Edges[e][1]*3+2];

        unsigned VertexIDCCW;
        if(v>2)VertexIDCCW=v-2;//3,4,5 -> 1,2,3
        else VertexIDCCW=2-v;//0,1,2 -> 2,1,0
        unsigned VertexIDCW;
        if(v>2)VertexIDCW=6-v;//3,4,5 -> 3,2,1
        else VertexIDCW=v;//0,1,2 -> 0,1,2
        Ptr[(Base+1)*4+3]=(s*4+VertexIDCW)*4+VertexIDCCW;

        for(unsigned o=0;o<Num;++o){//loop over opposite vertices
          Ptr[(Base+2+o)*4+0]=this->Adjacency->Points[this->Adjacency->EdgeOpposite[e][o]*3+0];
          Ptr[(Base+2+o)*4+1]=this->Adjacency->Points[this->Adjacency->EdgeOpposite[e][o]*3+1];
          Ptr[(Base+2+o)*4+2]=this->Adjacency->Points[this->Adjacency->EdgeOpposite[e][o]*3+2];
          Ptr[(Base+2+o)*4+3]=1;
        }
      }
    }
  }
  this->VBO->unmap();

  this->VAO=new ge::gl::VertexArrayObject();
  this->VAO->bind();
  this->VBO->bind(GL_ARRAY_BUFFER);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,4,
  GL_FLOAT,GL_FALSE,sizeof(float)*4*(this->Adjacency->MaxOpposite+2),(GLvoid*)(sizeof(float)*4*0));
  
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1,4,
  GL_FLOAT,GL_FALSE,sizeof(float)*4*(this->Adjacency->MaxOpposite+2),(GLvoid*)(sizeof(float)*4*1));

  for(unsigned a=0;a<this->Adjacency->MaxOpposite;++a){
    glEnableVertexAttribArray(2+a);
    glVertexAttribPointer(2+a,4,
    GL_FLOAT,GL_FALSE,sizeof(float)*4*(this->Adjacency->MaxOpposite+2),(GLvoid*)(sizeof(float)*4*(2+a)));
  }
  this->VAO->unbind();

  SVSSilTemplate TVS;
  TVS.Version=430;
  TVS.UseLayouts=true;
  TVS.UseVertexAttribArray=false;
  TVS.LightPositionUniformName="LightPosition";
  TVS.MatrixUniformName="mvp";
  TVS.VertexAttribName="Position";
  TVS.Universal=true;

  ge::gl::ShaderObject*VS=new ge::gl::ShaderObject(GL_VERTEX_SHADER,GenVSSilVertexHull(this->Adjacency->MaxOpposite,TVS));
  ge::gl::ShaderObject*FS=new ge::gl::ShaderObject(GL_FRAGMENT_SHADER,GenVSSilFragmentHull(TVS));

  this->Program=new ge::gl::ProgramObject(2,VS,FS);

  //delete VS;
  //delete FS;

}

CVertexSides::~CVertexSides(){
  delete this->VAO;
  delete this->VBO;
  delete this->Program;
}

void CVertexSides::DrawSides(float*mvp,simulation::Light*Light){
  this->Program->use();
  this->Program->set("LightPosition",1,glm::value_ptr(Light->position));
  this->Program->set("mvp",1,GL_FALSE,(const float*)mvp);
  this->VAO->bind();
  glDrawArrays(GL_TRIANGLES,0,this->Adjacency->NumEdges*3*2*this->Adjacency->MaxOpposite);
  this->VAO->unbind();
}


