#include"SimDataConfiguration.h"

using namespace objconf;

ShadowMethodConfig::ShadowMethodConfig(std::string name,simulation::SimulationObject*obj,simulation::SimulationData*sd){
  this->_bar = TwNewBar(name.c_str());
  
  for(unsigned i=0;i<obj->getNofDependentVariables();++i){
    std::string varName=obj->getDependentVariable(i);
    simulation::Data*data=sd->get(varName,NULL);
    switch(data->type){
      case simulation::BOOL:
        TwAddVarRW(this->_bar,varName.c_str(),TW_TYPE_BOOLCPP,&((simulation::Bool*)data)->data," ");
        break;
      case simulation::FLOAT:
        TwAddVarRW(this->_bar,varName.c_str(),TW_TYPE_FLOAT,&((simulation::Float*)data)->data," step=0.1");
        break;
      case simulation::UINT:
        TwAddVarRW(this->_bar,varName.c_str(),TW_TYPE_UINT32,&((simulation::Uint*)data)->data," ");
        break;


      default:
        break;
    }
  }
}

ShadowMethodConfig::~ShadowMethodConfig(){
  TwDeleteBar(this->_bar);
}
