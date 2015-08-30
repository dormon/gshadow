#include"PointAndPlane.h"
#include"ThreePlanes.h"

using namespace geom;

PointAndPlane::PointAndPlane(Point const&point,Plane const&plane,Plane const&finalPlane):PlaneCreator(POINT_AND_PLANE,finalPlane){
  this->_inputPoint = point;
  this->_inputPlane = plane;
}

PointAndPlane::PointAndPlane(PointAndPlane const&other):PlaneCreator(POINT_AND_PLANE,other.getPlane()){
  this->_inputPoint = other._inputPoint;
  this->_inputPlane = other._inputPlane;
}

Point const&PointAndPlane::getInputPoint()const{
  return this->_inputPoint;
}

Plane const&PointAndPlane::getInputPlane()const{
  return this->_inputPlane;
}

PlaneCreator*PointAndPlane::clone()const{
  return new PointAndPlane(this->getInputPoint(),this->getInputPlane(),this->getPlane());
}


bool PointAndPlane::operator==(PointAndPlane const&other)const{
  return 
    (this->getPlane()==other.getPlane())||(
     (this->getInputPoint()==((PointAndPlane*)&other)->getInputPoint())&&
     (this->getInputPlane()==((PointAndPlane*)&other)->getInputPlane()));
}

bool PointAndPlane::operator==(Plane const&plane)const{
  if(plane.relation(this->getInputPoint())==0)return true;
  return false;
}

bool PointAndPlane::operator==(Point const&point)const{
  if(this->getInputPoint()==point)return true;
  return false;
}

