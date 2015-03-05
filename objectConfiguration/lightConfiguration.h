#ifndef _LIGHTCONFIGURATION_H_
#define _LIGHTCONFIGURATION_H_

#define GLM_FORCE_RADIANS
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>

#include<AntTweakBar.h>

#include<geUtil/ArgumentObject.h>
#include"../simulationData/SimulationData.h"
#include<vector>



namespace objconf{
  void setLight(ge::util::ArgumentObject*args);
  void setLightAntTweakBar();
  void deinitLight();
  simulation::Light*getLight(unsigned i=0);
  struct{
    unsigned currentLight;
    TwBar*lbar;
    std::vector<simulation::Light*>lights;
  }lightConfiguration;
}

#endif//_LIGHTCONFIGURATION_H_
