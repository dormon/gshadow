#pragma once

#include<AntTweakBar.h>
#include<vector>
#include<iostream>
#include"../methods/ShadowMethod.h"

namespace objconf{

  class ShadowMethodConfig
  {
    private:
      TwBar* _bar;
    public:
      ShadowMethodConfig(std::string name,simulation::SimulationObject*sm,simulation::SimulationData*sd);
      ~ShadowMethodConfig();
  };
}
