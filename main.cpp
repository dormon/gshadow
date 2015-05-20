
#include<GL/gl.h>
#include<iostream>
#include<cstring>
#include<vector>
#include<map>
#include<set>


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

ENUM_CLASS(VertexSemantic,POSION,NORMAL,COORD);

template<typename TPtr>
void deleteSetNull(TPtr&b){
  if(b)delete b;b=NULL;
}

template<typename TPtr,typename...Args>
void deleteSetNull(TPtr&b,Args... args){
  if(b)delete b;b=NULL;
  deleteSetNull(args...);
}


template<typename KEY,typename VAL>
void insertVectorToMap(std::map<KEY,std::vector<VAL>>&data,KEY n,VAL t){
  if(!data.count(n))data.insert(std::pair<KEY,std::vector<VAL>>(n,std::vector<VAL>()));
    data[n].push_back(t);
}

template<typename KEY,typename VAL,typename... Args>
void insertVectorToMap(std::map<KEY,std::vector<VAL>>&data,KEY n,VAL t,Args... args);

template<typename KEY,typename VAL,typename... Args>
void insertVectorToMap(std::map<KEY,std::vector<VAL>>&data,KEY n,KEY n2,VAL t,Args... args){
    insertVectorToMap(data,n2,t,args...);
}

template<typename KEY,typename VAL,typename... Args>
void insertVectorToMap(std::map<KEY,std::vector<VAL>>&data,KEY n,VAL t,Args... args){
    insertVectorToMap(data,n,t);
      insertVectorToMap(data,n,args...);
}

template<typename KEY,typename VAL,typename... Args>
static std::map<KEY,std::vector<VAL>>createStaticMapOfVectors(Args... args){
  std::map<KEY,std::vector<VAL>>m;
  insertVectorToMap(m,args...);
  return m;
}

template<typename T>
void insertToVector(std::vector<T>&data,T t){
  data.push_back(t);
}

template<typename T,typename... Args>
void insertToVector(std::vector<T>&data,T t,Args... args){
  insertToVector(data,t);
  insertToVector(data,args...);
}

template<typename T,typename... Args>
static std::vector<T>createStaticVector(Args... args){
  std::vector<T>v;
  insertToVector(v,args...);
  return v;
}

class SimulationObject{
  protected:
    std::map<const char*,bool>_changed;
  public:
    SimulationObject();
    virtual void update()=0;
    virtual unsigned getNofVars()=0;
    virtual std::string getVar(unsigned var)=0;
    void setVariableAsChanged(const char*name);
};

void SimulationObject::setVariableAsChanged(const char*name){
  this->_changed[name]=true;
}

SimulationObject::SimulationObject(){}

class Shadow: public SimulationObject{
  protected:
    void basea(){std::cerr<<"basea\n";}
    void baseb(){std::cerr<<"baseb\n";}
    void basec(){std::cerr<<"basec\n";}
    static const std::vector<const char*>_vars;
    static const std::map<const char*,std::vector<void(Shadow::*)()>>_vars2UpdateRoutines;
  public:
    Shadow();
    virtual void update();
    virtual unsigned getNofVars();
    virtual std::string getVar(unsigned var);
};

const std::vector<const char*>Shadow::_vars = createStaticVector<const char*>("baseva","basevb");

const std::map<const char*,std::vector<void(Shadow::*)()>> Shadow::_vars2UpdateRoutines = createStaticMapOfVectors<const char*,void(Shadow::*)()>(
    "baseva",&Shadow::basea,&Shadow::baseb,
    "basevb",&Shadow::baseb,&Shadow::basec);

Shadow::Shadow(){
  std::cerr<<"constructor Shadow\n";
  for(unsigned i=0;i<this->_vars.size();++i){
    std::cerr<<this->_vars[i]<<std::endl;
    this->_changed.insert(std::pair<const char*,bool>(this->_vars[i],false));
  }
  std::cerr<<"this->_changed: "<<this->_changed.size()<<std::endl;
  std::cerr<<"constructor Shadow end\n";
}

unsigned Shadow::getNofVars(){
  return this->_vars.size();
}

std::string Shadow::getVar(unsigned var){
  return this->_vars[var];
}

//TODO vector of update routines

void Shadow::update(){
  std::cerr<<"Shadow::update()\n";
  std::cerr<<"this->_changed: "<<this->_changed.size()<<std::endl;
  std::vector<void(Shadow::*)()>methods2Call;
  for(unsigned i=0;i<this->_changed.size();++i){
    std::cerr<<"i: "<<i<<" "<<this->_vars[i]<<std::endl;
    if(!this->_changed[this->_vars[i]])continue;
    if(!this->_vars2UpdateRoutines.count(this->_vars[i]))continue;
    this->_vars2UpdateRoutines.find(this->_vars[i])->second.size();
    for(unsigned j=0;j<this->_vars2UpdateRoutines.find(this->_vars[i])->second.size();++j){
      void (Shadow::*ptr)()=this->_vars2UpdateRoutines.find(this->_vars[i])->second[j];
      bool alreadInserted=false;
      for(unsigned i=0;i<methods2Call.size();++i)
        if(methods2Call[i]==ptr){alreadInserted=true;break;}
      if(!alreadInserted)
        methods2Call.push_back(ptr);
    }
  }
  for(unsigned i=0;i<methods2Call.size();++i)
    (this->*methods2Call[i])();
}

class ShadowMapping: public Shadow{
  private:
    void ma(){std::cerr<<"ma\n";}
    void mb(){std::cerr<<"mb\n";}
    void mc(){std::cerr<<"mc\n";}
    void md(){std::cerr<<"md\n";}
    void me(){std::cerr<<"me\n";}
    void mf(){std::cerr<<"mf\n";}
    static const std::vector<const char*>_vars;
    static const std::map<const char*,std::vector<void(ShadowMapping::*)()>>_vars2UpdateRoutines;
  public:
    ShadowMapping();
    virtual void update();
    virtual unsigned getNofVars();
    virtual std::string getVar(unsigned var);
};

const std::vector<const char*>ShadowMapping::_vars = createStaticVector<const char*>("va","vb","vc");

const std::map<const char*,std::vector<void(ShadowMapping::*)()>> ShadowMapping::_vars2UpdateRoutines = createStaticMapOfVectors<const char*,void(ShadowMapping::*)()>(
    "va",&ShadowMapping::ma,&ShadowMapping::mb,
    "vb",&ShadowMapping::mc,&ShadowMapping::md,
    "vc",&ShadowMapping::me,&ShadowMapping::mf);

ShadowMapping::ShadowMapping():Shadow(){
  std::cerr<<"ShadowMapping::_vars.size(): "<<this->ShadowMapping::_vars.size()<<std::endl;
  std::cerr<<"Shadow::_vars.size(): "<<this->Shadow::_vars.size()<<std::endl;
  std::cerr<<"this->ShadowMapping::_changed.size(): "<<this->ShadowMapping::_changed.size()<<std::endl;
  std::cerr<<"this->Shadow::_changed.size(): "<<this->Shadow::_changed.size()<<std::endl;

  for(unsigned i=0;i<this->ShadowMapping::_vars.size();++i)
    this->ShadowMapping::_changed.insert(std::pair<const char*,bool>(this->ShadowMapping::_vars[i],false));
  std::cerr<<"this->ShadowMapping::_changed.size(): "<<this->ShadowMapping::_changed.size()<<std::endl;
  std::cerr<<"this->Shadow::_changed.size(): "<<this->Shadow::_changed.size()<<std::endl;

}

void ShadowMapping::update(){
  std::cerr<<"ShadowMapping::update()\n";
  std::vector<void(ShadowMapping::*)()>methods2Call;
  std::cerr<<"this->ShadowMapping::_changed.size(): "<<this->ShadowMapping::_changed.size()<<std::endl;
  std::cerr<<"this->Shadow::_changed.size(): "<<this->Shadow::_changed.size()<<std::endl;
  for(unsigned i=0;i<this->_changed.size();++i){
    if(!this->_changed[this->_vars[i]])continue;
    if(!this->_vars2UpdateRoutines.count(this->_vars[i]))continue;
    this->_vars2UpdateRoutines.find(this->_vars[i])->second.size();
    for(unsigned j=0;j<this->_vars2UpdateRoutines.find(this->_vars[i])->second.size();++j){
      void (ShadowMapping::*ptr)()=this->_vars2UpdateRoutines.find(this->_vars[i])->second[j];
      bool alreadInserted=false;
      for(unsigned i=0;i<methods2Call.size();++i)
        if(methods2Call[i]==ptr){alreadInserted=true;break;}
      if(!alreadInserted)
        methods2Call.push_back(ptr);
    }
  }
  for(unsigned i=0;i<methods2Call.size();++i)
    (this->*methods2Call[i])();
  this->Shadow::update();
}

unsigned ShadowMapping::getNofVars(){
  return this->_vars.size()+((Shadow*)this)->getNofVars();
}

std::string ShadowMapping::getVar(unsigned var){
  if(var>=this->_vars.size())return((Shadow*)this)->getVar(var-this->_vars.size());
  return _vars[var];
}

void ma(){std::cerr<<"ma\n";}
void mb(){std::cerr<<"mb\n";}
void mc(){std::cerr<<"mc\n";}
void md(){std::cerr<<"md\n";}
void me(){std::cerr<<"me\n";}
void mf(){std::cerr<<"mf\n";}

typedef void(*METHOD)();

#define DEF_UPDATE(...)\
  void update(){\
    std::map<const char*,std::vector<METHOD>>var2Method;\
    insertVectorToMap(var2Method,__VA_ARGS__);\
    std::set<METHOD>methods2Call;\
    std::vector<const char*>updatedVars;\
    updatedVars.push_back("va");\
    updatedVars.push_back("vd");\
    for(unsigned i=0;i<updatedVars.size();++i){\
      if(!var2Method.count(updatedVars[i]))continue;\
      for(unsigned j=0;j<var2Method[updatedVars[i]].size();++j)\
      methods2Call.insert(var2Method[updatedVars[i]][j]);\
    }\
    std::set<METHOD>::iterator ii;\
    for(ii=methods2Call.begin();ii!=methods2Call.end();++ii)\
    (*ii)();\
  }

DEF_UPDATE("va",ma,mb,"vb",mc,md,"vc",me,mf,"vd",ma,md,mf,mc)


int main(){
  ShadowMapping*shadowMapping=new ShadowMapping();
  shadowMapping->update();
  return 0;
  std::vector<float>data;
  insertToVector(data,3.f,1.f,.0f);
  static const std::vector<float>data2 = createStaticVector<float>(3.f,.2f,1.f,6.f);
  std::cerr<<data[0]<<" "<<data[1]<<" "<<data[2]<<std::endl;
  std::cerr<<data2[0]<<" "<<data2[1]<<" "<<data2[2]<<std::endl;
  return 0;
  update();
  VertexSemantic e=VertexSemantic::POSION;
  std::cerr<<e<<std::endl;
  return 0;
}




