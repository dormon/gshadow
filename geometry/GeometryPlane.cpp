#include"GeometryPlane.h"

using namespace geom;

Plane::Plane(float a,float b,float c,float d){
  this->x = a;
  this->y = b;
  this->z = c;
  this->w = d;
  this->_nofCreators = 0;
}

Plane::Plane(const float*data){
  for(unsigned i=0;i<4;++i)
    (*this)[i]=data[i];
  this->_nofCreators = 0;
}

Plane::Plane(glm::vec4 vec){
  for(unsigned i=0;i<4;++i)
    (*this)[i]=vec[i];
  this->_nofCreators = 0;
}

Plane::Plane(Plane const&plane,Point const&point){
  this->x = point.x;
  this->y = point.y;
  this->z = point.z;
  this->w = -glm::dot(glm::vec3((glm::vec4)(*this)),(glm::vec3)point);
  this->add(point);
  this->_nofCreators = 1;
}

Plane::Plane(Point const&A,Point const&B,Point const&C){
  Point a=A;
  Point b=B;
  Point c=C;
  std::vector<Point>points;
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

  this->_points.push_back(a);
  this->_points.push_back(b);
  this->_points.push_back(c);
  this->_nofCreators = 3;
}

unsigned Plane::creators()const{
  return this->_nofCreators;
}

bool  Plane::original()const{
  return this->creators()==0;
}

Point Plane::operator[](unsigned i)const{
  return this->_points[i];
}

int  Plane::relation(Plane const&other)const{
  for(unsigned i=0;i<4;++i){
    if((*this)[i]<other[i])return -1;
    if((*this)[i]>other[i])return +1;
  }
  return 0;
}

bool Plane::operator<(Plane const&other)const{
  return ((Plane*)this)->relation(other)<0;
}

bool Plane::operator==(Plane const&other)const{
  return ((Plane*)this)->relation(other)==0;
}

bool Plane::operator<=(Plane const&other)const{
  return ((Plane*)this)->relation(other)<=0;
}

bool Plane::operator> (Plane const&other)const{
  return ((Plane*)this)->relation(other)>0;
}

bool Plane::operator>=(Plane const&other)const{
  return ((Plane*)this)->relation(other)>=0;
}

bool Plane::contain(Point const&point)const{
  //TODO
  //if(this->_points.find(point)!=this->_points.end())return true;
  return false;
}

void Plane::addPoint(Point const&point){
  //TODO
  //((Plane*)this)->_points.insert(point);
}
