#pragma once

#include"Point.h"
#include"Plane.h"

namespace geom{
  class Point;
  class Plane;
  class PointCreator{
    public:
      enum Type{
        THREE_PLANES=0
      };
    protected:
      Type  _type ;
      Point _point;
    public:
      PointCreator(Type type,Point const&point);
      virtual ~PointCreator();
      Type getType()const;
      Point const&getPoint()const;
      virtual PointCreator*clone()const=0;
      template<typename TYPE>inline TYPE to()const{return *((TYPE*)this);}
      virtual bool operator==(PointCreator const&other)const;
      virtual bool operator==(Point const&point)const=0;
      virtual bool operator==(Plane const&plane)const=0;
  };
}
