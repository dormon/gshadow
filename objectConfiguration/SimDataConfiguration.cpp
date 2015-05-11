#include"SimDataConfiguration.h"

using namespace objconf;


ShadowMethodConfig::ShadowMethodConfig(std::string name,simulation::SimulationObject*obj,simulation::SimulationData*sd){
  this->_sd = sd;
  this->_bar = TwNewBar(name.c_str());
  for(unsigned i=0;i<obj->getNofDependentVariables();++i){
    std::string varName=obj->getDependentVariable(i);
    simulation::Data*data=sd->get(varName,NULL);
    switch(data->type){
      case simulation::BOOL:
        this->_callbackData.push_back(new CallBackData(this,varName,&((simulation::Bool*)data)->data));
        TwAddVarCB(this->_bar,(varName).c_str(),TW_TYPE_BOOLCPP,
            ShadowMethodConfig::_get<bool>,
            ShadowMethodConfig::_set<bool>,
            this->_callbackData[this->_callbackData.size()-1]," ");

        //TwAddVarRW(this->_bar,varName.c_str(),TW_TYPE_BOOLCPP,&((simulation::Bool*)data)->data," ");
        break;
      case simulation::FLOAT:
        this->_callbackData.push_back(new CallBackData(this,varName,&((simulation::Float*)data)->data));
        TwAddVarCB(this->_bar,(varName).c_str(),TW_TYPE_FLOAT,
            ShadowMethodConfig::_get<float>,
            ShadowMethodConfig::_set<float>,
            this->_callbackData[this->_callbackData.size()-1]," step=0.1");

        //TwAddVarRW(this->_bar,varName.c_str(),TW_TYPE_FLOAT,&((simulation::Float*)data)->data," step=0.1");
        break;
      case simulation::UINT:
        this->_callbackData.push_back(new CallBackData(this,varName,&((simulation::Uint*)data)->data));
        TwAddVarCB(this->_bar,(varName).c_str(),TW_TYPE_UINT32,
            ShadowMethodConfig::_get<unsigned>,
            ShadowMethodConfig::_set<unsigned>,
            this->_callbackData[this->_callbackData.size()-1]," ");
        //TwAddVarRW(this->_bar,varName.c_str(),TW_TYPE_UINT32,&((simulation::Uint*)data)->data," ");
        break;
      case simulation::VEC3:
        this->_callbackData.push_back(new CallBackData(this,varName,&((simulation::Vec3*)data)->data.x));
        TwAddVarCB(this->_bar,(varName+".x").c_str(),TW_TYPE_FLOAT,
            ShadowMethodConfig::_get<float>,
            ShadowMethodConfig::_set<float>,
            this->_callbackData[this->_callbackData.size()-1]," step=0.1");

        this->_callbackData.push_back(new CallBackData(this,varName,&((simulation::Vec3*)data)->data.y));
        TwAddVarCB(this->_bar,(varName+".y").c_str(),TW_TYPE_FLOAT,
            ShadowMethodConfig::_get<float>,
            ShadowMethodConfig::_set<float>,
            this->_callbackData[this->_callbackData.size()-1]," step=0.1");

        this->_callbackData.push_back(new CallBackData(this,varName,&((simulation::Vec3*)data)->data.z));
        TwAddVarCB(this->_bar,(varName+".z").c_str(),TW_TYPE_FLOAT,
            ShadowMethodConfig::_get<float>,
            ShadowMethodConfig::_set<float>,
            this->_callbackData[this->_callbackData.size()-1]," step=0.1");
        break;
      default:
        break;
    }
  }
}

ShadowMethodConfig::~ShadowMethodConfig(){
  TwDeleteBar(this->_bar);
  for(unsigned i=0;i<this->_callbackData.size();++i)
    delete this->_callbackData[i];
  this->_callbackData.clear();
}
