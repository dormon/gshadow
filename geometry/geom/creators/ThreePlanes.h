#pragma once

#include"PointCreator.h"
#include"Plane.h"

namespace geom{
  class ThreePlanes: public PointCreator{
    protected:
      Plane _inputPlanes[3];
    public:
      ThreePlanes(Plane const&a,Plane const&b,Plane const&c,Point const&point);
      ThreePlanes(ThreePlanes const&other);
      Plane const&getInputPlane(unsigned i)const;
      virtual PointCreator*clone()const;
      virtual bool operator==(ThreePlanes const&other)const;
      virtual bool operator==(Point const&point)const;
      virtual bool operator==(Plane const&plane)const;
  };
}
