
#include<GL/gl.h>
#include<iostream>
#include<cstring>
#include<vector>
#include<map>
#include<set>
#include<algorithm>

#include <chrono>

#define ENUM_CLASS(name,...)\
  enum class name {\
    __VA_ARGS__\
  };\
std::ostream& operator<<(std::ostream& os, name val){\
  char s[]=#__VA_ARGS__;\
  char*ret = strtok(&s[0],",");\
  for(int i=0; i<(int)val;++i)\
    ret=strtok(NULL,",");\
  return os << ret;\
}

ENUM_CLASS(VertexSemantic,POSION,NORMAL,COORD)

template<typename TPtr>
void deleteSetNull(TPtr&b){
  if(b)delete b;b=NULL;
}

template<typename TPtr,typename...Args>
void deleteSetNull(TPtr&b,Args... args){
  if(b)delete b;b=NULL;
  deleteSetNull(args...);
}


template<typename T>
void filterArgsToVector(std::vector<T>&data,T t){
  data.push_back(t);
}

template<typename T,typename T2>
void filterArgsToVector(std::vector<T>&,T2){}

template<typename T,typename T2,typename... Args>
void filterArgsToVector(std::vector<T>&data,T2,Args... args);

template<typename T,typename... Args>
void filterArgsToVector(std::vector<T>&data,T t,Args... args){
  filterArgsToVector(data,t);
  filterArgsToVector(data,args...);
}

template<typename T,typename T2,typename... Args>
void filterArgsToVector(std::vector<T>&data,T2,Args... args){
  filterArgsToVector(data,args...);
}



template<typename KEY,typename VAL>
void argsToMapOfVectors(std::map<KEY,std::vector<VAL>>&data,KEY n){
  if(!data.count(n))data[n]=std::vector<VAL>();
}

template<typename KEY,typename VAL>
void argsToMapOfVectors(std::map<KEY,std::vector<VAL>>&data,KEY n,VAL t){
  if(!data.count(n))data[n]=std::vector<VAL>();
  data[n].push_back(t);
}

template<typename KEY,typename VAL,typename... Args>
void argsToMapOfVectors(std::map<KEY,std::vector<VAL>>&data,KEY n,VAL t,Args... args);

template<typename KEY,typename VAL,typename... Args>
void argsToMapOfVectors(std::map<KEY,std::vector<VAL>>&data,KEY n1,KEY n2,VAL t,Args... args){
  if(!data.count(n1))data[n1]=std::vector<VAL>();
  argsToMapOfVectors(data,n2,t,args...);
}

template<typename KEY,typename VAL,typename... Args>
void argsToMapOfVectors(std::map<KEY,std::vector<VAL>>&data,KEY n,Args... args){
  argsToMapOfVectors(data,n);
  argsToMapOfVectors(data,args...);
}

template<typename KEY,typename VAL,typename... Args>
void argsToMapOfVectors(std::map<KEY,std::vector<VAL>>&data,KEY n,VAL t,Args... args){
  argsToMapOfVectors(data,n,t);
  argsToMapOfVectors(data,n,args...);
}

class SimulationObject{
  protected:
    std::map<const char*,bool>_changed;
  public:
    SimulationObject();
    virtual void update();
    virtual unsigned getNofVars();
    virtual std::string getVar(unsigned var);
    void setVariableAsChanged(const char*name);
};

void SimulationObject::setVariableAsChanged(const char*name){
  this->_changed[name]=true;
}

SimulationObject::SimulationObject(){}
void SimulationObject::update(){}
unsigned SimulationObject::getNofVars(){return 0;}
std::string SimulationObject::getVar(unsigned){return"";}

template<typename RET,typename ROUTINE,typename... Args>
static RET createStaticUpdateData(Args... args){
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
unsigned CLASS_NAME::getNofVars(){\
  return this->_updateData.vars.size()+this->BASECLASS_NAME::getNofVars();\
}\
\
std::string CLASS_NAME::getVar(unsigned var){\
  if(var>=this->_updateData.vars.size())return this->BASECLASS_NAME::getVar(var-this->_updateData.vars.size());\
  return this->_updateData.vars[var];\
}\
\
const CLASS_NAME::UpdateData CLASS_NAME::_updateData = createStaticUpdateData<CLASS_NAME::UpdateData,void(CLASS_NAME::*)()>(__VA_ARGS__)

#define DEF_CONSTRUCTOR \
  for(unsigned i=0;i<this->_updateData.vars.size();++i)\
    this->_changed[this->_updateData.vars[i]]=false;\
  for(unsigned i=0;i<this->_updateData.updateRoutines.size();++i)\
    this->_routines2Call.push_back(false)


#define CLASS_NAME Shadow
#define BASECLASS_NAME SimulationObject

class Shadow: public SimulationObject{
  protected:
    int a;
    void basea(){a+=10;/*std::cerr<<"basea\n";*/}
    void baseb(){a+=20;/*std::cerr<<"baseb\n";*/}
    void basec(){a+=30;/*std::cerr<<"basec\n";*/}
    DEF_UPDATEDATA(Shadow);
  public:
    int get(){return this->a;}
    Shadow();
    virtual void update();
    virtual unsigned getNofVars();
    virtual std::string getVar(unsigned var);
};

DEF_VARSANDROUTINES(
    "baseva",&Shadow::basea,&Shadow::baseb,
    "basevb",&Shadow::baseb,&Shadow::basec);

Shadow::Shadow(){
  DEF_CONSTRUCTOR;
}

#undef CLASS_NAME
#undef BASECLASS_NAME
#define CLASS_NAME ShadowMapping
#define BASECLASS_NAME Shadow

class ShadowMapping: public Shadow{
  private:
    void ma(){a+=1;/*std::cerr<<"ma\n";*/}
    void mb(){a+=2;/*std::cerr<<"mb\n";*/}
    void mc(){a+=3;/*std::cerr<<"mc\n";*/}
    void md(){a+=4;/*std::cerr<<"md\n";*/}
    void me(){a+=5;/*std::cerr<<"me\n";*/}
    void mf(){a+=6;/*std::cerr<<"mf\n";*/}
    DEF_UPDATEDATA(ShadowMapping);
  public:
    ShadowMapping();
    virtual void update();
    virtual unsigned getNofVars();
    virtual std::string getVar(unsigned var);
};

DEF_VARSANDROUTINES(
    "va",&ShadowMapping::ma,&ShadowMapping::mb,
    "vb",&ShadowMapping::mc,&ShadowMapping::md,
    "vc",&ShadowMapping::me,&ShadowMapping::mf,
    "emplty",
    "asdasd",
    "asd","asdassd","dsds","qwe","efase","asdsadas","asdasdasd");

ShadowMapping::ShadowMapping():Shadow(){
  DEF_CONSTRUCTOR;
}

template<typename T>
void printv(std::vector<T>&data){
  for(unsigned i=0;i<data.size();++i)
    std::cerr<<data[i]<<" ";
  std::cerr<<std::endl;
}



template<typename KEY,typename VAL>
void a2m(std::map<KEY,std::vector<VAL>>&data,KEY n){
  if(!data.count(n))data[n]=std::vector<VAL>();
}

template<typename KEY,typename VAL>
void a2m(std::map<KEY,std::vector<VAL>>&data,KEY n,VAL t){
  if(!data.count(n))data.insert(std::pair<KEY,std::vector<VAL>>(n,std::vector<VAL>()));
  data[n].push_back(t);
}

template<typename KEY,typename VAL,typename... Args>
void a2m(std::map<KEY,std::vector<VAL>>&data,KEY n,VAL t,Args... args);

template<typename KEY,typename VAL,typename... Args>
void a2m(std::map<KEY,std::vector<VAL>>&data,KEY n1,KEY n2,VAL t,Args... args){
  if(!data.count(n1))data[n1]=std::vector<VAL>();
  a2m(data,n2,t,args...);
}

template<typename KEY,typename VAL,typename... Args>
void a2m(std::map<KEY,std::vector<VAL>>&data,KEY n,Args... args){
  a2m(data,n);
  a2m(data,args...);
}

template<typename KEY,typename VAL,typename... Args>
void a2m(std::map<KEY,std::vector<VAL>>&data,KEY n,VAL t,Args... args){
  a2m(data,n,t);
  a2m(data,n,args...);
}



int main(){
  /*
  std::map<const char*,std::vector<int>>ddd;
  a2m(ddd,"a",1,2,"asd","b",1,3,"v","d","s");
  std::map<const char*,std::vector<int>>::iterator ii;
  for(ii=ddd.begin();ii!=ddd.end();++ii){
    std::cerr<<ii->first<<std::endl;
    printv(ii->second);
  }
  return 0;
  */
  /*std::vector<float>dd;
  filterArgsToVector(dd,"asd",1.f,.23f,"asda",32.f,"asssd",12.2f,32u,5.5f);
  printv(dd);
  return 0;*/
  ShadowMapping*shadowMapping=new ShadowMapping();
  shadowMapping->setVariableAsChanged("va");
  shadowMapping->setVariableAsChanged("basevb");
  shadowMapping->update();
  for(unsigned i=0;i<shadowMapping->getNofVars();++i)
    std::cerr<<shadowMapping->getVar(i)<<std::endl;
  std::cerr<<shadowMapping->get()<<std::endl;
  std::chrono::duration<double, std::nano> duration;
  std::chrono::high_resolution_clock::time_point timea,timeb;
  timea=std::chrono::high_resolution_clock::now();
  for(unsigned i=0;i<1000000;++i){
    shadowMapping->setVariableAsChanged("va");
    shadowMapping->setVariableAsChanged("basevb");
    shadowMapping->update();
  }
  timeb=std::chrono::high_resolution_clock::now();
  duration = timeb-timea;
  std::cerr<<shadowMapping->get()<<std::endl;
  std::cerr<<duration.count()/1000000000.<<std::endl;
  return 0;
}




