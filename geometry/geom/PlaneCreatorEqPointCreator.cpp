#include"PlaneCreatorEqPointCreator.h"
#include"ThreePlanesEqThreePoints.h"
#include"PointAndPlaneEqThreePlanes.h"

using namespace geom;

bool geom::operator==(PlaneCreator const&planeCreator,PointCreator const&pointCreator){
  switch(planeCreator.getType()){
    case PlaneCreator::THREE_POINTS   :
      switch(pointCreator.getType()){
        case PointCreator::THREE_PLANES:return planeCreator.to<ThreePoints>()==pointCreator.to<ThreePlanes>();
      }
    case PlaneCreator::POINT_AND_PLANE:
      switch(pointCreator.getType()){
        case PointCreator::THREE_PLANES:return planeCreator.to<PointAndPlane>()==pointCreator.to<ThreePlanes>();
      }
  }
  return false;
}
