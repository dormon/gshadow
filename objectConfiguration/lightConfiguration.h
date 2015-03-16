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
#include"AntOpenDialog.h"



namespace objconf{
  void setLight(ge::util::ArgumentObject*args);
  void setLightAntTweakBar();
  void deinitLight();
  //simulation::Light*getLight(unsigned i=0);
  struct{
    unsigned currentLight;
    TwBar*lbar;
    std::vector<simulation::Light*>lights;
  }lightConfiguration;

  class LightConfiguration{
    private:
      TwBar*            _bar;
      simulation::Light*_light;
      AntOpenDialog*    _openDialog;
      static void _load(std::string name,void*A);
      static void _save(std::string name,void*A);
    public:
      LightConfiguration();
      ~LightConfiguration();
      simulation::Light*getLight(){return this->_light;}
  };
}

#endif//_LIGHTCONFIGURATION_H_
