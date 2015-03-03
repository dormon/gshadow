#ifndef _SHADOWMETHOD_H_
#define _SHADOWMETHOD_H_

#include<geGL/geGL.h>

#define GLM_FORCE_RADIANS
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>

#include"../app/adjacency.hpp"
#include"../simulationData/SimulationData.h"

class ShadowMethod
{
  public:
    virtual void createShadowMask()=0;
};


#endif//_SHADOWMETHOD_H_
