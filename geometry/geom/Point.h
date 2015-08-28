#pragma once

#include<vector>
#include<set>
#include<map>
#include<glm/glm.hpp>
#include"Plane.h"
#include"PointCreator.h"

namespace geom{
  class Plane;
  class PointCreator;
  class Point: public glm::vec3{
    protected:
      PointCreator*_creator = nullptr;
    public:
      Point(float x=0,float y=0,float z=0);
      Point(const float*data);
      Point(glm::vec3 point);
      Point(Plane const&a,Plane const&b,Plane const&c);
      PointCreator*getCreator()const;
      int  relation  (Point const&other)const;
      int  relation  (Plane const&plane)const;
      bool operator==(Point const&other)const;
      bool operator< (Point const&other)const;
      bool operator> (Point const&other)const;
      bool operator<=(Point const&other)const;
      bool operator>=(Point const&other)const;
      bool operator==(Plane const&plane)const;
  };
}
