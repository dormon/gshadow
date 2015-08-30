#include"ThreePointsEqPointAndPlane.h"

using namespace geom;

bool geom::operator==(ThreePoints const&threePoints,PointAndPlane const&pointAndPlane){
  return threePoints==pointAndPlane.getPlane();
}
