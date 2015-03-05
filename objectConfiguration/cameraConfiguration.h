#ifndef _CAMERACONFIGURATION_H_
#define _CAMERACONFIGURATION_H_

#define GLM_FORCE_RADIANS
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>

#include<AntTweakBar.h>

#include<geUtil/ArgumentObject.h>
#include<geUtil/CameraObject.h>

#include"../simulationData/SimulationData.h"
#include<vector>

namespace objconf{
  void setCamera(ge::util::ArgumentObject*args,unsigned*windowSize);
  void setCameraAntTweakBar();
  void deinitCamera();
  ge::util::CameraObject*getCamera(unsigned i=0);
}


#endif//_CAMERACONFIGURATION_H_
