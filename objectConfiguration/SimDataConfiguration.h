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
        //bool changed=*((T*)value)!=*((T*)cb->_data);
        bool changed=false;
        if(typeid(T)==typeid(float)){
          if(cb->_float!=*((T*)cb->_data))changed=true;
          cb->_float=*((T*)cb->_data);
        }
        if(typeid(T)==typeid(unsigned)){
          if(cb->_uint!=*((T*)cb->_data))changed=true;
          cb->_uint=*((T*)cb->_data);
        }

        if(typeid(T)==typeid(int)){
          if(cb->_int!=*((T*)cb->_data))changed=true;
          cb->_int=*((T*)cb->_data);
        }
        if(typeid(T)==typeid(bool)){
          if(cb->_bool!=*((T*)cb->_data))changed=true;
          cb->_bool=*((T*)cb->_data);
        }

        //std::cerr<<cb->_name<<": "<<*((T*)value)<<" new: "<<*((T*)cb->_data)<<std::endl;
        *((T*)value)=*((T*)cb->_data);
        if(changed)cb->_this->_sd->setAsChanged(cb->_name);
      }
      class CallBackData{
        public:
          ShadowMethodConfig*_this;
          std::string        _name;
          void*              _data;
          union{
            float    _float;
            int      _int;
            unsigned _uint;
            bool     _bool;
          };
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
