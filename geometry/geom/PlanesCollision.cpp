#include"PlanesCollision.h"

using namespace geom;

PlanesCollision::PlanesCollision(Plane const&a,Plane const&b,Plane const&c):PointCreator(PointCreator::PLANES_COLLISION){
  this->_planes[0]=a;
  this->_planes[1]=b;
  this->_planes[2]=c;
}

bool PlanesCollision::operator==(PointCreator*other)const{
  if(this->getType()!=other->getType())return false;
  for(unsigned i=0;i<3;++i)
    if(!(this->_planes[i]==((PlanesCollision*)other)->_planes[i]))return false;
  return true;
}

bool PlanesCollision::operator==(Point const&point)const{
  for(unsigned i=0;i<3;++i)
    if(!(this->_planes[i]==point))return false;
  return true;
}
