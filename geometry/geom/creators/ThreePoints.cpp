#include"ThreePoints.h"
#include"ThreePlanes.h"

using namespace geom;

ThreePoints::ThreePoints(Point const&a,Point const&b,Point const&c,Plane const&plane):PlaneCreator(
    THREE_POINTS,plane){
  this->_inputPoints[0]=a;
  this->_inputPoints[1]=b;
  this->_inputPoints[2]=c;
}

ThreePoints::ThreePoints(ThreePoints const&other):PlaneCreator(THREE_POINTS,other.getPlane()){
  for(unsigned i=0;i<3;++i)
    this->_inputPoints[i]=other._inputPoints[i];
}

Point const&ThreePoints::getInputPoint(unsigned i)const{
  return this->_inputPoints[i];
}

PlaneCreator*ThreePoints::clone()const{
  return new ThreePoints(this->_inputPoints[0],this->_inputPoints[1],this->_inputPoints[2],this->getPlane());
}

bool ThreePoints::operator==(ThreePoints const&other)const{
  if(*this==other.getPlane())return true;
  if(other==this->getPlane())return true;
  for(unsigned i=0;i<3;++i)
    if(!(other==this->_inputPoints[i]))return false;
  return true;
}

bool ThreePoints::operator==(Plane const&plane)const{
  for(unsigned i=0;i<3;++i)
    if(!(this->_inputPoints[i]==plane))return false;
  return true;
}

bool ThreePoints::operator==(Point const&point)const{
  for(unsigned i=0;i<3;++i)
    if(this->_inputPoints[i]==point)return true;
  return false;
}

