#include"PointCreator.h"

using namespace geom;

PointCreator::PointCreator(Type type){
  this->_type = type;
}

PointCreator::~PointCreator(){}

PointCreator::Type PointCreator::getType()const{
  return this->_type;
}
