#include"Point.h"
#include<algorithm>
#include"PointCreator.h"
#include"PlaneCreator.h"
#include"PlaneCreatorEqPointCreator.h"
#include"creators/ThreePlanes.h"

#include<sstream>
#include<geCore/dtemplates.h>

using namespace geom;

Point::Point(float x,float y,float z){
  this->x = x;
  this->y = y;
  this->z = z;
}

Point::Point(const float*data){
  for(unsigned i=0;i<3;++i)
    (*this)[i]=data[i];
}

Point::Point(glm::vec3 point){
  for(unsigned i=0;i<3;++i)
    (*this)[i]=point[i];
}

Point::Point(Plane const&A,Plane const&B,Plane const&C){
  Plane a=A;
  Plane b=B;
  Plane c=C;
  if(b<a){Plane z=a;a=b;b=z;}
  if(c<b){Plane z=b;b=c;c=z;}
  if(b<a){Plane z=a;a=b;b=z;}
  glm::mat3 matrix;
  glm::vec3 result;
  matrix[0]=glm::vec3(a);
  matrix[1]=glm::vec3(b);
  matrix[2]=glm::vec3(c);
  float dt=glm::determinant(matrix);
  matrix[0]=glm::vec3(-a.w,a.y,a.z);
  matrix[1]=glm::vec3(-b.w,b.y,b.z);
  matrix[2]=glm::vec3(-c.w,c.y,c.z);
  result.x=glm::determinant(matrix);
  matrix[0]=glm::vec3(a.x,-a.w,a.z);
  matrix[1]=glm::vec3(b.x,-b.w,b.z);
  matrix[2]=glm::vec3(c.x,-c.w,c.z);
  result.y=glm::determinant(matrix);
  matrix[0]=glm::vec3(a.x,a.y,-a.w);
  matrix[1]=glm::vec3(b.x,b.y,-b.w);
  matrix[2]=glm::vec3(c.x,c.y,-c.w);
  result.z=glm::determinant(matrix);
  if(dt==0){
    this->x = std::numeric_limits<float>::quiet_NaN();
    this->y = std::numeric_limits<float>::quiet_NaN();
    this->z = std::numeric_limits<float>::quiet_NaN();
    return;
  }
  this->x=result.x/dt;
  this->y=result.y/dt;
  this->z=result.z/dt;
  this->_creator = new ThreePlanes(a,b,c,*this);
}

Point::Point(Point const&point){
  this->x = point.x;
  this->y = point.y;
  this->z = point.z;
  this->_planes = point._planes;
  if(point._creator)this->_creator = point._creator->clone();
  else this->_creator = nullptr;
}

Point&Point::operator=(Point const&point){
  this->~Point();
  this->x = point.x;
  this->y = point.y;
  this->z = point.z;
  this->_planes = point._planes;
  if(point._creator)this->_creator = point._creator->clone();
  else this->_creator = nullptr;
  return *this;
}

Point::~Point(){
  delete this->_creator;
  this->_planes.clear();
}

PointCreator*Point::getCreator()const{
  return this->_creator;
}

Point Point::cloneWithoutCreator()const{
  Point point;
  point.x = this->x;
  point.y = this->y;
  point.z = this->z;
  point._planes = this->_planes;
  point._creator = nullptr;
  return point;
}

int  Point::relation(Point const&other)const{
  for(unsigned i=0;i<3;++i){
    if((*this)[i]<other[i])return -1;
    if((*this)[i]>other[i])return +1;
  }
  return 0;
}

int  Point::relation  (Plane const&plane)const{
  return int(glm::sign(glm::dot((glm::vec4)(plane),glm::vec4((glm::vec3)(*this),1.f))));
}

bool Point::operator==(Point const&other)const{
  if(this->relation(other)==0)return true;
  unsigned nofMyPlanesOnOther=0;
  for(auto x:this->_planes){
    if(other==x)nofMyPlanesOnOther++;
    if(nofMyPlanesOnOther==3)return true;
  }
  unsigned nofOtherPlanesOnMe=0;
  for(auto x:other._planes){
    if(*this==x)nofOtherPlanesOnMe++;
    if(nofOtherPlanesOnMe==3)return true;
  }
  if(this->getCreator()){
    if(other.getCreator())return *this->getCreator()==*other.getCreator();
    else return *this->getCreator()==other;
  }else if(other.getCreator())return *other.getCreator()==*this;
  return false;
}

bool Point::operator<(Point const&other)const{
  if(*this==other)return false;
  return this->relation(other)<0;
}

bool Point::operator>(Point const&other)const{
  if(*this==other)return false;
  return this->relation(other)>0;
}

bool Point::operator<=(Point const&other)const{
  if(*this==other)return true;
  return this->relation(other)<=0;
}

bool Point::operator>=(Point const&other)const{
  if(*this==other)return true;
  return this->relation(other)>=0;
}

bool Point::operator!=(Point const&other)const{
  return !(*this==other);
}

bool Point::operator==(Plane const&plane)const{
  if(this->relation(plane)==0)return true;
  if(this->_planes.find(plane)!=this->_planes.end())return true;
  if(this->getCreator()){
    if(plane.getCreator())return *this->getCreator()==*plane.getCreator();
    else return *this->getCreator()==plane;
  }else if(plane.getCreator())return *plane.getCreator()==*this;
  return false;
}

bool Point::operator< (Plane const&plane)const{
  if(*this==plane)return false;
  return this->relation(plane)<0;
}

bool Point::operator> (Plane const&plane)const{
  if(*this==plane)return false;
  return this->relation(plane)>0;
}

bool Point::operator<=(Plane const&plane)const{
  if(*this==plane)return true;
  return this->relation(plane)<=0;
}

bool Point::operator>=(Plane const&plane)const{
  if(*this==plane)return true;
  return this->relation(plane)>=0;
}

bool Point::operator!=(Plane const&plane)const{
  return !(*this==plane);
}

bool Point::invalid()const{
  return std::isnan(this->x)||std::isnan(this->y)||std::isnan(this->z);
}

std::string Point::toStr()const{
  std::stringstream ss;
  ss<<this->x<<" "<<this->y<<" "<<this->z;
  return ss.str();
}

void Point::addPlane(Plane const&plane){
  this->_planes.insert(plane);
}
