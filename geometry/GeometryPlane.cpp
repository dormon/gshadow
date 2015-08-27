#include"GeometryPlane.h"

using namespace geom;

Plane::Plane(float a,float b,float c,float d){
  this->x = a;
  this->y = b;
  this->z = c;
  this->w = d;
}

Plane::Plane(const float*data){
  for(unsigned i=0;i<4;++i)
    (*this)[i]=data[i];
}

Plane::Plane(glm::vec4 vec){
  for(unsigned i=0;i<4;++i)
    (*this)[i]=vec[i];
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

  this->_points.insert(a);
  this->_points.insert(b);
  this->_points.insert(c);
}

int  Plane::relation(Plane const&other){
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

bool Plane::inFront(Point const&point)const{
  return glm::dot(glm::vec4(*this),glm::vec4(glm::vec3(point),1))>0;
}

bool Plane::behind (Point const&point)const{
  return glm::dot(glm::vec4(*this),glm::vec4(glm::vec3(point),1))<0;
}

bool Plane::in     (Point const&point)const{
  if(this->_points.find(point)!=this->_points.end())return true;
  return glm::dot(glm::vec4(*this),glm::vec4(glm::vec3(point),1))==0;
}

void Plane::addPoint(Point const&point)const{
  ((Plane*)this)->_points.insert(point);
}
