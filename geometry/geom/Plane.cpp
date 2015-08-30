#include"Plane.h"
#include"PlaneCreator.h"
#include"PointCreator.h"
#include"PlaneCreatorEqPointCreator.h"
#include"creators/ThreePoints.h"
#include"creators/PointAndPlane.h"

#include<sstream>
#include<geCore/dtemplates.h>

using namespace geom;

Plane::Plane(float a,float b,float c,float d){
  this->x = a;
  this->y = b;
  this->z = c;
  this->w = d;
  this->_creator = nullptr;
}

Plane::Plane(const float*data){
  for(unsigned i=0;i<4;++i)
    (*this)[i]=data[i];
  this->_creator = nullptr;
}

Plane::Plane(glm::vec4 vec){
  for(unsigned i=0;i<4;++i)
    (*this)[i]=vec[i];
  this->_creator = nullptr;
}

Plane::Plane(Plane const&plane,Point const&point){
  this->x = point.x;
  this->y = point.y;
  this->z = point.z;
  this->w = -glm::dot(glm::vec3((glm::vec4)(*this)),(glm::vec3)point);
  this->_creator = new PointAndPlane(point,plane,*this);
}

Plane::Plane(Point const&A,Point const&B,Point const&C){
  Point a=A;
  Point b=B;
  Point c=C;
  bool swap=false;
  if(b<a){Point z=a;a=b;b=z;swap=!swap;}
  if(c<b){Point z=b;b=c;c=z;swap=!swap;}
  if(b<a){Point z=a;a=b;b=z;swap=!swap;}

  glm::vec3 n=glm::normalize(glm::cross(glm::normalize(b-a),glm::normalize(c-a)));
  if(swap)n=-n;
  this->x=n.x;
  this->y=n.y;
  this->z=n.z;
  this->w=-glm::dot(n,(glm::vec3)a);
  this->_creator = new ThreePoints(a,b,c,*this);
}

Plane::Plane(Plane const&other){
  this->x = other.x;
  this->y = other.y;
  this->z = other.z;
  this->w = other.w;
  this->_points = other._points;
  if(other._creator)this->_creator = other._creator->clone();
  else this->_creator = nullptr;
}

Plane&Plane::operator=(Plane const&other){
  this->~Plane();
  this->x = other.x;
  this->y = other.y;
  this->z = other.z;
  this->w = other.w;
  this->_points = other._points;
  if(other._creator)this->_creator = other._creator->clone();
  else this->_creator = nullptr;
  return *this;
}


Plane::~Plane(){
  if(this->_creator)delete this->_creator;
  this->_points.clear();
}

PlaneCreator*Plane::getCreator()const{
  return this->_creator;
}

Plane Plane::cloneWithoutCreator()const{
  Plane plane;
  plane.x = this->x;
  plane.y = this->y;
  plane.z = this->z;
  plane.w = this->w;
  plane._points = this->_points;
  plane._creator = nullptr;
  return plane;
}


int  Plane::relation(Plane const&other)const{
  for(unsigned i=0;i<4;++i){
    if((*this)[i]<other[i])return -1;
    if((*this)[i]>other[i])return +1;
  }
  return 0;
}

int  Plane::relation  (Point const&point)const{
  return int(glm::sign(glm::dot(glm::vec4(*this),glm::vec4(glm::vec3(point),1.f))));
}

bool Plane::operator==(Plane const&other)const{
  if(this->relation(other)==0)return true;
  unsigned nofMyPointsOnOther=0;
  for(auto x:this->_points){
    if(other==x)nofMyPointsOnOther++;
    if(nofMyPointsOnOther==3)return true;
  }
  unsigned nofOtherPointsOnMe=0;
  for(auto x:other._points){
    if(*this==x)nofOtherPointsOnMe++;
    if(nofOtherPointsOnMe==3)return true;
  }
  if(this->getCreator()){
    if(other.getCreator())return *this->getCreator()==*other.getCreator();
    else return *this->getCreator()==other;
  }else if(other.getCreator())return *other.getCreator()==*this;
  return false;
}

bool Plane::operator<(Plane const&other)const{
  if(*this==other)return false;
  return this->relation(other)<0;
}

bool Plane::operator> (Plane const&other)const{
  if(*this==other)return false;
  return this->relation(other)>0;
}

bool Plane::operator<=(Plane const&other)const{
  if(*this==other)return true;
  return this->relation(other)<=0;
}


bool Plane::operator>=(Plane const&other)const{
  if(*this==other)return true;
  return this->relation(other)>=0;
}


bool Plane::operator!=(Plane const&other)const{
  return !(*this==other);
}

bool Plane::operator==(Point const&point)const{
  if(this->relation(point)==0)return true;
  if(this->_points.find(point)!=this->_points.end())return true;
  if(this->getCreator()){
    if(point.getCreator())return *this->getCreator()==*point.getCreator();
    else return *this->getCreator()==point;
  }else if(point.getCreator())return *point.getCreator()==*this;
  return false;
}

bool Plane::operator< (Point const&point)const{
  if(*this==point)return false;
  return this->relation(point)<0;
}

bool Plane::operator> (Point const&point)const{
  if(*this==point)return false;
  return this->relation(point)>0;
}

bool Plane::operator<=(Point const&point)const{
  if(*this==point)return true;
  return this->relation(point)<=0;
}

bool Plane::operator>=(Point const&point)const{
  if(*this==point)return true;
  return this->relation(point)>=0;
}

bool Plane::operator!=(Point const&point)const{
  return !(*this==point);
}

bool Plane::invalid()const{
  return 
    std::isnan(this->x)||
    std::isnan(this->y)||
    std::isnan(this->z)||
    std::isnan(this->w)||
    (this->x==0.f&&
     this->y==0.f&&
     this->z==0.f&&
     this->w==0.f);
}

std::string Plane::toStr()const{
  std::stringstream ss;
  ss<<this->x<<" "<<this->y<<" "<<this->z<<" "<<this->w;
  return ss.str();
}

void Plane::addPoint(Point const&point){
  this->_points.insert(point);
}
