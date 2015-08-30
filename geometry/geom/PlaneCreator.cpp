#include"PlaneCreator.h"

#include"creators/ThreePoints.h"
#include"creators/PointAndPlane.h"
#include"creators/ThreePlanes.h"
#include"eqCreators/ThreePointsEqPointAndPlane.h"
#include"eqCreators/PointAndPlaneEqThreePlanes.h"


using namespace geom;

PlaneCreator::PlaneCreator(Type type,Plane const&plane){
  this->_type  = type;
  this->_plane = plane.cloneWithoutCreator();
}

PlaneCreator::~PlaneCreator(){}

PlaneCreator::Type PlaneCreator::getType()const{
  return this->_type;
}

Plane const&PlaneCreator::getPlane()const{
  return this->_plane;
}

bool PlaneCreator::operator==(PlaneCreator const&other)const{
  switch(this->getType()){
    case PlaneCreator::THREE_POINTS:
      switch(other.getType()){
        case PlaneCreator::THREE_POINTS   :return this->to<ThreePoints>()==other.to<ThreePoints  >();
        case PlaneCreator::POINT_AND_PLANE:return this->to<ThreePoints>()==other.to<PointAndPlane>();
      }
    case PlaneCreator::POINT_AND_PLANE:
      switch(other.getType()){
        case PlaneCreator::THREE_POINTS   :return this->to<PointAndPlane>()==other.to<ThreePoints  >();
        case PlaneCreator::POINT_AND_PLANE:return this->to<PointAndPlane>()==other.to<PointAndPlane>();
      }
  }
  return false;
}

