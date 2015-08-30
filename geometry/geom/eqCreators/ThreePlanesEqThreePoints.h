#pragma once

#include"../creators/ThreePlanes.h"
#include"../creators/ThreePoints.h"

namespace geom{
  bool operator==(ThreePlanes const&threePlanes,ThreePoints const&threePoints);
  inline bool operator==(ThreePoints const&threePoints,ThreePlanes const&threePlanes){
    return threePlanes==threePoints;
  }
}
