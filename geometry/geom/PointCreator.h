#pragma once

#include"Point.h"
#include"Plane.h"

namespace geom{
  class Point;
  class Plane;
  class PointCreator{
    public:
      enum Type{
        PLANES_COLLISION=0
      };
    protected:
      Type _type;
    public:
      PointCreator(Type type);
      virtual ~PointCreator();
      Type getType()const;
      virtual bool operator==(PointCreator const&other)const=0;
      virtual bool operator==(Point const&point)const=0;
      virtual bool operator==(Plane const&plane)const=0;
  };
}
