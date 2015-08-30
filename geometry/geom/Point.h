#pragma once

#include<vector>
#include<set>
#include<map>
#include<glm/glm.hpp>
#include"Plane.h"
#include<iostream>

namespace geom{
  class Plane;
  class PointCreator;
  class Point: public glm::vec3{
    protected:
      PointCreator*_creator = nullptr;
      std::set<Plane>_planes;
    public:
      Point(float x=0,float y=0,float z=0);
      Point(const float*data);
      Point(glm::vec3 point);
      Point(Plane const&a,Plane const&b,Plane const&c);
      Point(Point const&point);
      Point&operator=(Point const&point);
      ~Point();
      PointCreator*getCreator()const;
      Point cloneWithoutCreator()const;
      int  relation  (Point const&other)const;
      int  relation  (Plane const&plane)const;
      bool operator==(Point const&other)const;
      bool operator< (Point const&other)const;
      bool operator> (Point const&other)const;
      bool operator<=(Point const&other)const;
      bool operator>=(Point const&other)const;
      bool operator!=(Point const&other)const;
      bool operator==(Plane const&plane)const;
      bool operator< (Plane const&plane)const;
      bool operator> (Plane const&plane)const;
      bool operator<=(Plane const&plane)const;
      bool operator>=(Plane const&plane)const;
      bool operator!=(Plane const&plane)const;
      bool invalid()const;
      std::string toStr()const;
      void addPlane(Plane const&plane);
      template<typename...>
      void add(){}
      template<typename... ARGS>
      void add(Plane const&plane,ARGS... args){
        this->addPlane(plane);
        this->add(args...);
      }
  };
}
