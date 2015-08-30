#include"ThreePlanesEqThreePoints.h"

using namespace geom;

bool geom::operator==(ThreePlanes const&threePlanes,ThreePoints const&threePoints){
  if(threePlanes==threePoints.getPlane())return true;
  if(threePoints==threePlanes.getPoint())return true;

  for(unsigned i=0;i<3;++i)
    if(threePlanes.getInputPlane(i)==threePoints.getPlane())return true;
  for(unsigned i=0;i<3;++i)
    if(threePoints.getInputPoint(i)==threePlanes.getPoint())return true;

  for(unsigned i=0;i<3;++i)
    if(threePlanes==threePoints.getInputPoint(i))return true;
  for(unsigned i=0;i<3;++i)
    if(threePoints==threePlanes.getInputPlane(i))return true;

  return false;
}

