#pragma once

#include"PlaneCreator.h"
#include"Point.h"

namespace geom{
  class PointAndPlane: public PlaneCreator{
    protected:
      Point _inputPoint;
      Plane _inputPlane;
    public:
      PointAndPlane(Point const&point,Plane const&plane,Plane const&finalPlane);
      PointAndPlane(PointAndPlane const&other);
      Point const&getInputPoint()const;
      Plane const&getInputPlane()const;
      PlaneCreator*clone()const;
      virtual bool operator==(PointAndPlane const&other)const;
      virtual bool operator==(Plane const&plane)const;
      virtual bool operator==(Point const&point)const;
  };
}
