#pragma once

#include"../creators/PointAndPlane.h"
#include"../creators/ThreePlanes.h"

namespace geom{
  bool operator==(PointAndPlane const&pointAndPlane,ThreePlanes const&threePlanes);
  inline bool operator==(ThreePlanes const&threePlanes,PointAndPlane const&pointAndPlane){
    return pointAndPlane==threePlanes;
  }
}
