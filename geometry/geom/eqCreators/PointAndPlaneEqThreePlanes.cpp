#include"PointAndPlaneEqThreePlanes.h"

using namespace geom;

bool geom::operator==(PointAndPlane const&pointAndPlane,ThreePlanes const&threePlanes){
  return pointAndPlane.getInputPoint()==threePlanes.getPoint();
}
