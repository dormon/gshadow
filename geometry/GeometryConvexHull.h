#pragma once

#include"GeometryPoint.h"
#include"GeometryPlane.h"

/*
namespace geom{
  class ConvexHull{
    protected:
      std::set<Point>_points;
      std::set<Plane>_planes;
      void _createUsingCorners(glm::vec3 points[8]);
      bool _determinePlanarity();
    public:
      ConvexHull(glm::mat4 p,glm::mat4 v);
      ConvexHull(glm::vec3 minPoint,glm::vec3 maxPoint);
      ConvexHull(ConvexHull const&a,ConvexHull const&b);
      ConvexHull(ConvexHull const&hull,Point const&point);
      bool inside (Point const&a)const;
      Point center()const;
      std::set<Plane>::iterator getPlanar()const;
  };
}
*/
