#include"Point.h"
#include<algorithm>

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
}

PointCreator*Point::getCreator()const{
  return this->_creator;
}

int  Point::relation(Point const&other)const{
  for(unsigned i=0;i<3;++i){
    if((*this)[i]<other[i])return -1;
    if((*this)[i]>other[i])return +1;
  }
  return 0;
}

int  Point::relation  (Plane const&plane)const{
  return int(glm::sign(glm::dot(glm::vec4(plane),glm::vec4(glm::vec3(*this),1.f))));
}

bool Point::operator<(Point const&other)const{
  return ((Point*)this)->relation(other)<0;
}

bool Point::operator==(Point const&other)const{
  if(this->_creator){
    if(other._creator){
      return *this->_creator==*other._creator;
    }else{
      return *this->_creator==other;
    }
  }else{
    if(other._creator){
      return *other._creator==*this;
    }else{
      return ((Point*)this)->relation(other)==0;
    }
  }
}

bool Point::operator<=(Point const&other)const{
  return ((Point*)this)->relation(other)<=0;
}

bool Point::operator>(Point const&other)const{
  return ((Point*)this)->relation(other)>0;
}

bool Point::operator>=(Point const&other)const{
  return ((Point*)this)->relation(other)>=0;
}

bool Point::operator==(Plane const&plane)const{
  if(this->getCreator()){
    if(plane.getCreator()){
      return false;//TODO
    }else{
      return *this->getCreator()==plane;
    }
  }else{
    if(plane.getCreator()){
      return *plane.getCreator()==*this;
    }else{
      return this->relation(plane)==0;
    }
  }
}

