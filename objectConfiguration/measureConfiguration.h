#pragma once

#define GLM_FORCE_RADIANS
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>

#include<AntTweakBar.h>

#include<geGL/geGL.h>
#include<geUtil/ArgumentObject.h>
#include<geUtil/CameraObject.h>
#include<geUtil/CameraPath.h>

#include"../simulationData/SimulationData.h"
#include<vector>
#include"AntOpenDialog.h"
#include"cameraConfiguration.h"
#include"cameraPathConfiguration.h"

namespace objconf{
  class MeasureConfiguration
  {
    private:
      objconf::CameraPathConfiguration*_path;
    public:
      MeasureConfiguration(objconf::CameraPathConfiguration*path);
  };
}
