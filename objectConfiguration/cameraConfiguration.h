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
#include"AntOpenDialog.h"


namespace objconf{
  void setCamera(ge::util::ArgumentObject*args,unsigned*windowSize);
  void setCameraAntTweakBar();
  void deinitCamera();
  ge::util::CameraObject*getCamera(unsigned i=0);

  class CameraConfiguration
  {
    private:
      ge::util::CameraObject* _camera;
      TwBar*                  _bar;
      AntOpenDialog*          _openDialog;
      static void _setInfinity(void*A);
      static void _set(void*A);
      static void _load(std::string name,void*A);
      static void _save(std::string name,void*A);
      static void _nearSet(const void*A,void*D);
      static void _nearGet(const void*A,void*D);
    public:
      CameraConfiguration(unsigned*windowSize,float fovy=45.,float near=1.,float far=9999);
      ~CameraConfiguration();
      ge::util::CameraObject*getCamera();
  };
}


#endif//_CAMERACONFIGURATION_H_
