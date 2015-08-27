#pragma once

#include<vector>
#include<set>
#include<map>
#include<glm/glm.hpp>
#include"GeometryPlane.h"

namespace geom{
  class Plane;
  class Point: public glm::vec3{
    protected:
      std::set<Plane>_planes;
    public:
      Point(float x=0,float y=0,float z=0);
      Point(const float*data);
      Point(glm::vec3 point);
      Point(Plane const&a,Plane const&b,Plane const&c);
      int  relation(Point const&other);
      bool operator< (Point const&other)const;
      bool operator==(Point const&other)const;
      bool operator<=(Point const&other)const;
      bool operator> (Point const&other)const;
      bool operator>=(Point const&other)const;
      bool inFront( Plane const&plane)const;
      bool behind  (Plane const&plane)const;
      bool in      (Plane const&plane)const;
      void addPlane(Plane const&plane)const;
      void add(){}
      template<typename... ARGS>
      void add(Plane const&plane,ARGS...args){
        this->addPlane(plane);
        this->add(args...);
      }
  };
}
