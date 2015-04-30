#include"CComputeSides.hpp"
#include"../../app/fastAdjacency.h"

#define FAST_ADJACENCY

std::string translateUniformType(GLenum type){
  switch(type){
    case GL_FLOAT                                    :return"GL_FLOAT";
    case GL_FLOAT_VEC2                               :return"GL_FLOAT_VEC2";
    case GL_FLOAT_VEC3                               :return"GL_FLOAT_VEC3";
    case GL_FLOAT_VEC4                               :return"GL_FLOAT_VEC4";
    case GL_DOUBLE                                   :return"GL_DOUBLE";
    case GL_DOUBLE_VEC2                              :return"GL_DOUBLE_VEC2";
    case GL_DOUBLE_VEC3                              :return"GL_DOUBLE_VEC3";
    case GL_DOUBLE_VEC4                              :return"GL_DOUBLE_VEC4";
    case GL_INT                                      :return"GL_INT";
    case GL_INT_VEC2                                 :return"GL_INT_VEC2";
    case GL_INT_VEC3                                 :return"GL_INT_VEC3";
    case GL_INT_VEC4                                 :return"GL_INT_VEC4";
    case GL_UNSIGNED_INT                             :return"GL_UNSIGNED_INT";
    case GL_UNSIGNED_INT_VEC2                        :return"GL_UNSIGNED_INT_VEC2";
    case GL_UNSIGNED_INT_VEC3                        :return"GL_UNSIGNED_INT_VEC3";
    case GL_UNSIGNED_INT_VEC4                        :return"GL_UNSIGNED_INT_VEC4";
    case GL_BOOL                                     :return"GL_BOOL";
    case GL_BOOL_VEC2                                :return"GL_BOOL_VEC2";
    case GL_BOOL_VEC3                                :return"GL_BOOL_VEC3";
    case GL_BOOL_VEC4                                :return"GL_BOOL_VEC4";
    case GL_FLOAT_MAT2                               :return"GL_FLOAT_MAT2";
    case GL_FLOAT_MAT3                               :return"GL_FLOAT_MAT3";
    case GL_FLOAT_MAT4                               :return"GL_FLOAT_MAT4";
    case GL_FLOAT_MAT2x3                             :return"GL_FLOAT_MAT2x3";
    case GL_FLOAT_MAT2x4                             :return"GL_FLOAT_MAT2x4";
    case GL_FLOAT_MAT3x2                             :return"GL_FLOAT_MAT3x2";
    case GL_FLOAT_MAT3x4                             :return"GL_FLOAT_MAT3x4";
    case GL_FLOAT_MAT4x2                             :return"GL_FLOAT_MAT4x2";
    case GL_FLOAT_MAT4x3                             :return"GL_FLOAT_MAT4x3";
    case GL_DOUBLE_MAT2                              :return"GL_DOUBLE_MAT2";
    case GL_DOUBLE_MAT3                              :return"GL_DOUBLE_MAT3";
    case GL_DOUBLE_MAT4                              :return"GL_DOUBLE_MAT4";
    case GL_DOUBLE_MAT2x3                            :return"GL_DOUBLE_MAT2x3";
    case GL_DOUBLE_MAT2x4                            :return"GL_DOUBLE_MAT2x4";
    case GL_DOUBLE_MAT3x2                            :return"GL_DOUBLE_MAT3x2";
    case GL_DOUBLE_MAT3x4                            :return"GL_DOUBLE_MAT3x4";
    case GL_DOUBLE_MAT4x2                            :return"GL_DOUBLE_MAT4x2";
    case GL_DOUBLE_MAT4x3                            :return"GL_DOUBLE_MAT4x3";
    case GL_SAMPLER_1D                               :return"GL_SAMPLER_1D";
    case GL_SAMPLER_2D                               :return"GL_SAMPLER_2D";
    case GL_SAMPLER_3D                               :return"GL_SAMPLER_3D";
    case GL_SAMPLER_CUBE                             :return"GL_SAMPLER_CUBE";
    case GL_SAMPLER_1D_SHADOW                        :return"GL_SAMPLER_1D_SHADOW";
    case GL_SAMPLER_2D_SHADOW                        :return"GL_SAMPLER_2D_SHADOW";
    case GL_SAMPLER_1D_ARRAY                         :return"GL_SAMPLER_1D_ARRAY";
    case GL_SAMPLER_2D_ARRAY                         :return"GL_SAMPLER_2D_ARRAY";
    case GL_SAMPLER_1D_ARRAY_SHADOW                  :return"GL_SAMPLER_1D_ARRAY_SHADOW";
    case GL_SAMPLER_2D_ARRAY_SHADOW                  :return"GL_SAMPLER_2D_ARRAY_SHADOW";
    case GL_SAMPLER_2D_MULTISAMPLE                   :return"GL_SAMPLER_2D_MULTISAMPLE";
    case GL_SAMPLER_2D_MULTISAMPLE_ARRAY             :return"GL_SAMPLER_2D_MULTISAMPLE_ARRAY";
    case GL_SAMPLER_CUBE_SHADOW                      :return"GL_SAMPLER_CUBE_SHADOW";
    case GL_SAMPLER_BUFFER                           :return"GL_SAMPLER_BUFFER";
    case GL_SAMPLER_2D_RECT                          :return"GL_SAMPLER_2D_RECT";
    case GL_SAMPLER_2D_RECT_SHADOW                   :return"GL_SAMPLER_2D_RECT_SHADOW";
    case GL_INT_SAMPLER_1D                           :return"GL_INT_SAMPLER_1D";
    case GL_INT_SAMPLER_2D                           :return"GL_INT_SAMPLER_2D";
    case GL_INT_SAMPLER_3D                           :return"GL_INT_SAMPLER_3D";
    case GL_INT_SAMPLER_CUBE                         :return"GL_INT_SAMPLER_CUBE";
    case GL_INT_SAMPLER_1D_ARRAY                     :return"GL_INT_SAMPLER_1D_ARRAY";
    case GL_INT_SAMPLER_2D_ARRAY                     :return"GL_INT_SAMPLER_2D_ARRAY";
    case GL_INT_SAMPLER_2D_MULTISAMPLE               :return"GL_INT_SAMPLER_2D_MULTISAMPLE";
    case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY         :return"GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY";
    case GL_INT_SAMPLER_BUFFER                       :return"GL_INT_SAMPLER_BUFFER";
    case GL_INT_SAMPLER_2D_RECT                      :return"GL_INT_SAMPLER_2D_RECT";
    case GL_UNSIGNED_INT_SAMPLER_1D                  :return"GL_UNSIGNED_INT_SAMPLER_1D";
    case GL_UNSIGNED_INT_SAMPLER_2D                  :return"GL_UNSIGNED_INT_SAMPLER_2D";
    case GL_UNSIGNED_INT_SAMPLER_3D                  :return"GL_UNSIGNED_INT_SAMPLER_3D";
    case GL_UNSIGNED_INT_SAMPLER_CUBE                :return"GL_UNSIGNED_INT_SAMPLER_CUBE";
    case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY            :return"GL_UNSIGNED_INT_SAMPLER_1D_ARRAY";
    case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY            :return"GL_UNSIGNED_INT_SAMPLER_2D_ARRAY";
    case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE      :return"GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE";
    case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:return"GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY";
    case GL_UNSIGNED_INT_SAMPLER_BUFFER              :return"GL_UNSIGNED_INT_SAMPLER_BUFFER";
    case GL_UNSIGNED_INT_SAMPLER_2D_RECT             :return"GL_UNSIGNED_INT_SAMPLER_2D_RECT";
    default                                          :return"unknown";
  }
}

CComputeSides::CComputeSides(SAdjecency*Adj,
    unsigned WorkGroupSize,bool CullSides){
  this->_workGroupSize=WorkGroupSize;
  this->_adjacency=Adj;


  //input
#ifdef FAST_ADJACENCY
  Adjacency*ad=this->_adjacency->adj;
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
  /*
     for(unsigned e=0;e<this->_nofEdges;++e){
     for(unsigned v=0;v<NumVer;++v){
     for(unsigned i=0;i<4;++i)
     std::cerr<<Ptr[(e*NumVer+v)*4+i]<<" ";
     std::cerr<<std::endl;
     }
     std::cerr<<std::endl;
     }
  // */

  this->_input->unmap();
#else//FAST_ADJACENCY
  this->_maxMult  = this->_adjacency->MaxOpposite;
  this->_nofEdges = this->_adjacency->NumEdges   ;
  unsigned NumVer=2+1+this->_maxMult;
  this->_input=new ge::gl::BufferObject(sizeof(float)*4*NumVer*this->_nofEdges);
  float*Ptr=(float*)this->_input->map();

  for(unsigned e=0;e<this->_nofEdges;++e){
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

    for(unsigned o=0;o<this->_adjacency->EdgeOppositeNum[e];++o){
      for(int k=0;k<3;++k)
        Ptr[(e*NumVer+3+o)*4+k]=
          this->_adjacency->Points[this->_adjacency->EdgeOpposite[e][o]*3+k];
      Ptr[(e*NumVer+3+o)*4+3]=1;
    }
    for(unsigned o=this->_adjacency->EdgeOppositeNum[e];o<this->_maxMult;++o){
      for(unsigned k=0;k<4;++k)
        Ptr[(e*NumVer+3+o)*4+k]=0;
    }
  }
  /*
     for(unsigned e=0;e<this->_adjacency->NumEdges;++e){
     for(unsigned v=0;v<NumVer;++v){
     for(unsigned i=0;i<4;++i)
     std::cerr<<Ptr[(e*NumVer+v)*4+i]<<" ";
     std::cerr<<std::endl;
     }
     std::cerr<<std::endl;
     }
  // */

  this->_input->unmap();
#endif//FAST_ADJACENCY

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

  std::cerr<<this->_computeProgram->uniformsToStr()<<std::endl;
  GLint numBuffers;
  glGetProgramInterfaceiv(this->_computeProgram->getId(),GL_BUFFER_VARIABLE,GL_ACTIVE_RESOURCES,&numBuffers);
  std::cerr<<"GL_ACTIVE_RESOURCES: "<<numBuffers<<std::endl;
  GLint maxBufferLength;
  glGetProgramInterfaceiv(this->_computeProgram->getId(),GL_BUFFER_VARIABLE,GL_MAX_NAME_LENGTH,&maxBufferLength);
  std::cerr<<"GL_MAX_NAME_LENGTH: "<<maxBufferLength<<std::endl;

  char*bufferName=new char[maxBufferLength];

  for(GLint b=0;b<numBuffers;++b){
    glGetProgramResourceName(this->_computeProgram->getId(),GL_BUFFER_VARIABLE,b,maxBufferLength,NULL,bufferName);
    std::string name=std::string(bufferName);
    std::size_t pos=name.find("[0]");
    if(pos!=std::string::npos)name=name.substr(0,pos);
    std::cerr<<name<<std::endl;
    GLuint index=glGetProgramResourceIndex(this->_computeProgram->getId(),GL_BUFFER_VARIABLE,name.c_str());
    std::cerr<<"index: "<<index<<std::endl;
    const GLenum props[]={
      GL_TYPE,
      GL_ARRAY_SIZE,
      GL_OFFSET,
      GL_BLOCK_INDEX,
      GL_ARRAY_STRIDE,
      GL_MATRIX_STRIDE,
      GL_IS_ROW_MAJOR,
      GL_REFERENCED_BY_VERTEX_SHADER,
      GL_REFERENCED_BY_TESS_CONTROL_SHADER,
      GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
      GL_REFERENCED_BY_GEOMETRY_SHADER,
      GL_REFERENCED_BY_FRAGMENT_SHADER,
      GL_REFERENCED_BY_COMPUTE_SHADER,
      GL_TOP_LEVEL_ARRAY_SIZE,
      GL_TOP_LEVEL_ARRAY_STRIDE
    };
    std::string propsNames[]={
      "GL_TYPE",
      "GL_ARRAY_SIZE",
      "GL_OFFSET",
      "GL_BLOCK_INDEX",
      "GL_ARRAY_STRIDE",
      "GL_MATRIX_STRIDE",
      "GL_IS_ROW_MAJOR",
      "GL_REFERENCED_BY_VERTEX_SHADER",
      "GL_REFERENCED_BY_TESS_CONTROL_SHADER",
      "GL_REFERENCED_BY_TESS_EVALUATION_SHADER",
      "GL_REFERENCED_BY_GEOMETRY_SHADER",
      "GL_REFERENCED_BY_FRAGMENT_SHADER",
      "GL_REFERENCED_BY_COMPUTE_SHADER",
      "GL_TOP_LEVEL_ARRAY_SIZE",
      "GL_TOP_LEVEL_ARRAY_STRIDE"
    };
    const unsigned nofProps=sizeof(props)/sizeof(const GLenum);
    GLint propBuffer[nofProps];
    GLint propLenght;

    glGetProgramResourceiv(this->_computeProgram->getId(),GL_BUFFER_VARIABLE,b,nofProps,props,sizeof(propBuffer)/sizeof(GLint),&propLenght,propBuffer);
    for(unsigned p=0;p<nofProps;++p){
      if(props[p]==GL_TYPE)std::cerr<<"  "<<propsNames[p]<<" : "<<translateUniformType(propBuffer[p])<<std::endl;
      else std::cerr<<"  "<<propsNames[p]<<" : "<<propBuffer[p]<<std::endl;
    }



  }

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
  this->_computeProgram->set("NumEdge",this->_nofEdges);
  this->_computeProgram->set("LightPosition",1,glm::value_ptr(Light->position));
  this->_computeProgram->set("mvp",1,GL_FALSE,(const float*)mvp);
  this->_input  ->bindBase(GL_SHADER_STORAGE_BUFFER,0);
  this->_output ->bindBase(GL_SHADER_STORAGE_BUFFER,1);
  this->_counter->bindBase(GL_SHADER_STORAGE_BUFFER,2);

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

