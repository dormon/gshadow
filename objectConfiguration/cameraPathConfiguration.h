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

namespace objconf{
  void setCameraPath();
  ge::util::CameraPath*getCameraPath();
  void setCameraPathAntTweakBar();
  class CameraPathConfiguration
  {
    private:
      ge::util::CameraPath*   _path;
      ge::util::CameraObject* _camera;
      TwBar*                  _bar;
      AntOpenDialog*          _openDialog;
      void _deletePath();
      static void _load(std::string name,void*A);
      static void _save(std::string name,void*A);
      static void _new(void*A);
      static void _push(void*A);
      static void _remove(void*A);
      bool     _draw;
      unsigned _selected;
      static void _selectSet(const void*A,void*D);
      static void _selectGet(void*A,void*D);
    public:
      CameraPathConfiguration();
      ~CameraPathConfiguration();
      ge::util::CameraPath*getPath();
      void setCamera(ge::util::CameraObject*camera);
      bool isDraw(){return this->_draw;}
  };

}

