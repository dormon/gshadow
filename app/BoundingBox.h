#ifndef _BOUNDINGBOX_H_
#define _BOUNDINGBOX_H_

#include<glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>

class AxisAlignBoundingBox
{
  public:
    glm::vec3 minPoint;
    glm::vec3 maxPoint;
    AxisAlignBoundingBox();
    void addPoint(glm::vec3 point);
    glm::vec3 getCenter();
    glm::vec3 getSize();
    glm::vec3 getHalfSize();
};

class OrthoBoundingBox
{
  public:
    glm::vec3 minPoint;
    glm::vec3 xAxis;
    glm::vec3 yAxis;
    glm::vec3 zAxis;
};

#endif//_BOUNDINGBOX_H_
