#include"PointCreator.h"
#include"creators/ThreePlanes.h"

using namespace geom;

PointCreator::PointCreator(Type type,Point const&point){
  this->_type  = type ;
  this->_point = point.cloneWithoutCreator();
}

PointCreator::~PointCreator(){}

bool PointCreator::operator==(PointCreator const&other)const{
  switch(this->getType()){
    case PointCreator::THREE_PLANES:
      switch(other.getType()){
        case PointCreator::THREE_PLANES:return this->to<ThreePlanes>()==other.to<ThreePlanes>();
      }
  }
  return false;
}

PointCreator::Type PointCreator::getType()const{
  return this->_type;
}

Point const&PointCreator::getPoint()const{
  return this->_point;
}
