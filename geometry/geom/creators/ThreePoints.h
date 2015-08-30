#pragma once

#include"PlaneCreator.h"
#include"Point.h"

namespace geom{
  class ThreePoints: public PlaneCreator{
    protected:
      Point _inputPoints[3];
    public:
      ThreePoints(Point const&a,Point const&b,Point const&c,Plane const&plane);
      ThreePoints(ThreePoints const&other);
      Point const&getInputPoint(unsigned i)const;
      virtual PlaneCreator*clone()const;
      virtual bool operator==(ThreePoints const&other)const;
      virtual bool operator==(Plane const&plane)const;
      virtual bool operator==(Point const&point)const;
  };
}
