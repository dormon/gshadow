#include"SimulationData.h"

#include<sstream>
#include<cstdlib>
#include<geUtil/ArgumentObject.h>
#include<algorithm>

using namespace simulation;

std::string Namespace::toUpper(std::string str){
  std::transform(str.begin(), str.end(),str.begin(), ::toupper);
  return str;
}


std::string Namespace::define(std::string prefix){
  //std::cerr<<"ptr: "<<this<<" : "<<this->_name<<std::endl;
  //std::cerr<<"definujeme: "<<this->_name<<std::endl;
  std::stringstream ss;
  for(unsigned v=0;v<this->_subVariables.size();++v){
    switch(this->_subVariables[v]->type){
      case BOOL:
        if(((Bool*)this->_subVariables[v])->data)
          ss<<ge::gl::ShaderObject::define(prefix+this->toUpper(this->_subVariableNames[v]));
        break;
      case INT:
        ss<<ge::gl::ShaderObject::define(prefix+this->toUpper(this->_subVariableNames[v]),
            ((Int*)this->_subVariables[v])->data);
        break;
      case IVEC2:
        ss<<ge::gl::ShaderObject::define(prefix+this->toUpper(this->_subVariableNames[v]),
            ((IVec2*)this->_subVariables[v])->data.x,
            ((IVec2*)this->_subVariables[v])->data.y);
        break;
      case IVEC3:
        ss<<ge::gl::ShaderObject::define(prefix+this->toUpper(this->_subVariableNames[v]),
            ((IVec3*)this->_subVariables[v])->data.x,
            ((IVec3*)this->_subVariables[v])->data.y,
            ((IVec3*)this->_subVariables[v])->data.z);
        break;
      case IVEC4:
        ss<<ge::gl::ShaderObject::define(prefix+this->toUpper(this->_subVariableNames[v]),
            ((IVec4*)this->_subVariables[v])->data.x,
            ((IVec4*)this->_subVariables[v])->data.y,
            ((IVec4*)this->_subVariables[v])->data.z,
            ((IVec4*)this->_subVariables[v])->data.w);
        break;
      case UINT:
        ss<<ge::gl::ShaderObject::define(prefix+this->toUpper(this->_subVariableNames[v]),
            ((Uint*)this->_subVariables[v])->data);
        break;
      case UVEC2:
        ss<<ge::gl::ShaderObject::define(prefix+this->toUpper(this->_subVariableNames[v]),
            ((UVec2*)this->_subVariables[v])->data.x,
            ((UVec2*)this->_subVariables[v])->data.y);
        break;
      case UVEC3:
        ss<<ge::gl::ShaderObject::define(prefix+this->toUpper(this->_subVariableNames[v]),
            ((UVec3*)this->_subVariables[v])->data.x,
            ((UVec3*)this->_subVariables[v])->data.y,
            ((UVec3*)this->_subVariables[v])->data.z);
        break;
      case UVEC4:
        ss<<ge::gl::ShaderObject::define(prefix+this->toUpper(this->_subVariableNames[v]),
            ((UVec4*)this->_subVariables[v])->data.x,
            ((UVec4*)this->_subVariables[v])->data.y,
            ((UVec4*)this->_subVariables[v])->data.z,
            ((UVec4*)this->_subVariables[v])->data.w);
        break;
      case FLOAT:
        ss<<ge::gl::ShaderObject::define(prefix+this->toUpper(this->_subVariableNames[v]),
            ((Float*)this->_subVariables[v])->data);
        break;
      case VEC2:
        ss<<ge::gl::ShaderObject::define(prefix+this->toUpper(this->_subVariableNames[v]),
            ((Vec2*)this->_subVariables[v])->data.x,
            ((Vec2*)this->_subVariables[v])->data.y);
        break;
      case VEC3:
        ss<<ge::gl::ShaderObject::define(prefix+this->toUpper(this->_subVariableNames[v]),
            ((Vec3*)this->_subVariables[v])->data.x,
            ((Vec3*)this->_subVariables[v])->data.y,
            ((Vec3*)this->_subVariables[v])->data.z);
        break;
      case VEC4:
        ss<<ge::gl::ShaderObject::define(prefix+this->toUpper(this->_subVariableNames[v]),
            ((Vec4*)this->_subVariables[v])->data.x,
            ((Vec4*)this->_subVariables[v])->data.y,
            ((Vec4*)this->_subVariables[v])->data.z,
            ((Vec4*)this->_subVariables[v])->data.w);
        break;
      case STRING:
        ss<<ge::gl::ShaderObject::define(prefix+this->toUpper(this->_subVariableNames[v]),
            ((String*)this->_subVariables[v])->data);
        break;
      default:
        break;
    }
  }
  //std::cerr<<"po def: "<<this->_name<<std::endl;
  for(unsigned n=0;n<this->_subNamespaces.size();++n){
    ss<<this->_subNamespaces[n]->define(this->toUpper(this->_subNamespaceNames[n])+"_");
  }
  return ss.str();
}

Namespace::Namespace(std::string name){
  this->_name=name;
}
void Namespace::insertNamespace(std::string name,Namespace*nmspace){
  this->_subNamespaceNames.push_back(name);
  this->_subNamespaces.push_back(nmspace);
  this->_name2Namespace.insert(std::pair<std::string,Namespace*>(name,nmspace));
}

void Namespace::insertVariable (std::string name,Data*variable){
  this->_subVariableNames.push_back(name);
  this->_subVariables.push_back(variable);
  this->_name2Variable.insert(std::pair<std::string,Data*>(name,variable));
}

void Namespace::insert(std::string name,Data*variable){
  int pos=name.find(".");
  if(pos==0)
    throw std::string("incorect variable name - empty namespace name");
  else if(pos<0)
    this->insertVariable(name,variable);
  else{
    std::string rest=name.substr(pos+1);
    std::string namespaceName=name.substr(0,pos);
    Namespace*nextNamespace=this->getNamespace(namespaceName);
    if(nextNamespace==NULL){
      nextNamespace=new Namespace(namespaceName);
      this->insertNamespace(namespaceName,nextNamespace);
    }
    nextNamespace->insert(rest,variable);
  }
}

std::string Namespace::toStr(unsigned indentation){
  std::stringstream ss;
  std::string ind="";
  for(unsigned i=0;i<indentation;++i)ind+=" ";
  ss<<ind<<this->_name<<"{"<<std::endl;
  for(unsigned v=0;v<this->_subVariableNames.size();++v)
    ss<<ind<<"  "<<this->_subVariableNames[v]<<":"<<this->_subVariables[v]->toStr()<<" "<<this->_subVariables[v]->type<<std::endl;
  for(unsigned n=0;n<this->_subNamespaceNames.size();++n)
    ss<<this->_subNamespaces[n]->toStr(indentation+2);
  ss<<ind<<"}"<<std::endl;
  return ss.str();
}
Namespace*Namespace::getNamespace(std::string name){
  if(this->_name2Namespace.count(name))
    return this->_name2Namespace[name];
  return NULL;
}
Data*     Namespace::getVariable (std::string name){
  if(this->_name2Variable.count(name))
    return this->_name2Variable[name];
  return NULL;
}

SimulationData::SimulationData(){
  this->_rootNamespace=new Namespace("*");
}

SimulationObject::~SimulationObject(){
}

unsigned SimulationObject::getNofDependentVariables(){
  return 0;
}

std::string SimulationObject::getDependentVariable(unsigned){
  return"";
}

void SimulationObject::update(){
}

void SimulationData::insertVariable(std::string name,Data*data){
  this->_variable.insert(std::pair<std::string,Data*>(name,data));
  this->_rootNamespace->insert(name,data);
}


bool SimulationData::_isNumber(std::string data){
  if(data=="")return false;
  return 
    std::isdigit(data[0])||
    data[0]=='+'||
    data[0]=='-'||
    data.find(".")  !=std::string::npos||
    data.find("inf")!=std::string::npos||
    data.find("nan")!=std::string::npos;
}

bool SimulationData::_isFloat(std::string data){
  return this->_isNumber(data)&&(
      data.find(".")  !=std::string::npos||
      data.find("inf")!=std::string::npos||
      data.find("nan")!=std::string::npos);
}

void SimulationData::insertVariable(std::string name,std::string data){
  if(this->_isNumber(data)){
    std::vector<std::string>numbers;
    std::string actDataString=data;
    std::size_t pos=0;
    bool isUint = true;
    bool isInt  = true;
    while((pos=actDataString.find(" "))!=std::string::npos){
      //std::cerr<<"-"<<actDataString<<"-"<<std::endl;
      std::string actNumber=actDataString.substr(0,pos);
      if(actNumber[0]=='-')isUint=false;
      if(this->_isFloat(actNumber))isInt=false;
      numbers.push_back(actNumber);
      actDataString=actDataString.substr(pos+1);
    }
    if(actDataString[0]=='-')isUint=false;
    if(this->_isFloat(actDataString))isInt=false;
    numbers.push_back(actDataString);
    //std::cerr<<data<<": "<<numbers.size()<<" isUint: "<<isUint<<" isInt: "<<isInt<<std::endl;
    //for(unsigned i=0;i<numbers.size();++i)
    //  std::cerr<<numbers[i]<<"#";
    //std::cerr<<std::endl;
    switch(numbers.size()){
      case 1:
        if(isInt&&isUint)this->insertVariable(name,new Uint (data));
        else if(isInt)   this->insertVariable(name,new Int  (data));
        else             this->insertVariable(name,new Float(data));
        break;
      case 2:
        if(isInt&&isUint)this->insertVariable(name,new UVec2(data));
        else if(isInt)   this->insertVariable(name,new IVec2(data));
        else             this->insertVariable(name,new Vec2 (data));
        break;
      case 3:
        if(isInt&&isUint)this->insertVariable(name,new UVec3(data));
        else if(isInt)   this->insertVariable(name,new IVec3(data));
        else             this->insertVariable(name,new Vec3 (data));
        break;
      case 4:
        if(isInt&&isUint)this->insertVariable(name,new UVec4(data));
        else if(isInt)   this->insertVariable(name,new IVec4(data));
        else             this->insertVariable(name,new Vec4 (data));
        break;
    }
  }else if(data=="true"||data=="false"){
    this->insertVariable(name,new Bool(data));
  }else{
    this->insertVariable(name,new String(data));
  }
}

std::string SimulationData::toStr(){
  return this->_rootNamespace->toStr(0);
}

std::string SimulationData::getKeys(){
  std::stringstream ss;
  typedef std::map<std::string, simulation::Data*>::iterator it_type;
  for(it_type iterator = this->_variable.begin(); iterator != this->_variable.end(); iterator++) {
    ss<<iterator->first<<std::endl;
  }
  return ss.str();
}

simulation::Data*SimulationData::get(std::string variableName,simulation::Data*def){
  if(!this->_variable.count(variableName))return def;
  return this->_variable[variableName];
}

bool SimulationData::getBool(std::string variableName,bool def){
  if(!this->_variable.count(variableName))return def;
  Data*data=this->_variable[variableName];
  if(data->type==BOOL )return ((Bool* )data)->data;
  if(data->type==INT  )return ((Int*  )data)->data;
  if(data->type==UINT )return ((Uint* )data)->data;
  if(data->type==FLOAT)return ((Float*)data)->data;
  return def;
}
int SimulationData::getInt(std::string variableName,int def){
  if(!this->_variable.count(variableName))return def;
  Data*data=this->_variable[variableName];
  if(data->type==BOOL )return ((Bool* )data)->data;
  if(data->type==INT  )return ((Int*  )data)->data;
  if(data->type==UINT )return ((Uint* )data)->data;
  if(data->type==FLOAT)return ((Float*)data)->data;
  return def;
}
glm::ivec2 SimulationData::getIVec2(std::string variableName,glm::ivec2 def){
  if(!this->_variable.count(variableName))return def;
  Data*data=this->_variable[variableName];
  if(data->type==IVEC2)return glm::ivec2(((IVec2*)data)->data);
  if(data->type==UVEC2)return glm::ivec2(((UVec2*)data)->data);
  if(data->type==VEC2 )return glm::ivec2(((Vec2* )data)->data);
  return def;
}
glm::ivec3 SimulationData::getIVec3(std::string variableName,glm::ivec3 def){
  if(!this->_variable.count(variableName))return def;
  Data*data=this->_variable[variableName];
  if(data->type==IVEC3)return glm::ivec3(((IVec3*)data)->data);
  if(data->type==UVEC3)return glm::ivec3(((UVec3*)data)->data);
  if(data->type==VEC3 )return glm::ivec3(((Vec3* )data)->data);
  return def;
}
glm::ivec4 SimulationData::getIVec4(std::string variableName,glm::ivec4 def){
  if(!this->_variable.count(variableName))return def;
  Data*data=this->_variable[variableName];
  if(data->type==IVEC4)return glm::ivec4(((IVec4*)data)->data);
  if(data->type==UVEC4)return glm::ivec4(((UVec4*)data)->data);
  if(data->type==VEC4 )return glm::ivec4(((Vec4* )data)->data);
  return def;
}

unsigned SimulationData::getUint(std::string variableName,unsigned def){
  if(!this->_variable.count(variableName))return def;
  Data*data=this->_variable[variableName];
  if(data->type==BOOL )return ((Bool* )data)->data;
  if(data->type==INT  )return ((Int*  )data)->data;
  if(data->type==UINT )return ((Uint* )data)->data;
  if(data->type==FLOAT)return ((Float*)data)->data;
  return def;
}
glm::uvec2 SimulationData::getUVec2(std::string variableName,glm::uvec2 def){
  if(!this->_variable.count(variableName))return def;
  Data*data=this->_variable[variableName];
  if(data->type==IVEC2)return glm::uvec2(((IVec2*)data)->data);
  if(data->type==UVEC2)return glm::uvec2(((UVec2*)data)->data);
  if(data->type==VEC2 )return glm::uvec2(((Vec2* )data)->data);
  return def;
}
glm::uvec3 SimulationData::getUVec3(std::string variableName,glm::uvec3 def){
  if(!this->_variable.count(variableName))return def;
  Data*data=this->_variable[variableName];
  if(data->type==IVEC3)return glm::uvec3(((IVec3*)data)->data);
  if(data->type==UVEC3)return glm::uvec3(((UVec3*)data)->data);
  if(data->type==VEC3 )return glm::uvec3(((Vec3* )data)->data);
  return def;
}
glm::uvec4 SimulationData::getUVec4(std::string variableName,glm::uvec4 def){
  if(!this->_variable.count(variableName))return def;
  Data*data=this->_variable[variableName];
  if(data->type==IVEC4)return glm::uvec4(((IVec4*)data)->data);
  if(data->type==UVEC4)return glm::uvec4(((UVec4*)data)->data);
  if(data->type==VEC4 )return glm::uvec4(((Vec4* )data)->data);
  return def;
}

float SimulationData::getFloat(std::string variableName,float def){
  if(!this->_variable.count(variableName))return def;
  Data*data=this->_variable[variableName];
  if(data->type==BOOL )return((Bool* )data)->data;
  if(data->type==INT  )return((Int*  )data)->data;
  if(data->type==UINT )return((Uint* )data)->data;
  if(data->type==FLOAT)return((Float*)data)->data;
  return def;
}
glm::vec2 SimulationData::getVec2(std::string variableName,glm::vec2 def){
  if(!this->_variable.count(variableName))return def;
  Data*data=this->_variable[variableName];
  if(data->type==IVEC2)return glm::vec2(((IVec2*)data)->data);
  if(data->type==UVEC2)return glm::vec2(((UVec2*)data)->data);
  if(data->type==VEC2 )return glm::vec2(((Vec2* )data)->data);
  return def;
}
glm::vec3 SimulationData::getVec3(std::string variableName,glm::vec3 def){
  if(!this->_variable.count(variableName))return def;
  Data*data=this->_variable[variableName];
  if(data->type==IVEC3)return glm::vec3(((IVec3*)data)->data);
  if(data->type==UVEC3)return glm::vec3(((UVec3*)data)->data);
  if(data->type==VEC3 )return glm::vec3(((Vec3* )data)->data);
  return def;
}
glm::vec4 SimulationData::getVec4(std::string variableName,glm::vec4 def){
  if(!this->_variable.count(variableName))return def;
  Data*data=this->_variable[variableName];
  if(data->type==IVEC4)return glm::vec4(((IVec4*)data)->data);
  if(data->type==UVEC4)return glm::vec4(((UVec4*)data)->data);
  if(data->type==VEC4 )return glm::vec4(((Vec4* )data)->data);
  return def;
}

std::string SimulationData::getString(std::string variableName,std::string def){
  if(!this->_variable.count(variableName))return def;
  Data*data=this->_variable[variableName];
  if(data->type==STRING)return ((String*)data)->data;
  return def;
}

glm::mat4 SimulationData::getMat4(std::string variableName,glm::mat4 def){
  if(!this->_variable.count(variableName))return def;
  Data*data=this->_variable[variableName];
  if(data->type==MAT4)return ((Mat4*)data)->data;
  return def;
}

void*SimulationData::getObject(std::string variableName,void*def){
  if(!this->_variable.count(variableName))return def;
  Data*data=this->_variable[variableName];
  if(data->type==OBJECT)return ((Object*)data)->data;
  return def;
}
Light*SimulationData::getLight (std::string variableName,Light*def){
  if(!this->_variable.count(variableName))return def;
  Data*data=this->_variable[variableName];
  if(data->type==LIGHT)return ((Light*)data);
  return def;
}
Gauge*SimulationData::getGauge(std::string variableName,Gauge*def){
  if(!this->_variable.count(variableName))return def;
  Data*data=this->_variable[variableName];
  if(data->type==GAUGE)return ((Gauge*)data);
  return def;
}
GpuGauge*SimulationData::getGpuGauge(std::string variableName,GpuGauge*def){
  if(!this->_variable.count(variableName))return def;
  Data*data=this->_variable[variableName];
  if(data->type==GPUGAUGE)return ((GpuGauge*)data);
  return def;
}

std::string SimulationData::define(std::string prefix){
  if(prefix=="")return this->_rootNamespace->define("");
  std::vector<std::string>path;
  std::size_t pos;
  while((pos=prefix.find("."))!=std::string::npos){
    path.push_back(prefix.substr(0,pos));
    prefix=prefix.substr(pos+1);
  }
  if(prefix!="")path.push_back(prefix);
  simulation::Namespace*currentNamespace=this->_rootNamespace;
  for(unsigned pathId=0;pathId<path.size();++pathId){
    if(currentNamespace==NULL)return"";
    currentNamespace=currentNamespace->getNamespace(path[pathId]);
  }
  return currentNamespace->define("");
}




SimulationObject::SimulationObject(SimulationData*data){
  this->_simulationData=data;
}

void SimulationObject::initChanged(std::string name){
  this->_changed.insert(std::pair<std::string,bool>(name,false));
}

void SimulationObject::variableChanged(std::string name){
  this->_changed[name]=true;
}

void SimulationData::registerUser(SimulationObject*user){
  this->_users.push_back(user);
  this->_sendUpdate.push_back(false);
  for(unsigned v=0;v<user->getNofDependentVariables();++v){
    std::string name=user->getDependentVariable(v);
    VariableUsers*users=NULL;
    if(!this->_variableUser.count(name)){
      users=new VariableUsers;
      this->_variableUser.insert(std::pair<std::string,VariableUsers*>(name,users));
    }else
      users=this->_variableUser[name];
    users->user.push_back(this->_users.size()-1);
    user->initChanged(name);
  }
}

void SimulationData::unregisterUser(SimulationObject*user){
  bool found=false;
  unsigned userId=0;
  for(unsigned i=0;i<this->_users.size();++i)
    if(this->_users[i]==user){
      found=true;
      userId=i;
      break;
    }
  if(!found)return;

  unsigned victimId=this->_users.size()-1;//for swap with userId

  //only victim
  for(unsigned u=0;u<this->_users[victimId]->getNofDependentVariables();++u){
    std::string name=this->_users[victimId]->getDependentVariable(u);
    bool skip=false;
    for(unsigned i=0;i<this->_users[userId]->getNofDependentVariables();++i)
      if(name==this->_users[userId]->getDependentVariable(i)){
        skip=true;
        break;
      }
    if(skip)continue;
    VariableUsers*users=this->_variableUser[name];
    for(unsigned i=0;i<users->user.size();++i)
      if(users->user[i]==victimId){
        users->user[i]=userId;
        break;
      }
  }
  //both and user only
  for(unsigned u=0;u<this->_users[userId]->getNofDependentVariables();++u){
    std::string name=this->_users[userId]->getDependentVariable(u);
    bool both=false;
    for(unsigned i=0;i<this->_users[victimId]->getNofDependentVariables();++i)
      if(name==this->_users[victimId]->getDependentVariable(i)){
        both=true;
        break;
      }
    if(both){
      VariableUsers*users=this->_variableUser[name];
      unsigned victimPos=0;
      unsigned userPos=0;
      for(unsigned i=0;i<users->user.size();++i){
        if(users->user[i]==userId)
          userPos=i;
        if(users->user[i]==victimId)
          victimPos=i;
      }
      users->user[victimPos]=userId;
      users->user[userPos]=users->user[users->user.size()-1];
      users->user.pop_back();
    }else{
      VariableUsers*users=this->_variableUser[name];
      for(unsigned i=0;i<users->user.size();++i)
        if(users->user[i]==userId){
          users->user[i]=users->user[users->user.size()-1];
          users->user.pop_back();
          break;
        }
    }
  }

  this->_users[userId]=this->_users[this->_users.size()-1];
  this->_users.pop_back();
  this->_sendUpdate[userId]=this->_sendUpdate[this->_sendUpdate.size()-1];
  this->_sendUpdate.pop_back();
}

void SimulationData::setAsChanged(std::string name){
  if(!this->_variableUser.count(name))return;
  VariableUsers*users=this->_variableUser[name];
  for(unsigned u=0;u<users->user.size();++u){
    this->_sendUpdate[u]=true;
    this->_users[u]->variableChanged(name);
  }
}

void SimulationData::sendUpdate(){
  for(unsigned u=0;u<this->_users.size();++u){//loop over users
    if(this->_sendUpdate[u]){//do we have to sent update?
      this->_sendUpdate[u]=false;//next time we will skip this user
      this->_users[u]->update();//sent update
    }
  }
}



