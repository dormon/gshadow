#pragma once

#include"../creators/ThreePoints.h"
#include"../creators/PointAndPlane.h"

namespace geom{
  bool operator==(ThreePoints const&threePoints,PointAndPlane const&pointAndPlane);
  inline bool operator==(PointAndPlane const&pointAndPlane,ThreePoints const&threePoints){
    return threePoints==pointAndPlane;
  }
}
