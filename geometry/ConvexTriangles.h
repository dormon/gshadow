#ifndef _CONVEXTRIANGLES_H_
#define _CONVEXTRIANGLES_H_

#include"Common.h"
#include"Plane.h"
#include"WorkingObjects.h"
#include"ConvexHull.h"

namespace geometry{
  class ConvexTriangles{
    public:
      std::vector<glm::vec3>points;
      std::vector<unsigned>triangles;
      ConvexTriangles(ConvexHull*hull);
      void draw(DrawPrimitive*drawer);
  };
}

#endif//_CONVEXTRIANGLES_H_
