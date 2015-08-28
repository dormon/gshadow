#include"Plane.h"

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

Plane::Plane(Plane const&plane,Point const&point){
  this->x = point.x;
  this->y = point.y;
  this->z = point.z;
  this->w = -glm::dot(glm::vec3((glm::vec4)(*this)),(glm::vec3)point);
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
}

PlaneCreator*Plane::getCreator()const{
  return this->_creator;
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

bool Plane::operator==(Point const&point)const{
  if(this->getCreator()){
    if(point.getCreator()){
      return false;//TODO
    }else{
      return *this->getCreator()==point;
    }
  }else{
    if(point.getCreator()){
      return *point.getCreator()==*this;
    }else{
      return this->relation(point)==0;
    }
  }
}

