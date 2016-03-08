#pragma once

#include"../app/fastAdjacency.h"
#include"../app/core.h"

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
//#define GETADJACENCY   ((SAdjecency*)GETOBJECT(ADJACENCY))
#define GETFASTADJACENCY ((Adjacency*)GETOBJECT(FASTADJACENCY))
#define GETLIGHT       GET(getLight,LIGHT,NULL)
#define GETCAMERA      ((ge::util::CameraObject*)GETOBJECT(CAMERA))
#define GETFBO         ((ge::gl::FramebufferObject*)GETOBJECT(GBUFFER_FBO))
#define GETUINT(var)   GET(getUint,var,0)
#define GETUVEC2(var)  GET(getUVec2,var,glm::uvec2(0u,0u))
#define GETVEC3(var)   GET(getVec3,var,glm::vec3(0.f))
#define GETFLOAT(var)  GET(getFloat,var,0.f)
#define GETTEXTURE(var) ((ge::gl::TextureObject*)GETOBJECT(var))
#define GETVAO(var)     ((ge::gl::VertexArrayObject*)GETOBJECT(var))
#define GETBUFFER(var)     ((ge::gl::BufferObject*)GETOBJECT(var))

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

template<typename T>
void insertType(std::map<std::string,std::vector<T>>&data,std::string n,T t){
  if(!data.count(n))data.insert(std::pair<std::string,std::vector<T>>(n,std::vector<T>()));
  data[n].push_back(t);
}

template<typename T,typename... Args>
void insertType(std::map<std::string,std::vector<T>>&data,std::string n,T t,Args... args);

template<typename T,typename... Args>
void insertType(std::map<std::string,std::vector<T>>&data,std::string n,std::string n2,T t,Args... args){
  insertType(data,n2,t,args...);
}

template<typename T,typename... Args>
void insertType(std::map<std::string,std::vector<T>>&data,std::string n,T t,Args... args){
  insertType(data,n,t);
  insertType(data,n,args...);
} 



template<typename RET,typename ROUTINE,typename... Args>
static RET createStaticUpdateData(Args... args){
  std::cerr<<"createStaticUpdateData\n";
  std::map<const char*,std::vector<ROUTINE>>varsToRoutines;
  argsToMapOfVectors(varsToRoutines,args...);

  std::vector<ROUTINE>routines;
  filterArgsToVector(routines,args...);

  std::vector<const char*>vars;
  filterArgsToVector(vars,args...);

  std::vector<unsigned>routineIndices;
  std::vector<ROUTINE>uniqueRoutines;
  unsigned routineCounter=0;
  for(unsigned i=0;i<routines.size();++i){
    bool alreadyInserted=false;
    for(unsigned j=0;j<uniqueRoutines.size();++j)
      if(uniqueRoutines[j]==routines[i]){
        alreadyInserted=true;
        routineIndices.push_back(j);
        break;
      }
    if(alreadyInserted)continue;
    uniqueRoutines.push_back(routines[i]);
    routineIndices.push_back(routineCounter++);
  }

  std::map<const char*,std::vector<unsigned>>varsToRoutineIndices;
  unsigned indexToRoutineIndices=0;
  for(unsigned v=0;v<vars.size();++v){
    std::vector<unsigned>indices;
    for(unsigned r=0;r<varsToRoutines[vars[v]].size();++r){
      indices.push_back(routineIndices[indexToRoutineIndices++]);
    }
    varsToRoutineIndices[vars[v]]=indices;
  }

  RET ud;
  ud.vars = vars;
  ud.vars2RoutineIndex = varsToRoutineIndices;
  ud.updateRoutines = uniqueRoutines;
  return ud;
}

#define DEF_UPDATEDATA(CLASS_NAME)\
  struct UpdateData{\
    std::vector<const char*>vars;\
    std::vector<void(CLASS_NAME::*)()>updateRoutines;\
    std::map<const char*,std::vector<unsigned>>vars2RoutineIndex;\
  }static const _updateData;\
  std::vector<bool>_routines2Call

#define DEF_VARSANDROUTINES(...)\
void CLASS_NAME::update(){\
  std::cerr<<"update\n";\
  for(unsigned i=0;i<this->_updateData.vars.size();++i){\
    if(!this->_changed.count(this->_updateData.vars[i]))continue;\
    if(!this->_changed[this->_updateData.vars[i]])continue;\
    if(!this->_updateData.vars2RoutineIndex.count(this->_updateData.vars[i]))continue;\
    for(unsigned j=0;j<this->_updateData.vars2RoutineIndex.find(this->_updateData.vars[i])->second.size();++j)\
      this->_routines2Call[this->_updateData.vars2RoutineIndex.find(this->_updateData.vars[i])->second[j]]=true;\
    this->_changed[this->_updateData.vars[i]]=false;\
  }\
  for(unsigned i=0;i<this->_routines2Call.size();++i){\
    if(!this->_routines2Call[i])continue;\
    (this->*_updateData.updateRoutines[i])();\
    this->_routines2Call[i]=false;\
  }\
  this->BASECLASS_NAME::update();\
}\
\
unsigned CLASS_NAME::getNofDependentVariables(){\
  std::cerr<<"getNofDependentVariables\n";\
  return this->_updateData.vars.size()+this->BASECLASS_NAME::getNofDependentVariables();\
}\
\
std::string CLASS_NAME::getDependentVariable(unsigned var){\
  std::cerr<<"getDependentVariable\n";\
  if(var>=this->_updateData.vars.size())return this->BASECLASS_NAME::getDependentVariable(var-this->_updateData.vars.size());\
  return this->_updateData.vars[var];\
}\
\
const CLASS_NAME::UpdateData CLASS_NAME::_updateData = createStaticUpdateData<CLASS_NAME::UpdateData,void(CLASS_NAME::*)()>(__VA_ARGS__)

#define DEF_CONSTRUCTOR \
  for(unsigned i=0;i<this->_updateData.vars.size();++i)\
    this->_changed[this->_updateData.vars[i]]=false;\
  for(unsigned i=0;i<this->_updateData.updateRoutines.size();++i)\
    this->_routines2Call.push_back(false)

