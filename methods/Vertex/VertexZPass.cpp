#include"VertexZPass.h"

#include<geGL/VertexArrayObject.h>

const std::string vertexZPassVariables[]={
  "emptyVAO",
  "shaderDirectory",
  "adjacency",
  "light",
  "shadowMask",
  "gbuffer.fbo",
  "gbuffer.stencil"
  "measure.stencil.drawSides",
  "measure.stencil.blit2ShadowMask",
  "camera",
  "context.version",
  "context.profile",
  "vertexMethod.program.VERTEX_ATTRIB_NAME",
  "vertexMethod.program.LIGHT_UNIFORM_NAME",
  "vertexMethod.program.MATRIX_UNIFORM_NAME",
  "vertexMethod.program.MODEL_MATRIX_UNIFORM_NAME",
  "vertexMethod.program.VIEW_MATRIX_UNIFORM_NAME",
  "vertexMethod.program.PROJECTION_MATRIX_UNIFORM_NAME",
  "vertexMethod.program.SIDE_ID_INPUT_NAME",
  "vertexMethod.program.VERTEX_ID_ATTRIB_NAME",
  "vertexMethod.program.MAX_MULTIPLICITY",
  "vertexMethod.program.USE_INSTANCING",
  "vertexMethod.program.USE_MULTIDRAW",
  "vertexMethod.program.UNIVERSAL",
  "vertexMethod.program.USE_INSTANCE_ID_INSTEAD_OF_SIDE_ID",
  "vertexMethod.program.SEPARATE_MATRICES"
};

const unsigned vertexZPassNumVariables=sizeof(vertexZPassVariables)/sizeof(std::string);

enum vertexZPassVariableNames{
  EMPTYVAO=0,
  SHADERDIRECTORY,
  ADJACENCY,
  LIGHT,
  SHADOWMASK,
  GBUFFER_FBO,
  GBUFFER_STENCIL,
  MEASURE_STENCIL_DRAWSIDES,
  MEASURE_STENCIL_BLIT2SHADOWMASK,
  CAMERA,
  VERSION,
  PROFILE,
  VERTEX_ATTRIB_NAME,
  LIGHT_UNIFORM_NAME,
  MATRIX_UNIFORM_NAME,
  MODEL_MATRIX_UNIFORM_NAME,
  VIEW_MATRIX_UNIFORM_NAME,
  PROJECTION_MATRIX_UNIFORM_NAME,
  SIDE_ID_INPUT_NAME,
  VERTEX_ID_ATTRIB_NAME,
  MAX_MULTIPLICITY,
  USE_INSTANCING,
  USE_MULTIDRAW,
  UNIVERSAL,
  USE_INSTANCE_ID_INSTEAD_OF_SIDE_ID,
  SEPARATE_MATRICES
};

#if SEPARATE_MATRICES != vertexZPassVariableNames
#error vertexZPassVariableNames and vertexZPassVariables are different
#endif

#define GET(fce,var,def) this->_simulationData->fce(vertexZPassVariables[var],def)
#define GETBOOL(var)   GET(getBool,var,false)
#define GETOBJECT(var) GET(getObject,var,NULL)
#define GETSTRING(var) GET(getString,var,"")
#define GETGAUGE(var)  GET(getGauge,var,NULL)
#define GETADJACENCY   ((SAdjecency*)GETOBJECT(ADJACENCY))
#define GETLIGHT       GET(getLight,LIGHT,NULL)
#define GETCAMERA      ((ge::util::CameraObject*)GETOBJECT(CAMERA))
#define GETFBO         ((ge::gl::FramebufferObject*)GETOBJECT(GBUFFER_FBO))
#define GETUINT(var)   GET(getUint,var,0)
#define GETTEXTURE(var) ((ge::gl::TextureObject*)GETOBJECT(var))
#define GETVAO(var)     ((ge::gl::VertexArrayObject*)GETOBJECT(var))

#define CHANGED(var) this->_changed[vertexZPassVariables[var]]



//UNIVERSAL  - nonsupported
//MULTIDRAW  - vbo, vao, program
//INSTANCING - vbo, vao, program
//adjacency  - vbo, vao
//

unsigned VertexZPass::getNofDependentVariables(){
  return sizeof(vertexZPassVariables)/sizeof(std::string);
}
std::string VertexZPass::getDependentVariable(unsigned id){
  return vertexZPassVariables[id];
}
void VertexZPass::update(){
  typedef void(VertexZPass::*UPDATEFCE)();
  UPDATEFCE updateRoutines[]={
    &VertexZPass::_updateData,
    &VertexZPass::_updateFBO,
    &VertexZPass::_updateProgram,
    &VertexZPass::_setMeasureDrawSides,
    &VertexZPass::_setMeasureBlit
  };
#define SETBIT(x) (1<<x)
#define GETBIT(x,y) ((x>>y)&1)

  enum updateRoutines{
    DATA=0,
    FBO,
    PROGRAM,
    DRAWSIDE_GAUGE,
    BLIT_GAUGE
  };

  static const unsigned var2Method[]={
  ADJACENCY                      ,SETBIT(DATA)|SETBIT(PROGRAM),
  SHADOWMASK                     ,SETBIT(FBO)                 ,
  MEASURE_STENCIL_DRAWSIDES      ,SETBIT(DRAWSIDE_GAUGE)      ,
  MEASURE_STENCIL_BLIT2SHADOWMASK,SETBIT(BLIT_GAUGE)          ,
  VERSION                        ,SETBIT(PROGRAM)             ,
  PROFILE                        ,SETBIT(PROGRAM)             ,
  USE_INSTANCING                 ,SETBIT(DATA)|SETBIT(PROGRAM),
  USE_MULTIDRAW                  ,SETBIT(DATA)|SETBIT(PROGRAM),
  UNIVERSAL                      ,SETBIT(DATA)|SETBIT(PROGRAM),
  };
  unsigned vertexZPassUpdate[vertexZPassNumVariables];
  for(unsigned v=0;v<vertexZPassNumVariables;++v)
    vertexZPassUpdate[v]=0;

  for(unsigned vs=0u;vs<sizeof(var2Method)/sizeof(unsigned);vs+=2u)
    vertexZPassUpdate[var2Method[vs*2u+0u]]=var2Method[vs*2u+1u];

#define UPDATE_METHODS (sizeof(updateRoutines)/sizeof(UPDATEFCE))

  bool toUpdate[UPDATE_METHODS];
  for(unsigned u=0;u<UPDATE_METHODS;++u)toUpdate[u]=false;

  for(unsigned i=0;i<sizeof(vertexZPassVariables)/sizeof(std::string);++i)
    if(CHANGED(EMPTYVAO+i))
      for(unsigned u=0;u<UPDATE_METHODS;++u)
        toUpdate[u]|=GETBIT(vertexZPassUpdate[i],u);

  for(unsigned u=0;u<UPDATE_METHODS;++u)
    if(toUpdate[u])(this->*updateRoutines[u])();
}

void VertexZPass::_updateData(){
  this->_deleteUniversal();
  this->_deleteSpecific();
  this->_createUniversal();
  /*
     if(this->_universal)this->_createUniversal();
     else this->_createSpecific();
     */
}

void VertexZPass::_updateFBO(){
  if(this->_shadowMaskFBO)delete this->_shadowMaskFBO;
  this->_shadowMaskFBO = new ge::gl::FramebufferObject();
  this->_shadowMaskFBO->attachStencilTexture(GETTEXTURE(GBUFFER_STENCIL)->getId());
  this->_shadowMaskFBO->attachColorTexture(GL_COLOR_ATTACHMENT0,GETTEXTURE(SHADOWMASK)->getId());
}

void VertexZPass::_createUniversal(){
  unsigned numSides;
  unsigned numVertices;

  if(GETBOOL(USE_MULTIDRAW))numSides=1;
  else numSides=GETADJACENCY->MaxOpposite;

  if(GETBOOL(USE_INSTANCING))numVertices=1;
  else numVertices=6;

  this->_vboSideU=new ge::gl::BufferObject(
      sizeof(float)*4*(GETADJACENCY->MaxOpposite+2)*numVertices*numSides*GETADJACENCY->NumEdges);

  float*Ptr=(float*)this->_vboSideU->map();
  for(unsigned e=0;e<GETADJACENCY->NumEdges;++e){//loop over edges
    for(unsigned s=0;s<numSides;++s){//loop over sides
      for(unsigned v=0;v<numVertices;++v){//loop over vertices of side
        unsigned Base=((e*numSides+s)*numVertices+v)*(GETADJACENCY->MaxOpposite+2);

        Ptr[(Base+0)*4+0]=GETADJACENCY->Points[GETADJACENCY->Edges[e][0]*3+0];
        Ptr[(Base+0)*4+1]=GETADJACENCY->Points[GETADJACENCY->Edges[e][0]*3+1];
        Ptr[(Base+0)*4+2]=GETADJACENCY->Points[GETADJACENCY->Edges[e][0]*3+2];

        unsigned Num=GETADJACENCY->MaxOpposite;

        if(GETADJACENCY->EdgeOppositeNum[e]<GETADJACENCY->MaxOpposite)
          Num=GETADJACENCY->EdgeOppositeNum[e];
        Ptr[(Base+0)*4+3]=Num;

        Ptr[(Base+1)*4+0]=GETADJACENCY->Points[GETADJACENCY->Edges[e][1]*3+0];
        Ptr[(Base+1)*4+1]=GETADJACENCY->Points[GETADJACENCY->Edges[e][1]*3+1];
        Ptr[(Base+1)*4+2]=GETADJACENCY->Points[GETADJACENCY->Edges[e][1]*3+2];

        unsigned VertexIDCCW;
        if(v>2)VertexIDCCW=v-2;//3,4,5 -> 1,2,3
        else VertexIDCCW=2-v;//0,1,2 -> 2,1,0
        unsigned VertexIDCW;
        if(v>2)VertexIDCW=6-v;//3,4,5 -> 3,2,1
        else VertexIDCW=v;//0,1,2 -> 0,1,2
        Ptr[(Base+1)*4+3]=(s*4+VertexIDCW)*4+VertexIDCCW;

        for(unsigned o=0;o<Num;++o){//loop over opposite vertices
          Ptr[(Base+2+o)*4+0]=GETADJACENCY->Points[GETADJACENCY->EdgeOpposite[e][o]*3+0];
          Ptr[(Base+2+o)*4+1]=GETADJACENCY->Points[GETADJACENCY->EdgeOpposite[e][o]*3+1];
          Ptr[(Base+2+o)*4+2]=GETADJACENCY->Points[GETADJACENCY->EdgeOpposite[e][o]*3+2];
          Ptr[(Base+2+o)*4+3]=1;
        }
        for(unsigned o=Num;o<GETADJACENCY->MaxOpposite;++o)
          for(int i=0;i<4;++i)
            Ptr[(Base+2+o)*4+i]=0;
      }
    }
  }
  this->_vboSideU->unmap();

  this->_vaoSideU=new ge::gl::VertexArrayObject();

  unsigned divisor=0;
  if(GETBOOL(USE_INSTANCING))divisor=GETADJACENCY->MaxOpposite;

  this->_vaoSideU->addAttrib(this->_vboSideU,0,4,GL_FLOAT,
      sizeof(float)*4*(GETADJACENCY->MaxOpposite+2),(GLvoid*)(sizeof(float)*4*0),
      false,divisor);
  this->_vaoSideU->addAttrib(this->_vboSideU,1,4,GL_FLOAT,
      sizeof(float)*4*(GETADJACENCY->MaxOpposite+2),(GLvoid*)(sizeof(float)*4*1),
      false,divisor);
  for(unsigned a=0;a<GETADJACENCY->MaxOpposite;++a)
    this->_vaoSideU->addAttrib(this->_vboSideU,2+a,4,GL_FLOAT,
        sizeof(float)*4*(GETADJACENCY->MaxOpposite+2),(GLvoid*)(sizeof(float)*4*(2+a)),
        false,divisor);
}
void VertexZPass::_createSpecific(){
  //TODO
}

void VertexZPass::_deleteUniversal(){
  for(unsigned i=0;i<this->_nofS;++i){
    delete this->_vboSideS[i];
    delete this->_vaoSideS[i];
    this->_vboSideS[i]=0;
    this->_vaoSideS[i]=0;
  }
  if(this->_vboSideS)delete[]this->_vboSideS;
  if(this->_vaoSideS)delete[]this->_vaoSideS;
  this->_vboSideS=NULL;
  this->_vaoSideS=NULL;
  this->_nofS=0;
}

void VertexZPass::_deleteSpecific(){
  if(this->_vboSideU)delete this->_vboSideU;
  if(this->_vaoSideU)delete this->_vaoSideU;
  this->_vboSideU=NULL;
  this->_vaoSideU=NULL;
}


void VertexZPass::_updateProgram(){
  if(this->_programSide)delete this->_programSide;
  this->_programSide = new ge::gl::ProgramObject(
      GETSTRING(SHADERDIRECTORY)+"methods/Vertex/side.vp",
      ge::gl::ShaderObject::define("MAX_MULTIPLICITY",GETADJACENCY->MaxOpposite)+
      this->_simulationData->define("stencilMethos")+
      this->_simulationData->define("vertexMethod.program")+
      "",
      GETSTRING(SHADERDIRECTORY)+"methods/Vertex/side.fp",
      this->_simulationData->define("stencilMethos")+
      this->_simulationData->define("vertexMethod.program")+
      "",
      GETUINT(VERSION),
      GETSTRING(PROFILE));
}


VertexZPass::VertexZPass(simulation::SimulationData*data):simulation::SimulationObject(data){
  this->_simulationData->registerUser(this);
  this->_vaoSideU    = NULL;
  this->_vboSideU    = NULL;
  this->_vaoSideS    = NULL;
  this->_vboSideS    = NULL;
  this->_programSide = NULL;

  this->_shadowMaskFBO    = NULL;
  this->_createShadowMask = NULL;

  this->_measureDrawSides = NULL;
  this->_measureBlit      = NULL;

  this->_updateData();
  this->_updateProgram();
  this->_updateFBO();

  this->_createShadowMask = new ge::gl::ProgramObject(
      GETSTRING(SHADERDIRECTORY)+"methods/Vertex/blit.vp",
      GETSTRING(SHADERDIRECTORY)+"methods/Vertex/blit.gp",
      GETSTRING(SHADERDIRECTORY)+"methods/Vertex/blit.fp");
}

VertexZPass::~VertexZPass(){
  this->_simulationData->unregisterUser(this);
  delete this->_programSide;
  this->_deleteUniversal();
  this->_deleteSpecific();
  if(this->_measureDrawSides)delete this->_measureDrawSides;
  if(this->_measureBlit)delete this->_measureBlit;
  delete this->_createShadowMask;
}

void VertexZPass::_drawStencil(){
  GETFBO->bind();
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS,0,0);
  glStencilOpSeparate(GL_FRONT,GL_KEEP,GL_KEEP,GL_INCR_WRAP);
  glStencilOpSeparate(GL_BACK ,GL_KEEP,GL_KEEP,GL_DECR_WRAP);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_FALSE);
  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);

  if(this->_measureDrawSides)this->_measureDrawSides->begin();

  glm::mat4 mvp=GETCAMERA->getProjection()*GETCAMERA->getView();

  this->_programSide->use();
  this->_programSide->set("lightPosition",1,glm::value_ptr(GETLIGHT->position));
  this->_programSide->set("mvp",1,GL_FALSE,(const float*)glm::value_ptr(mvp));

  if(GETBOOL(USE_MULTIDRAW)){
    if(GETBOOL(USE_INSTANCING)){
      for(unsigned i=0;i<GETADJACENCY->MaxOpposite;++i){
        this->_programSide->set("side",i);
        this->_vaoSideU->bind();
        glDrawArraysInstanced(GL_TRIANGLES,0,6,GETADJACENCY->NumEdges);
        this->_vaoSideU->unbind();
      }
    }else{
      for(unsigned i=0;i<GETADJACENCY->MaxOpposite;++i){
        this->_programSide->set("side",i);
        this->_vaoSideU->bind();
        glDrawArrays(GL_TRIANGLES,0,GETADJACENCY->NumEdges*3*2);
        this->_vaoSideU->unbind();
      }
    }
  }else{
    if(GETBOOL(USE_INSTANCING)){
      this->_vaoSideU->bind();
      glDrawArraysInstanced(GL_TRIANGLES,0,6,GETADJACENCY->NumEdges*GETADJACENCY->MaxOpposite);
      this->_vaoSideU->unbind();
    }else{
      this->_vaoSideU->bind();
      glDrawArrays(GL_TRIANGLES,0,GETADJACENCY->NumEdges*3*2*GETADJACENCY->MaxOpposite);
      this->_vaoSideU->unbind();
    }
  }

  if(this->_measureDrawSides){
    this->_measureDrawSides->begin();
    this->_drawSidesGauge->insert(this->_measureDrawSides->getui64()/1000000000.f);
  }
  GETFBO->unbind();
}


void VertexZPass::createShadowMask(){
  this->_drawStencil();
  this->_createShadowMask->use();
  this->_shadowMaskFBO->bind();
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_EQUAL,0,0xff);
  glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
  glDisable(GL_DEPTH_TEST);
  GETVAO(EMPTYVAO)->bind();
  glDrawArrays(GL_POINTS,0,1);
  GETVAO(EMPTYVAO)->unbind();
  this->_shadowMaskFBO->unbind();
}

void VertexZPass::_setMeasureDrawSides(){
  this->_drawSidesGauge = GETGAUGE(MEASURE_STENCIL_DRAWSIDES);
  if(this->_drawSidesGauge){
    if(!this->_measureDrawSides)
      this->_measureDrawSides = new ge::gl::AsynchronousQueryObject(
          GL_TIME_ELAPSED,GL_QUERY_RESULT_NO_WAIT,ge::gl::AsynchronousQueryObject::UINT64);
  }
}

void VertexZPass::_setMeasureBlit(){
  this->_blitGauge = GETGAUGE(MEASURE_STENCIL_BLIT2SHADOWMASK);
  if(this->_blitGauge){
    if(!this->_measureBlit)
      this->_measureBlit = new ge::gl::AsynchronousQueryObject(
          GL_TIME_ELAPSED,GL_QUERY_RESULT_NO_WAIT,ge::gl::AsynchronousQueryObject::UINT64);
  }
}


