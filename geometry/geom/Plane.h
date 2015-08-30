#pragma once

#include<vector>
#include<set>
#include<map>
#include<glm/glm.hpp>
#include"Point.h"
#include<iostream>

namespace geom{
  class Point;
  class PlaneCreator;
  class Plane: public glm::vec4{
    protected:
      PlaneCreator*_creator = nullptr;
      std::set<Point>_points;
    public:
      Plane(float a = 0,float b = 0,float c = 0,float d = 0);
      Plane(const float*data);
      Plane(glm::vec4 vec);
      Plane(Plane const&plane,Point const&point);
      Plane(Point const&a,Point const&b,Point const&c);
      Plane(Plane const&other);
      Plane&operator=(Plane const&other);
      virtual ~Plane();
      PlaneCreator*getCreator()const;
      Plane cloneWithoutCreator()const;
      int  relation  (Plane const&other)const;
      int  relation  (Point const&other)const;
      bool operator==(Plane const&other)const;
      bool operator< (Plane const&other)const;
      bool operator> (Plane const&other)const;
      bool operator<=(Plane const&other)const;
      bool operator>=(Plane const&other)const;
      bool operator!=(Plane const&other)const;
      bool operator==(Point const&point)const;
      bool operator< (Point const&point)const;
      bool operator> (Point const&point)const;
      bool operator<=(Point const&point)const;
      bool operator>=(Point const&point)const;
      bool operator!=(Point const&point)const;
      bool invalid()const;
      std::string toStr()const;
      void addPoint(Point const&point);
      template<typename...>
      void add(){}
      template<typename... ARGS>
      void add(Point const&point,ARGS... args){
        this->addPoint(point);
        this->add(args...);
      }
  };
}
