#include"ThreePlanes.h"
#include"ThreePoints.h"
#include"PointAndPlane.h"

#include<iostream>
using namespace geom;

ThreePlanes::ThreePlanes(Plane const&a,Plane const&b,Plane const&c,Point const&point):PointCreator(PointCreator::THREE_PLANES,point){
  this->_inputPlanes[0]=a;
  this->_inputPlanes[1]=b;
  this->_inputPlanes[2]=c;
}

ThreePlanes::ThreePlanes(ThreePlanes const&other):PointCreator(
    PointCreator::THREE_PLANES,other.getPoint()){
  for(unsigned i=0;i<3;++i)
    this->_inputPlanes[i]=other._inputPlanes[i];
}

Plane const&ThreePlanes::getInputPlane(unsigned i)const{
  return this->_inputPlanes[i];
}

PointCreator*ThreePlanes::clone()const{
  return new ThreePlanes(this->_inputPlanes[0],this->_inputPlanes[1],this->_inputPlanes[2],this->getPoint());
}

bool ThreePlanes::operator==(ThreePlanes const&other)const{
  if(*this==other.getPoint())return true;
  if(other==this->getPoint())return true;
  for(unsigned i=0;i<3;++i)
    if(!(other==this->_inputPlanes[i]))return false;
  return true;
}

bool ThreePlanes::operator==(Point const&point)const{
  for(unsigned i=0;i<3;++i)
    if(!(this->_inputPlanes[i]==point))return false;
  return true;
}

bool ThreePlanes::operator==(Plane const&plane)const{
  for(unsigned i=0;i<3;++i){
    if(this->_inputPlanes[i]==plane)return true;
  }
  return false;
}


