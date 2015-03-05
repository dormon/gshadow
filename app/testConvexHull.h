#ifndef _TESTCONVEXHULL_H_
#define _TESTCONVEXHULL_H_

#define GLM_FORCE_RADIANS
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>

#include<AntTweakBar.h>

#include"../drawPrimitive/drawPrimitive.h"
#include"../geometry/Geometry.h"

namespace test{
  void drawConvexHull(
      glm::mat4 view,glm::mat4 projection,
      glm::mat4 p,glm::mat4 v,glm::vec3 minPoint,glm::vec3 maxPoint,glm::vec3 light);
  void setTestConvexHull(DrawPrimitive*simp);
}

#endif//_TESTCONVEXHULL_H_
