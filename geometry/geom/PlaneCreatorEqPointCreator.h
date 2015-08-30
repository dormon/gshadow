#pragma once

#include"PlaneCreator.h"
#include"PointCreator.h"

namespace geom{
  bool operator==(PlaneCreator const&planeCreator,PointCreator const&pointCreator);
  inline bool operator==(PointCreator const&pointCreator,PlaneCreator const&planeCreator){
    return planeCreator==pointCreator;
  }
}
