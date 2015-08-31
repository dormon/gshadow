#include"lightConfiguration.h"

#include<sstream>

using namespace objconf;

//simulation::Light light(glm::vec4(10.f,10.f,100.f,1.f),glm::vec3(1.f),glm::vec3(1.f));

void objconf::setLight(ge::util::ArgumentObject*args){
  lightConfiguration.currentLight=0;
  lightConfiguration.lights.push_back(
      new simulation::Light(glm::vec4(10.f,10.f,100.f,1.f),glm::vec3(1.f),glm::vec3(1.f)));

  lightConfiguration.lights[lightConfiguration.currentLight]->position[0] = args->getArgf("--light-start","--light-end",0,"10");
  lightConfiguration.lights[lightConfiguration.currentLight]->position[1] = args->getArgf("--light-start","--light-end",1,"10");
  lightConfiguration.lights[lightConfiguration.currentLight]->position[2] = args->getArgf("--light-start","--light-end",2,"10");
  lightConfiguration.lights[lightConfiguration.currentLight]->position[3] = args->getArgf("--light-start","--light-end",3,"1");
  lightConfiguration.lights[lightConfiguration.currentLight]->diffuse [0] = args->getArgf("--light-start","--light-end",4,"1");
  lightConfiguration.lights[lightConfiguration.currentLight]->diffuse [1] = args->getArgf("--light-start","--light-end",5,"1");
  lightConfiguration.lights[lightConfiguration.currentLight]->diffuse [2] = args->getArgf("--light-start","--light-end",6,"1");
  lightConfiguration.lights[lightConfiguration.currentLight]->specular[0] = args->getArgf("--light-start","--light-end",7,"1");
  lightConfiguration.lights[lightConfiguration.currentLight]->specular[1] = args->getArgf("--light-start","--light-end",8,"1");
  lightConfiguration.lights[lightConfiguration.currentLight]->specular[2] = args->getArgf("--light-start","--light-end",9,"1");
}

void objconf::deinitLight(){
  for(unsigned i=0;i<lightConfiguration.lights.size();++i)
    delete lightConfiguration.lights[i];
}


/*
simulation::Light*objconf::getLight(unsigned i){
  if(i>=lightConfiguration.lights.size())return lightConfiguration.lights[0];
  return lightConfiguration.lights[i];
}
*/
void createLightBarVar();

void TW_CALL setCurrentAsActive(void *clientData){
  TwRemoveAllVars(lightConfiguration.lbar); 
  createLightBarVar();
}

void createLightBarVar(){
  TwAddButton(lightConfiguration.lbar,"setCurrentAsActive",setCurrentAsActive,NULL,"label='set current as active'");

  std::stringstream ss;
  ss<<" label='current light' min=0 max="<<lightConfiguration.lights.size()-1<<" step=1";
  TwAddVarRW(lightConfiguration.lbar,"current",TW_TYPE_UINT32,&lightConfiguration.currentLight,ss.str().c_str());

  float*pos  = glm::value_ptr(lightConfiguration.lights[lightConfiguration.currentLight]->position);
  float*dif  = glm::value_ptr(lightConfiguration.lights[lightConfiguration.currentLight]->diffuse );
  float*spec = glm::value_ptr(lightConfiguration.lights[lightConfiguration.currentLight]->specular);

  TwAddVarRW(lightConfiguration.lbar,"xpos"    ,TW_TYPE_FLOAT  ,pos+0," label='xpos' help='xpos of current light'");
  TwAddVarRW(lightConfiguration.lbar,"ypos"    ,TW_TYPE_FLOAT  ,pos+1," label='ypos' help='ypos of current light'");
  TwAddVarRW(lightConfiguration.lbar,"zpos"    ,TW_TYPE_FLOAT  ,pos+2," label='zpos' help='zpos of current light'");
  TwAddVarRW(lightConfiguration.lbar,"diffuse" ,TW_TYPE_COLOR3F,dif  ," noalpha help='Light diffuse color.' "     );
  TwAddVarRW(lightConfiguration.lbar,"speculra",TW_TYPE_COLOR3F,spec ," noalpha help='Light specular color.' "    );
}

void objconf::setLightAntTweakBar(){
  lightConfiguration.lbar=TwNewBar("lights");


  createLightBarVar();
}

LightConfiguration::LightConfiguration(){
  this->_bar = TwNewBar("light configurator");

  this->_light = new simulation::Light();
  float*pos  = glm::value_ptr(this->_light->position);
  float*dif  = glm::value_ptr(this->_light->diffuse );
  float*spec = glm::value_ptr(this->_light->specular);

  TwAddVarRW(this->_bar,"xpos"    ,TW_TYPE_FLOAT  ,pos+0," label='xpos' help='xpos of current light'");
  TwAddVarRW(this->_bar,"ypos"    ,TW_TYPE_FLOAT  ,pos+1," label='ypos' help='ypos of current light'");
  TwAddVarRW(this->_bar,"zpos"    ,TW_TYPE_FLOAT  ,pos+2," label='zpos' help='zpos of current light'");
  TwAddVarRW(this->_bar,"diffuse" ,TW_TYPE_COLOR3F,dif  ," noalpha help='Light diffuse color.' "     );
  TwAddVarRW(this->_bar,"speculra",TW_TYPE_COLOR3F,spec ," noalpha help='Light specular color.' "    );
  this->_openDialog = new AntOpenDialog(this->_bar,"light_configurator");
  this->_openDialog->setOpenCallBack(this->_load,this);
  this->_openDialog->setSaveCallBack(this->_save,this);

}
void LightConfiguration::_load(std::string name,void*A){
  std::cout<<"LightConfiguration:_load"<<std::endl;
  LightConfiguration*_this=(LightConfiguration*)A;
  _this->_light->loadFromFile(name);

}
void LightConfiguration::_save(std::string name,void*A){
  LightConfiguration*_this=(LightConfiguration*)A;
  _this->_light->saveToFile(name);
}


LightConfiguration::~LightConfiguration(){
  delete this->_openDialog;
  TwDeleteBar(this->_bar);
}

