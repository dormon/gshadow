#include"cameraConfiguration.h"

#include<sstream>

using namespace objconf;

struct{
  ge::util::CameraObject*camera;
  struct CameraParam{
    float near;
    float far;
    float fovy;
  }param;
  TwBar*bar;
}cameraConfiguration;


void objconf::setCamera(ge::util::ArgumentObject*args,unsigned*windowSize){
  cameraConfiguration.param.near = args->getArgf("-near","1");
  cameraConfiguration.param.far  = args->getArgf("-far" ,"9999");
  cameraConfiguration.param.fovy = args->getArgf("-fovy","45" );
  cameraConfiguration.camera = new ge::util::CameraObject(
      windowSize,
      cameraConfiguration.param.near,
      cameraConfiguration.param.far,
      cameraConfiguration.param.fovy);
  cameraConfiguration.camera->down(15);
}
void objconf::deinitCamera(){
  delete cameraConfiguration.camera;
}

ge::util::CameraObject*objconf::getCamera(unsigned){
  return cameraConfiguration.camera;
}

void TW_CALL setInfinity(void *clientData){
  cameraConfiguration.param.far=std::numeric_limits<float>::infinity();
}

void TW_CALL set(void *clientData){
  cameraConfiguration.camera->setNearFar(cameraConfiguration.param.near,cameraConfiguration.param.far);
  cameraConfiguration.camera->setFovy(cameraConfiguration.param.fovy);
}


void objconf::setCameraAntTweakBar(){
  cameraConfiguration.bar=TwNewBar("camera");
  TwAddButton(cameraConfiguration.bar,"setInfinity",setInfinity,NULL,"label='set infinity'");
  TwAddButton(cameraConfiguration.bar,"set",set,NULL,"label='set camera'");

  TwAddVarRW(cameraConfiguration.bar,"near",TW_TYPE_FLOAT,&cameraConfiguration.param.near," label='near' help='near plane' min='0.1' max='1000' step=0.1");
  TwAddVarRW(cameraConfiguration.bar,"far" ,TW_TYPE_FLOAT,&cameraConfiguration.param.far," label='far' help='far plane' min='10' max='10000' step=1"    );
  TwAddVarRW(cameraConfiguration.bar,"fovy",TW_TYPE_FLOAT,&cameraConfiguration.param.fovy," label='fovy' help='far plane' min='10' max='10000' step=1"    );

}

