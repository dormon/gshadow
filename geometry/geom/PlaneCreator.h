#pragma once

#include"Point.h"
#include"Plane.h"

namespace geom{
  class Point;
  class Plane;
  class PlaneCreator{
    public:
      enum Type{
        TREE_POINTS    ,
        POINT_AND_PLANE,
      };
    protected:
      Type _type;
    public:
      PlaneCreator(Type type);
      virtual ~PlaneCreator();
      Type getType()const;
      virtual bool operator==(PlaneCreator const&other)const=0;
      virtual bool operator==(Plane const&point)const=0;
      virtual bool operator==(Point const&point)const=0;
  };
}
