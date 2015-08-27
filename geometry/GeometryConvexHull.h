#pragma once

#include"GeometryPoint.h"
#include"GeometryPlane.h"

namespace geom{
  class ConvexHull{
    protected:
      std::set<Point>_points;
      std::set<Plane>_planes;
    public:
      ConvexHull(glm::mat4 p,glm::mat4 v);
      ConvexHull(glm::vec3 minPoint,glm::vec3 maxPoint);
      ConvexHull(ConvexHull const&a,ConvexHull const&b);
      void extend(Point const&point);
  };
}
