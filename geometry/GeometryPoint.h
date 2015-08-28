#pragma once

#include<vector>
#include<set>
#include<map>
#include<glm/glm.hpp>
#include"GeometryPlane.h"

namespace geom{
  class Plane;

  class PointCreator{
    protected:
    public:
      virtual bool operator==(PointCreator const&other)const=0;
  };

  class Point: public glm::vec3{
    protected:

      unsigned _nofCreators;
      std::vector<Plane>_planes;
    public:
      Point(float x=0,float y=0,float z=0);
      Point(const float*data);
      Point(glm::vec3 point);
      Point(Plane const&a,Plane const&b,Plane const&c);
      unsigned creators()const;
      bool     original()const;
      Plane operator[](unsigned i)const;
      int  relation  (Point const&other)const;
      bool operator==(Point const&other)const;
      bool operator< (Point const&other)const;
      bool operator> (Point const&other)const;
      bool operator<=(Point const&other)const;
      bool operator>=(Point const&other)const;
      bool operator==(Plane const&other)const;
      bool contain    (Plane const&plane)const;
      void addPlane(Plane const&plane);
      void add(){}
      template<typename... ARGS>
      void add(Plane const&plane,ARGS...args){
        this->addPlane(plane);
        this->add(args...);
      }
  };
}
