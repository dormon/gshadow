#include"cameraConfiguration.h"

#include<sstream>

using namespace objconf;

#define FAR_LIMIT 10000

CameraConfiguration::CameraConfiguration(glm::uvec2 windowSize,float fovy,float near,float far){
  this->_bar=TwNewBar("camera configurator");
  this->_camera = new ge::util::CameraObject(
      glm::value_ptr(windowSize),
      near,
      far,
      fovy);
  TwAddVarCB(this->_bar,"near",TW_TYPE_FLOAT,this->_nearSet,this->_nearGet,this,"label='near' step=0.1");
  TwAddVarCB(this->_bar,"far" ,TW_TYPE_FLOAT,this->_farSet ,this->_farGet ,this,"label='far'  step=1.0");
  TwAddButton(this->_bar,"resetFar",_resetFar,this,"label='reset far'");

  TwAddVarRW(this->_bar,"mouseLock",TW_TYPE_BOOLCPP,&this->_mouseLock,
      " help='mouse lock' ");

  this->_mouseLock = false;
  this->_openDialog = new AntOpenDialog(this->_bar,"camera_configurator");
  this->_openDialog->setOpenCallBack(this->_load,this);
  this->_openDialog->setSaveCallBack(this->_save,this);
}

void CameraConfiguration::_resetFar(void*A){
  CameraConfiguration*_this=(CameraConfiguration*)A;
  _this->_camera->setFar(FAR_LIMIT);

}

void CameraConfiguration::_nearSet(const void*A,void*D){
  CameraConfiguration*_this=(CameraConfiguration*)D;
  float newNear=*(float*)A;
  if(newNear<=0)newNear=0.1;
  _this->_camera->setNear(newNear);
}

void CameraConfiguration::_nearGet(void*A,void*D){
  CameraConfiguration*_this=(CameraConfiguration*)D;
  *(float*)A=_this->_camera->getNear();
}

void CameraConfiguration::_farSet(const void*A,void*D){
  CameraConfiguration*_this=(CameraConfiguration*)D;
  float newFar=*(float*)A;
  if(newFar<1)newFar=1;
  if(newFar>FAR_LIMIT)newFar=std::numeric_limits<float>::infinity();
  _this->_camera->setFar(newFar);
}

void CameraConfiguration::_farGet(void*A,void*D){
  CameraConfiguration*_this=(CameraConfiguration*)D;
  *(float*)A=_this->_camera->getFar();
}

bool CameraConfiguration::isMouseLock(){
  return this->_mouseLock;
}

void CameraConfiguration::_save(std::string name,void*A){
  CameraConfiguration*_this=(CameraConfiguration*)A;
  _this->_camera->saveToFile(name);
}

void CameraConfiguration::_load(std::string name,void*A){
  CameraConfiguration*_this=(CameraConfiguration*)A;
  _this->_camera->loadFromFile(name);
}

CameraConfiguration::~CameraConfiguration(){
  delete this->_openDialog;
  TwDeleteBar(this->_bar);
  delete this->_camera;
}

ge::util::CameraObject*CameraConfiguration::getCamera(){
  return this->_camera;
}

