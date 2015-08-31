#include"cameraPathConfiguration.h"

#include<sstream>
#include"AntOpenDialog.h"

using namespace objconf;


struct{
  ge::util::CameraPath*path;
  std::string file;
  TwBar*bar;
  AntOpenDialog*antDirectory;
}cameraPathConfiguration;

void CameraPathConfiguration::_selectSet(const void*A,void*D){
  CameraPathConfiguration*_this=(CameraPathConfiguration*)D;
  if(!_this->_path)return;
  _this->_selected=*(unsigned*)A;
  if(_this->_selected>=_this->_path->getLength()){
    if(_this->_path->getLength()==0)_this->_selected=0;
    else _this->_selected=_this->_path->getLength()-1;
  }
  _this->_path->select(_this->_selected);
}
void CameraPathConfiguration::_selectGet(void*A,void*D){
  CameraPathConfiguration*_this=(CameraPathConfiguration*)D;
  *(unsigned*)A=_this->_selected;
}
void CameraPathConfiguration::_push(void*A){
  CameraPathConfiguration*_this=(CameraPathConfiguration*)A;
  if(!_this->_path)return;
  _this->_path->insertToEnd(new ge::util::CameraKeyPoint(_this->_camera));
}
void CameraPathConfiguration::_remove(void*A){
  CameraPathConfiguration*_this=(CameraPathConfiguration*)A;
  if(!_this->_path)return;
  _this->_path->deletePoint(_this->_selected);
}


CameraPathConfiguration::CameraPathConfiguration(){
  this->_draw   = false;
  this->_path   = NULL;
  this->_camera = NULL;
  this->_selected = 0;
  this->_bar=TwNewBar("cameraPath");
  TwAddVarRW(this->_bar,"draw",TW_TYPE_BOOLCPP,&this->_draw,"label='draw'");
  TwAddButton(this->_bar,"new",this->_new,this,"label='new'");
  TwAddVarCB(this->_bar,"selected",TW_TYPE_UINT32,this->_selectSet,this->_selectGet,this,"label='selected'");
  TwAddButton(this->_bar,"push",this->_push,this,"label='push'");
  TwAddButton(this->_bar,"remove",this->_remove,this,"label='remove'");
  this->_openDialog = new AntOpenDialog(this->_bar,"path_configurator");
  this->_openDialog->setOpenCallBack(this->_load,this);
  this->_openDialog->setSaveCallBack(this->_save,this);
}

CameraPathConfiguration::~CameraPathConfiguration(){
  delete this->_openDialog;
  TwDeleteBar(this->_bar);
  this->_deletePath();
}

ge::util::CameraPath*CameraPathConfiguration::getPath(){
  return this->_path;
}

void CameraPathConfiguration::setCamera(ge::util::CameraObject*camera){
  this->_camera = camera;
}

void CameraPathConfiguration::_deletePath(){
  if(this->_path)delete this->_path;
  this->_path=NULL;
}

void CameraPathConfiguration::_load(std::string name,void*A){
  CameraPathConfiguration*_this=(CameraPathConfiguration*)A;
  _this->_deletePath();
  _this->_path=new ge::util::CameraPath();
  _this->_path->loadCSV(name);
}

void CameraPathConfiguration::_save(std::string name,void*A){
  CameraPathConfiguration*_this=(CameraPathConfiguration*)A;
  if(!_this->_path)return;
  _this->_path->saveCSV(name);
}

void CameraPathConfiguration::_new(void*A){
  CameraPathConfiguration*_this=(CameraPathConfiguration*)A;
  _this->_deletePath();
  _this->_path = new ge::util::CameraPath();
}


void TW_CALL cpyStdStringToClient(
    std::string&destinationClientString, 
    const std::string&sourceLibraryString){
  destinationClientString = sourceLibraryString;
}

void objconf::setCameraPathAntTweakBar(){
  cameraPathConfiguration.bar=TwNewBar("cameraPath");
  TwCopyStdStringToClientFunc(cpyStdStringToClient);






  cameraPathConfiguration.antDirectory=new AntOpenDialog(cameraPathConfiguration.bar,"cam_path_config");

  /*
     TwAddVarRW(bar,"CameraMationFile",TW_TYPE_STDSTRING,&cameraPathConfiguration.file, 
     " label='current file' group=StdString help='file' ");
     TwAddVarRW(bar,"current dir",TW_TYPE_STDSTRING,&cameraPathConfiguration.currentDir, 
     " label='dir' group=StdString help='dir' ");
     loadDirContent(cameraPathConfiguration.currentDir);
     */
}
