#pragma once

#include<vector>
#include<set>
#include<map>
#include<glm/glm.hpp>
#include"Point.h"
#include"PlaneCreator.h"

namespace geom{
  class Point;
  class PlaneCreator;
  class Plane: public glm::vec4{
    protected:
      PlaneCreator*_creator = nullptr;
    public:
      Plane(float a = 0,float b = 0,float c = 0,float d = 0);
      Plane(const float*data);
      Plane(glm::vec4 vec);
      Plane(Plane const&plane,Point const&point);
      Plane(Point const&a,Point const&b,Point const&c);
      PlaneCreator*getCreator()const;
      int  relation  (Plane const&other)const;
      int  relation  (Point const&other)const;
      bool operator==(Plane const&other)const;
      bool operator< (Plane const&other)const;
      bool operator> (Plane const&other)const;
      bool operator<=(Plane const&other)const;
      bool operator>=(Plane const&other)const;
      bool operator==(Point const&other)const;
  };
}
