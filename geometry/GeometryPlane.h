#pragma once

#include<vector>
#include<set>
#include<map>
#include<glm/glm.hpp>
#include"GeometryPoint.h"

namespace geom{
  class Point;
  class Plane: public glm::vec4{
    protected:
      unsigned _nofCreators;
      std::vector<Point>_points;
    public:
      Plane(float a = 0,float b = 0,float c = 0,float d = 0);
      Plane(const float*data);
      Plane(glm::vec4 vec);
      Plane(Plane const&plane,Point const&point);
      Plane(Point const&a,Point const&b,Point const&c);
      unsigned creators()const;
      bool     original()const;
      Point operator[](unsigned i)const;
      int  relation  (Plane const&other)const;
      bool operator==(Plane const&other)const;
      bool operator< (Plane const&other)const;
      bool operator> (Plane const&other)const;
      bool operator<=(Plane const&other)const;
      bool operator>=(Plane const&other)const;
      bool operator==(Point const&other)const;
      bool contain    (Point const&point)const;
      void addPoint(Point const&point);
      void add(){}
      template<typename... ARGS>
      void add(Point const&point,ARGS...args){
        this->addPoint(point);
        this->add(args...);
      }
  };
}
