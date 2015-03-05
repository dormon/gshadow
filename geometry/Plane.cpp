#include"Plane.h"

#include<limits>

#include<iostream>
#include<sstream>

using namespace geometry;
Plane::Plane(){
  this->data=glm::vec4(0.f);
}

Plane::Plane(glm::vec4 plane){
  this->data=plane;
}

int Plane::pointRelation(glm::vec3 a,glm::vec3 b){
  return int(glm::dot(glm::sign(a-b),glm::vec3(4.f,2.f,1.f)));
}

Plane::Plane(glm::vec3 a,glm::vec3 b,glm::vec3 c){
 
  bool swap=false;
  if(this->pointRelation(a,b)>0){glm::vec3 z=a;a=b;b=z;swap=!swap;}
  if(this->pointRelation(b,c)>0){glm::vec3 z=b;b=c;c=z;swap=!swap;}
  if(this->pointRelation(a,b)>0){glm::vec3 z=a;a=b;b=z;swap=!swap;}

  glm::vec3 n=glm::normalize(glm::cross(glm::normalize(b-a),glm::normalize(c-a)));
  if(swap)n=-n;
  this->data.x=n.x;
  this->data.y=n.y;
  this->data.z=n.z;
  this->data.w=-glm::dot(n,a);
}

float Plane::distance(glm::vec3 point){
  return glm::dot(glm::vec3(this->data),point)+this->data.w;
}

bool Plane::operator==(Plane b){
  //float d=glm::dot(this->data,b.data);
  //return d<=std::numeric_limits<float>::epsilon()&&d>=-std::numeric_limits<float>::epsilon();

  float d=glm::length(glm::normalize(this->data)-glm::normalize(b.data));
  return d<0.0001;
}

Plane Plane::neg(){
  Plane result(this->data);
  result.data.x*=-1;
  result.data.y*=-1;
  result.data.z*=-1;
  result.data.w*=-1;
  return result;
}

bool Plane::isNan(){
  return glm::any(glm::isnan(this->data));
}
int Plane::relation(Plane p){
  return int(glm::dot(glm::sign(this->data-p.data),glm::vec4(8.f,4.f,2.f,1.f)));
}
std::string Plane::toStr(){
  std::stringstream ss;
  ss<<this->data[0]<<" "<<this->data[1]<<" "<<this->data[2]<<" "<<this->data[3];
  return ss.str();
}

void Plane::draw(DrawPrimitive*drawer){
  drawer->plane(this->data,0);
}

glm::vec3 geometry::planes2Point(Plane a,Plane b,Plane c){
  if(a.relation(b)>0){Plane z=a;a=b;b=z;}
  if(b.relation(c)>0){Plane z=b;b=c;c=z;} 
  if(a.relation(b)>0){Plane z=a;a=b;b=z;}
  glm::mat3 matrix;
  glm::vec3 result;
  matrix[0]=glm::vec3(a.data);
  matrix[1]=glm::vec3(b.data);
  matrix[2]=glm::vec3(c.data);
  float dt=glm::determinant(matrix);
  matrix[0]=glm::vec3(-a.data.w,a.data.y,a.data.z);
  matrix[1]=glm::vec3(-b.data.w,b.data.y,b.data.z);
  matrix[2]=glm::vec3(-c.data.w,c.data.y,c.data.z);
  result.x=glm::determinant(matrix);
  matrix[0]=glm::vec3(a.data.x,-a.data.w,a.data.z);
  matrix[1]=glm::vec3(b.data.x,-b.data.w,b.data.z);
  matrix[2]=glm::vec3(c.data.x,-c.data.w,c.data.z);
  result.y=glm::determinant(matrix);
  matrix[0]=glm::vec3(a.data.x,a.data.y,-a.data.w);
  matrix[1]=glm::vec3(b.data.x,b.data.y,-b.data.w);
  matrix[2]=glm::vec3(c.data.x,c.data.y,-c.data.w);
  result.z=glm::determinant(matrix);
  if(dt==0)return glm::vec3(std::numeric_limits<float>::quiet_NaN());
  return result/dt;
}

int Plane::relation(std::vector<glm::vec3>&p,unsigned sa,unsigned sb){
  bool inFrontOf=false;
  bool behind=false;
  for(unsigned i=0;i<p.size();++i){
    if(i==sa)continue;
    if(i==sb)continue;
    if(this->distance(p[i])<0)behind=true;
    else inFrontOf=true;
    if(inFrontOf&&behind)return 0;
  }
  if(inFrontOf)return 1;
  return-1;
}
