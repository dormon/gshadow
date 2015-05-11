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
      simulation::SimulationData*_sd;
      template<typename T>
      static void _get(const void*value,void*data){
        ShadowMethodConfig::CallBackData*cb=(ShadowMethodConfig::CallBackData*)data;
        *((T*)cb->_data)=*((T*)value);
      }
      template<typename T>
      static void _set(void*value,void*data){
        ShadowMethodConfig::CallBackData*cb=(ShadowMethodConfig::CallBackData*)data;
        *((T*)value)=*((T*)cb->_data);
        cb->_this->_sd->setAsChanged(cb->_name);
      }
      class CallBackData{
        public:
          ShadowMethodConfig*_this;
          std::string        _name;
          void*              _data;
          CallBackData(ShadowMethodConfig*_this,std::string name,void*data){
            this->_this=_this;
            this->_name=name;
            this->_data=data;
          }
      };
      std::vector<CallBackData*>_callbackData;
    public:
      ShadowMethodConfig(std::string name,simulation::SimulationObject*sm,simulation::SimulationData*sd);
      ~ShadowMethodConfig();
  };
}
