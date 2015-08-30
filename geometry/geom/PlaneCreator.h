#pragma once

#include"Point.h"
#include"Plane.h"

namespace geom{
  class Plane;
  class Point;
  class PlaneCreator{
    public:
      enum Type{
        THREE_POINTS   ,
        POINT_AND_PLANE,
      };
    protected:
      Type  _type ;
      Plane _plane;
    public:
      PlaneCreator(Type type,Plane const&plane);
      virtual ~PlaneCreator();
      Type getType()const;
      Plane const&getPlane()const;
      virtual PlaneCreator*clone()const=0;
      template<typename TYPE>inline TYPE to()const{return *((TYPE*)this);}
      virtual bool operator==(PlaneCreator const&other)const;
      virtual bool operator==(Plane const&plane)const=0;
      virtual bool operator==(Point const&point)const=0;
  };
}
