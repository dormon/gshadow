#pragma once

#include"PointCreator.h"
#include"Plane.h"

namespace geom{
  class PlanesCollision: public PointCreator{
    protected:
      Plane _planes[3];
    public:
      PlanesCollision(Plane const&a,Plane const&b,Plane const&c);
      bool operator==(PointCreator*other)const;
      bool operator==(Point const&point)const;
  };
}
