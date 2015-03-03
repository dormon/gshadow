#ifndef _PLANE_H_
#define _PLANE_H_

#include"Common.h"

namespace geometry{
 class Plane
 {
    public:
      glm::vec4 data;
      Plane();
      Plane(glm::vec4 plane);
      Plane(glm::vec3 a,glm::vec3 b,glm::vec3 c);
      float distance(glm::vec3 point);
      bool operator==(Plane b);
      Plane neg();
      bool isNan();
      int pointRelation(glm::vec3 a,glm::vec3 b);
      int relation(Plane p);
      std::string toStr();
      void draw(DrawPrimitive*drawer);
  };

  glm::vec3 planes2Point(Plane a,Plane b,Plane c);
}


#endif//_PLANE_H_
