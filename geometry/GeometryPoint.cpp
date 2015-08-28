#include"GeometryPoint.h"
#include<algorithm>

using namespace geom;

Point::Point(float x,float y,float z){
  this->x = x;
  this->y = y;
  this->z = z;
  this->_nofCreators = 0;
}

Point::Point(const float*data){
  for(unsigned i=0;i<3;++i)
    (*this)[i]=data[i];
  this->_nofCreators = 1;
}

Point::Point(glm::vec3 point){
  for(unsigned i=0;i<3;++i)
    (*this)[i]=point[i];
  this->_nofCreators = 1;
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
  this->_planes.push_back(a);
  this->_planes.push_back(b);
  this->_planes.push_back(c);
  this->_nofCreators = 3;
}

unsigned Point::creators()const{
  return this->_nofCreators;
}

bool Point::original()const{
  return this->creators()==0;
}

Plane Point::operator[](unsigned i)const{
  return this->_planes[i];
}

int  Point::relation(Point const&other)const{
  for(unsigned i=0;i<3;++i){
    if((*this)[i]<other[i])return -1;
    if((*this)[i]>other[i])return +1;
  }
  return 0;
}

bool Point::operator<(Point const&other)const{
  return ((Point*)this)->relation(other)<0;
}

bool Point::operator==(Point const&other)const{
  /*
  if(!this->creators()){
    if(!other.creators()){
      return ((Point*)this)->relation(other)==0;
    }else{
      for(unsigned i=0;i<3;++i)
        if(!(other._planes[i]==*this))return false;
      return true;
    }
  }else{
    if(!other.creators()){
      return other.operator==(*this);
    }else{
      for(unsigned i=0;i<3;++i)
        if(!(other._planes[i]==this->_planes[i]))return false;
      return true;
    }
  }*/
  //TODO
  return ((Point*)this)->relation(other)==0;
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

bool Point::operator==(Plane const&other)const{
  return true;
  //TODO
  /*
  if(!this->creators()){
    if(!other.creators()){
      return glm::dot(glm::vec4(other),glm::vec4(glm::vec3(*this),1.f))==0.0f;
    }else{
      for(unsigned i=0;i<other.creators();++i)
        if(other[i]==*this)return true;
      return other.relation(*this)==0;
    }
  }else{
    if(!other.creators()){
      return plane==*this;
    }else{
    }
  }*/
}

bool Point::contain    (Plane const&plane)const{
  //TODO
  //if(this->_planes.find(plane)!=this->_planes.end())return true;
  return false;
}

void Point::addPlane(Plane const&plane){
  //TODO
  //((Point*)this)->_planes.insert(plane);
}
