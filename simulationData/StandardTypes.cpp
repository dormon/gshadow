#include"StandardTypes.h"

#include<sstream>
#include<cstdlib>
#include<geUtil/ArgumentObject.h>
#include<algorithm>

using namespace simulation;
Bool::Bool(std::string input){
  this->type=BOOL;
  if(input=="true")this->data=true;
  else this->data=false;
}
Bool::Bool(bool input){
  this->type=BOOL;
  this->data=input;
}
std::string Bool::toStr(){
  if(this->data)return"true";
  return"false";
}
bool Bool::operator==(Data*b){return this->type==b->type && this->data==((Bool*)b)->data;}


Int::Int(std::string input){
  this->type=INT;
  this->data=std::atoi(input.c_str());
}
Int::Int(int input){
  this->type=INT;
  this->data=input;
}
std::string Int::toStr(){
  std::stringstream ss;
  ss<<this->data;
  return ss.str();
}
bool Int::operator==(Data*b){return this->type==b->type && this->data==((Int*)b)->data;}

IVec2::IVec2(std::string input){
  this->type=IVEC2;
  std::string val,rest=input;
  for(unsigned i=0;i<1;++i){
    this->data[i]=std::atoi(rest.substr(0,rest.find(" ")).c_str());
    rest=rest.substr(rest.find(" ")+1);
  }
  this->data[1]=std::atoi(rest.c_str());
}
IVec2::IVec2(glm::ivec2 input){
  this->type=IVEC2;
  this->data=input;
}
std::string IVec2::toStr(){
  std::stringstream ss;
  ss<<this->data.x<<" "<<this->data.y;
  return ss.str();
}
bool IVec2::operator==(Data*b){return this->type==b->type && this->data==((IVec2*)b)->data;}

IVec3::IVec3(std::string input){
  this->type=IVEC3;
  std::string val,rest=input;
  for(unsigned i=0;i<2;++i){
    this->data[i]=std::atoi(rest.substr(0,rest.find(" ")).c_str());
    rest=rest.substr(rest.find(" ")+1);
  }
  this->data[2]=std::atoi(rest.c_str());
}
IVec3::IVec3(glm::ivec3 input){
  this->type=IVEC3;
  this->data=input;
}
std::string IVec3::toStr(){
  std::stringstream ss;
  ss<<this->data.x<<" "<<this->data.y<<" "<<this->data.z;
  return ss.str();
}
bool IVec3::operator==(Data*b){return this->type==b->type && this->data==((IVec3*)b)->data;}


IVec4::IVec4(std::string input){
  this->type=IVEC4;
  std::string val,rest=input;
  for(unsigned i=0;i<3;++i){
    this->data[i]=std::atoi(rest.substr(0,rest.find(" ")).c_str());
    rest=rest.substr(rest.find(" ")+1);
  }
  this->data[3]=std::atoi(rest.c_str());
}
IVec4::IVec4(glm::ivec4 input){
  this->type=IVEC4;
  this->data=input;
}
std::string IVec4::toStr(){
  std::stringstream ss;
  ss<<this->data.x<<" "<<this->data.y<<" "<<this->data.z<<" "<<this->data.w;
  return ss.str();
}
bool IVec4::operator==(Data*b){return this->type==b->type && this->data==((IVec4*)b)->data;}

Uint::Uint(std::string input){
  this->type=UINT;
  this->data=std::atoi(input.c_str());
}
Uint::Uint(unsigned input){
  this->type=UINT;
  this->data=input;
}
std::string Uint::toStr(){
  std::stringstream ss;
  ss<<this->data;
  return ss.str();
}
bool Uint::operator==(Data*b){return this->type==b->type && this->data==((Uint*)b)->data;}

UVec2::UVec2(std::string input){
  this->type=UVEC2;
  std::string val,rest=input;
  for(unsigned i=0;i<1;++i){
    this->data[i]=std::atoi(rest.substr(0,rest.find(" ")).c_str());
    rest=rest.substr(rest.find(" ")+1);
  }
  this->data[1]=std::atoi(rest.c_str());
}
UVec2::UVec2(glm::uvec2 input){
  this->type=UVEC2;
  this->data=input;
}
std::string UVec2::toStr(){
  std::stringstream ss;
  ss<<this->data.x<<" "<<this->data.y;
  return ss.str();
}
bool UVec2::operator==(Data*b){return this->type==b->type && this->data==((UVec2*)b)->data;}

UVec3::UVec3(std::string input){
  this->type=UVEC3;
  std::string val,rest=input;
  for(unsigned i=0;i<2;++i){
    this->data[i]=std::atoi(rest.substr(0,rest.find(" ")).c_str());
    rest=rest.substr(rest.find(" ")+1);
  }
  this->data[2]=std::atoi(rest.c_str());
}
UVec3::UVec3(glm::uvec3 input){
  this->type=UVEC3;
  this->data=input;
}
std::string UVec3::toStr(){
  std::stringstream ss;
  ss<<this->data.x<<" "<<this->data.y<<" "<<this->data.z;
  return ss.str();
}
bool UVec3::operator==(Data*b){return this->type==b->type && this->data==((UVec3*)b)->data;}

UVec4::UVec4(std::string input){
  this->type=UVEC4;
  std::string val,rest=input;
  for(unsigned i=0;i<3;++i){
    this->data[i]=std::atoi(rest.substr(0,rest.find(" ")).c_str());
    rest=rest.substr(rest.find(" ")+1);
  }
  this->data[3]=std::atoi(rest.c_str());
}
UVec4::UVec4(glm::uvec4 input){
  this->type=UVEC4;
  this->data=input;
}
std::string UVec4::toStr(){
  std::stringstream ss;
  ss<<this->data.x<<" "<<this->data.y<<" "<<this->data.z<<" "<<this->data.w;
  return ss.str();
}
bool UVec4::operator==(Data*b){return this->type==b->type && this->data==((UVec4*)b)->data;}

Float::Float(std::string input){
  this->type=FLOAT;
  this->data=ge::util::atof(input.c_str());
}
Float::Float(float input){
  this->type=FLOAT;
  this->data=input;
}
std::string Float::toStr(){
  std::stringstream ss;
  ss<<this->data;
  return ss.str();
}
bool Float::operator==(Data*b){return this->type==b->type && this->data==((Float*)b)->data;}

Vec2::Vec2(std::string input){
  this->type=VEC2;
  std::string val,rest=input;
  for(unsigned i=0;i<1;++i){
    this->data[i]=ge::util::atof(rest.substr(0,rest.find(" ")).c_str());
    rest=rest.substr(rest.find(" ")+1);
  }
  this->data[1]=ge::util::atof(rest.c_str());
}
Vec2::Vec2(glm::vec2 input){
  this->type=VEC2;
  this->data=input;
}
std::string Vec2::toStr(){
  std::stringstream ss;
  ss<<this->data.x<<" "<<this->data.y;
  return ss.str();
}
bool Vec2::operator==(Data*b){return this->type==b->type && this->data==((Vec2*)b)->data;}

Vec3::Vec3(std::string input){
  this->type=VEC3;
  std::string val,rest=input;
  for(unsigned i=0;i<2;++i){
    this->data[i]=ge::util::atof(rest.substr(0,rest.find(" ")).c_str());
    rest=rest.substr(rest.find(" ")+1);
  }
  this->data[2]=ge::util::atof(rest.c_str());
}
Vec3::Vec3(glm::vec3 input){
  this->type=VEC3;
  this->data=input;
}
std::string Vec3::toStr(){
  std::stringstream ss;
  ss<<this->data.x<<" "<<this->data.y<<" "<<this->data.z;
  return ss.str();
}
bool Vec3::operator==(Data*b){return this->type==b->type && this->data==((Vec3*)b)->data;}

Vec4::Vec4(std::string input){
  this->type=VEC4;
  std::string val,rest=input;
  for(unsigned i=0;i<3;++i){
    this->data[i]=ge::util::atof(rest.substr(0,rest.find(" ")).c_str());
    rest=rest.substr(rest.find(" ")+1);
  }
  this->data[3]=ge::util::atof(rest.c_str());
}
Vec4::Vec4(glm::vec4 input){
  this->type=VEC4;
  this->data=input;
}
std::string Vec4::toStr(){
  std::stringstream ss;
  ss<<this->data.x<<" "<<this->data.y<<" "<<this->data.z<<" "<<this->data.w;
  return ss.str();
}
bool Vec4::operator==(Data*b){return this->type==b->type && this->data==((Vec4*)b)->data;}


String::String(std::string input){
  this->type=STRING;
  this->data=input;
}
std::string String::toStr(){
  return this->data;
}
bool String::operator==(Data*b){return this->type==b->type && this->data==((String*)b)->data;}

Mat4::Mat4(glm::mat4 input){
  this->type=MAT4;
  this->data=input;
}
Mat4::Mat4(){
  this->type=MAT4;
  this->data=glm::mat4(1.f);
}
std::string Mat4::toStr(){
  std::stringstream ss;
  for(int y=0;y<4;++y)
    for(int x=0;x<4;++x){
      ss<<this->data[y][x];
      if(y!=3&&y!=3)ss<<" ";
    }
  return ss.str();
}
bool Mat4::operator==(Data*b){return this->type==b->type && this->data==((Mat4*)b)->data;}


Object::Object(void*data){
  this->data=data;
  this->type=OBJECT;
}

std::string Object::toStr(){
  std::stringstream ss;
  ss<<this->data;
  return ss.str();
}

bool Object::operator==(Data*b){return this->type==b->type && this->data==((Object*)b)->data;}
Light::Light(glm::vec4 position,glm::vec3 diffuse,glm::vec3 specular){
  this->type     = LIGHT;
  this->position = position;
  this->diffuse  = diffuse;
  this->specular = specular;
}
std::string Light::toStr(){
  std::stringstream ss;
  ss<<this->position.x<<" "<<this->position.y<<" "<<this->position.z<<" "<<this->position.w<<" ";
  ss<<this->diffuse.x<<" "<<this->diffuse.y<<" "<<this->diffuse.z<<" ";
  ss<<this->specular.x<<" "<<this->specular.y<<" "<<this->specular.z;
  return ss.str();
}
bool Light::operator==(Data*b){
  return 
    this->type     == b->type && 
    this->position == ((Light*)b)->position && 
    this->diffuse  == ((Light*)b)->diffuse  &&
    this->specular == ((Light*)b)->specular;
}

std::vector<float>*Gauge::getData(){
  return &this->values;
}
void Gauge::insert(float value){
  this->values.push_back(value);
}
Gauge::Gauge(){this->type=GAUGE;}
std::string Gauge::toStr(){
  if(!this->values.size())return"";
  std::stringstream ss;
  ss<<this->values[0];
  for(unsigned i=1;i<this->values.size();++i)
    ss<<" "<<this->values[i];
  return ss.str();
}
bool Gauge::operator==(Data*b){
  if(this->type!=b->type)return false;
  Gauge*bb=(Gauge*)b;
  if(this->values.size()!=bb->values.size())return false;
  for(unsigned i=0;i<this->values.size();++i)
    if(this->values[i]!=bb->values[i])return false;
  return true;
}

