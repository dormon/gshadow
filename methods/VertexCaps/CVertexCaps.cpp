#include"CVertexCaps.hpp"

#if 0

CVertexCaps::CVertexCaps(SAdjecency*Adjacency){
  this->Adjacency=Adjacency;

  this->VBO=new ge::gl::BufferObject(
      sizeof(float)*4*this->Adjacency->NumTriangles*6*3,NULL,GL_STATIC_DRAW);

  float*Ptr=(float*)this->VBO->map();
  for(unsigned t=0;t<this->Adjacency->NumTriangles;++t){//loop over triangles
    for(unsigned v=0;v<6;++v){//loop over vertices of caps
      for(unsigned a=0;a<3;++a){//loop over vertex attributes
        for(unsigned e=0;e<3;++e){//loop over elements of vector
          Ptr[((t*6+v)*3+a)*4+e]=this->Adjacency->Points[this->Adjacency->Triangles[t*3+a]*3+e];
        }
        if(a>0)Ptr[((t*6+v)*3+a)*4+3]=1;
        else{
          unsigned CapVertexID=0;
          if(v<3)CapVertexID=v;
          else CapVertexID=8-v;
          Ptr[((t*6+v)*3+a)*4+3]=CapVertexID;
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
      GL_FLOAT,GL_FALSE,sizeof(float)*4*(3),(GLvoid*)(sizeof(float)*4*0));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1,4,
      GL_FLOAT,GL_FALSE,sizeof(float)*4*(3),(GLvoid*)(sizeof(float)*4*1));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2,4,
      GL_FLOAT,GL_FALSE,sizeof(float)*4*(3),(GLvoid*)(sizeof(float)*4*2));

  this->VAO->unbind();

  SVSSilTemplate TVS;
  TVS.Version=430;
  TVS.UseLayouts=true;
  TVS.UseVertexAttribArray=false;
  TVS.LightPositionUniformName="LightPosition";
  TVS.MatrixUniformName="mvp";
  TVS.VertexAttribName="Position";
  TVS.Universal=true;


  ge::gl::ShaderObject*VS=new ge::gl::ShaderObject(GL_VERTEX_SHADER,GenVSSilVertexCap(TVS));
  ge::gl::ShaderObject*FS=new ge::gl::ShaderObject(GL_FRAGMENT_SHADER,GenVSSilFragmentCap(TVS));
  this->Program=new ge::gl::ProgramObject(2,VS,FS);

  //delete VS;
  //delete FS;

}

CVertexCaps::~CVertexCaps(){
  delete this->VAO;
  delete this->VBO;
  delete this->Program;
}

void CVertexCaps::DrawCaps(float*mvp,simulation::Light*Light){
  this->Program->use();
  this->Program->set("LightPosition",1,glm::value_ptr(Light->position));
  this->Program->set("mvp",1,GL_FALSE,(const float*)mvp);
  this->VAO->bind();
  glDrawArrays(GL_TRIANGLES,0,this->Adjacency->NumTriangles*6);
  this->VAO->unbind();
}

#endif
