#include"CGeometrySides.hpp"

#include"../../app/SilhouetteShaderGenerator.h"

bool operator!=(const SGeometryTemplate&a, const SGeometryTemplate&b){
  return
    (a.Deterministic         != b.Deterministic        )||
    (a.ReferenceEdge         != b.ReferenceEdge        )||
    (a.Universal             != b.Universal            )||
    (a.UseVS2GSArray         != b.UseVS2GSArray        )||
    (a.UseVertexArray        != b.UseVertexArray       )||
    (a.UseLayouts            != b.UseLayouts           )||
    (a.UseStencilValueExport != b.UseStencilValueExport)||
    (a.CCWIsFrontFace        != b.CCWIsFrontFace       )||
    (a.FrontFaceInside       != b.FrontFaceInside      )||
    (a.CullSides             != b.CullSides            )||
    (a.Visualize             != b.Visualize            )||
    (a.FrontCapToInfinity    != b.FrontCapToInfinity   )||
    (a.GenerateSides         != b.GenerateSides        )||
    (a.GenerateCaps          != b.GenerateCaps         )||
    (a.Matrix                != b.Matrix               )||
    (a.Version               != b.Version              )||
    (a.Profile               != b.Profile              )||
    (a.MaxMultiplicity       != b.MaxMultiplicity      )||
    (a.LightUniform          != b.LightUniform         )||
    (a.VertexAttribName      != b.VertexAttribName     );
}


ge::gl::ProgramObject*GenerateGeometryShader(SGeometryTemplate t){
  return new ge::gl::ProgramObject(
      ShaderDir+"app/geometrysilhouette.vp",
      (t.Universal     ?ge::gl::ShaderObject::define("UNIVERSAL"         ):"")+
      (t.UseVS2GSArray ?ge::gl::ShaderObject::define("USE_ARRAY_TRANSFER"):"")+
      (t.UseVertexArray?ge::gl::ShaderObject::define("USE_VERTEX_ARRAY"  ):"")+
      (t.UseLayouts    ?ge::gl::ShaderObject::define("USE_LAYOUTS"       ):"")+
      ge::gl::ShaderObject::define("VERTEX_ATTRIB_NAME",t.VertexAttribName   )+
      ge::gl::ShaderObject::define("MAX_MULTIPLICITY"  ,t.MaxMultiplicity    )+
      "",
      ShaderDir+"app/geometrysilhouette.gp",
      (t.Deterministic        ?ge::gl::ShaderObject::define("DETERMINISTIC"           ):"")+
      (t.ReferenceEdge        ?ge::gl::ShaderObject::define("REFERENCE_EDGE"          ):"")+
      (t.Universal            ?ge::gl::ShaderObject::define("UNIVERSAL"               ):"")+
      (t.UseVS2GSArray        ?ge::gl::ShaderObject::define("USE_ARRAY_TRANSFER"      ):"")+
      (t.UseStencilValueExport?ge::gl::ShaderObject::define("USE_STENCIL_VALUE_EXPORT"):"")+
      (t.CCWIsFrontFace       ?ge::gl::ShaderObject::define("CCW_IS_FRONT_FACE"       ):"")+
      (t.FrontFaceInside      ?ge::gl::ShaderObject::define("FRONT_FACE_INSIDE"       ):"")+
      (t.CullSides            ?ge::gl::ShaderObject::define("CULL_SIDES"              ):"")+
      (t.FrontCapToInfinity   ?ge::gl::ShaderObject::define("FRONT_CAP_TO_INFINITY"   ):"")+
      (t.GenerateSides        ?ge::gl::ShaderObject::define("GENERATE_SIDES"          ):"")+
      (t.GenerateCaps         ?ge::gl::ShaderObject::define("GENERATE_CAPS"           ):"")+
      (t.Matrix!=""           ?ge::gl::ShaderObject::define("MATRIX",t.Matrix         ):"")+
      ge::gl::ShaderObject::define("MAX_MULTIPLICITY",t.MaxMultiplicity                   )+
      ge::gl::ShaderObject::define("LIGHT"           ,t.LightUniform                      )+
      "",
      ShaderDir+"app/geometrysilhouette.fp",
      (t.UseStencilValueExport?ge::gl::ShaderObject::define("USE_STENCIL_VALUE_EXPORT"):"")+
      (t.UseLayouts           ?ge::gl::ShaderObject::define("USE_LAYOUTS"             ):"")+
      (t.Visualize            ?ge::gl::ShaderObject::define("VISUALIZE"               ):"")+
      "",
      t.Version,
      t.Profile);
}

std::string translateType(GLenum type){
  switch(type){
    case GL_BYTE                        :return"GL_BYTE"                        ;
    case GL_UNSIGNED_BYTE               :return"GL_UNSIGNED_BYTE"               ;
    case GL_SHORT                       :return"GL_SHORT"                       ;
    case GL_UNSIGNED_SHORT              :return"GL_UNSIGNED_SHORT"              ;
    case GL_INT                         :return"GL_INT"                         ;
    case GL_UNSIGNED_INT                :return"GL_UNSIGNED_INT"                ;
    case GL_FLOAT                       :return"GL_FLOAT"                       ;
    case GL_HALF_FLOAT                  :return"GL_HALF_FLOAT"                  ;
    case GL_DOUBLE                      :return"GL_DOUBLE"                      ;
    case GL_FIXED                       :return"GL_FIXED"                       ;
    case GL_INT_2_10_10_10_REV          :return"GL_INT_2_10_10_10_REV"          ;
    case GL_UNSIGNED_INT_2_10_10_10_REV :return"GL_UNSIGNED_INT_2_10_10_10_REV" ;
    case GL_UNSIGNED_INT_10F_11F_11F_REV:return"GL_UNSIGNED_INT_10F_11F_11F_REV";
    default                             :return"unknown"                        ;
  }
}

std::string traslateBoolean(GLboolean boolean){
  switch(boolean){
    case GL_TRUE :return"GL_TRUE" ;
    case GL_FALSE:return"GL_FALSE";
    default      :return"unknown" ;
  }
}

CGeometrySides::CGeometrySides(
    SAdjecency*Adjacency,
    SGeometryTemplate t){
  this->Adjacency=Adjacency;
  this->Universal=t.Universal;
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
        Ptr[(e*NumV+2+1+o)*4+3]=1;
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
      //std::cerr<<"CARD: "<<m+1<<" : "<<this->Adjacency->ClassCard[m]<<std::endl;
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
          Ptr[(ec*NumV+2+o)*4+3]=1;
        }
        //zeros
        for(;o<this->Adjacency->MaxOpposite;++o)
          for(int k=0;k<4;++k)Ptr[(ec*NumV+2+o)*4+k]=0;
        ec++;
      }     
      this->SVBO[m]->unmap();
    }
  }

  t.GenerateCaps    = false;
  t.GenerateSides   = true;
  t.MaxMultiplicity = this->Adjacency->MaxOpposite;

  if(this->Universal){
    this->UProgram=GenerateGeometryShader(t);
  }else{
    this->SProgram=new ge::gl::ProgramObject*[this->Adjacency->MaxOpposite];
    for(unsigned m=0;m<this->Adjacency->MaxOpposite;++m){
      if(this->Adjacency->ClassCard[m]==0)continue;//skip empty classes
      t.MaxMultiplicity=m+1;
      this->SProgram[m]=GenerateGeometryShader(t);
    }
  }
  std::cerr<<"preduniversal\n";

  //generate VAO
  if(this->Universal){
    this->uvao=new ge::gl::VertexArrayObject();
    //this->UVAO=new ge::gl::CVertexArray();
    //this->UVAO->Bind();
    this->UVBO->bind(GL_ARRAY_BUFFER);
    unsigned NumV=2+1+this->Adjacency->MaxOpposite;
    for(unsigned a=0;a<NumV;++a){
      this->uvao->addAttrib(
          this->UVBO->getId(),a,4,GL_FLOAT,sizeof(float)*4*NumV,(GLvoid*)(sizeof(float)*4*a));
    }
    //this->UVAO->UnBind();
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

  if(this->uvao){
    std::cerr<<this->uvao->getInfo();
  }
}

CGeometrySides::~CGeometrySides(){
  if(this->Universal){
    delete this->UVBO;
    //delete this->UVAO;
    delete this->uvao;
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

void CGeometrySides::DrawSides(float*mvp,simulation::Light*Light){
  if(this->Universal){
    this->UProgram->use();
    this->UProgram->set("mvp",1,GL_FALSE,mvp);
    this->UProgram->set("LightPosition",1,glm::value_ptr(Light->position));
    //this->UVAO->Bind();
    this->uvao->bind();
    glDrawArrays(GL_POINTS,0,this->Adjacency->NumEdges);
    this->uvao->unbind();
    //this->UVAO->UnBind();
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


