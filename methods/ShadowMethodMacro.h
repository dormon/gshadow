#pragma once

#define JOIN___(x,y) x##y
#define JOIN__(x,y) JOIN___(x,y)
#define JOIN_(x,y) JOIN__(x,y)
#define JOIN(x,y) JOIN_(x,y)

#ifndef CLASSNAME
#define CLASSNAME ComputeGeometry
#endif//CLASSNAME

#ifndef VARS
#define VARS JOIN(CLASSNAME,VariableNames)
#endif//VARS

#define DEFVARSSTART const std::string VARS[]={
#define DEFVARSEND };

#define VARSNUM (sizeof(VARS)/sizeof(std::string))

#define DEFVARSIDSTART enum JOIN(CLASSNAME,ID){
#define DEFVARSIDEND };

#define GET(fce,var,def) this->_simulationData->fce(VARS[var],def)
#define GETBOOL(var)   GET(getBool,var,false)
#define GETOBJECT(var) GET(getObject,var,NULL)
#define GETSTRING(var) GET(getString,var,"")
#define GETGAUGE(var)  GET(getGauge,var,NULL)
#define GETADJACENCY   ((SAdjecency*)GETOBJECT(ADJACENCY))
#define GETLIGHT       GET(getLight,LIGHT,NULL)
#define GETCAMERA      ((ge::util::CameraObject*)GETOBJECT(CAMERA))
#define GETFBO         ((ge::gl::FramebufferObject*)GETOBJECT(GBUFFER_FBO))
#define GETUINT(var)   GET(getUint,var,0)
#define GETUVEC2(var)  GET(getUVec2,var,glm::uvec2(0u,0u))
#define GETVEC3(var)   GET(getVec3,var,glm::vec3(0.f))
#define GETFLOAT(var)  GET(getFloat,var,0.f)
#define GETTEXTURE(var) ((ge::gl::TextureObject*)GETOBJECT(var))
#define GETVAO(var)     ((ge::gl::VertexArrayObject*)GETOBJECT(var))
#define GETGPUGAUGE(var) GET(getGpuGauge,var,NULL)

#define CHANGED(var) this->_changed[VARS[var]]

#define DEFGETNOFDEP unsigned CLASSNAME::getNofDependentVariables(){return VARSNUM;}
#define DEFGETDEP    std::string CLASSNAME::getDependentVariable(unsigned id){return VARS[id];}

#define SETBIT(x) (1<<x)
#define GETBIT(x,y) ((x>>y)&1)

template<typename T>
unsigned getMask(T t){return (1<<t);}
template<typename T,typename... Args>
unsigned getMask(T t,Args... args){return (1<<t)|getMask(args...);}

#define GETCHECKSTRING(name) std::string(#name)+" : "+VARS[name]

#define VAR2UPDATEMETHOD JOIN(CLASSNAME,Var2UpdateMethod)
#define UPDATEROUTINES JOIN(CLASSNAME,UpdateRoutines)

#define DEFUPDATEROUTINESSTART\
  typedef void(CLASSNAME::*FCEPOINTER)();\
  const FCEPOINTER UPDATEROUTINES[]={

#define DEFUPDATEROUTINESEND\
  };

#define UPDATEROUTINESNUM (sizeof(UPDATEROUTINES)/sizeof(FCEPOINTER))

#define FCEPOINTER JOIN(CLASSNAME,FcePointer)

#define GETPOINTER(name) &CLASSNAME::name


#define DEFVAR2UPDATESTART\
  const unsigned VAR2UPDATEMETHOD[]={

#define DEFVAR2UPDATEEND\
  };

#define DEFUPDATEROUTINEIDSTART\
  enum JOIN(CLASSNAME,RoutineID){
#define DEFUPDATEROUTINEIDEND\
  };

#define DEFUPDATE\
  void CLASSNAME::update(){\
    unsigned updateVar[VARSNUM];\
    for(unsigned v=0;v<VARSNUM;++v)\
      updateVar[v]=0;\
    for(unsigned vs=0;vs<sizeof(VAR2UPDATEMETHOD)/sizeof(unsigned);vs+=2)\
      updateVar[VAR2UPDATEMETHOD[vs+0]]=VAR2UPDATEMETHOD[vs+1];\
    bool toUpdate[UPDATEROUTINESNUM];\
    for(unsigned u=0;u<UPDATEROUTINESNUM;++u)toUpdate[u]=false;\
    for(unsigned i=0;i<VARSNUM;++i)\
      if(CHANGED(i)){\
        for(unsigned u=0;u<UPDATEROUTINESNUM;++u)\
          toUpdate[u]|=GETBIT(updateVar[i],u);\
        CHANGED(i)=false;\
      }\
    for(unsigned u=0;u<UPDATEROUTINESNUM;++u)\
      if(toUpdate[u])(this->*UPDATEROUTINES[u])();\
  }


