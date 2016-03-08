#pragma once

#include<AntTweakBar.h>
#include<geUtil/NamespaceWithUsers.h>

class NamespaceManipulator{
  protected:
    TwBar* _bar = nullptr;
    std::shared_ptr<ge::util::sim::NamespaceWithUsers> _ns = nullptr;
    class _CallBackData{
      public:
        std::string _name = "";
        std::shared_ptr<ge::util::sim::NamespaceWithUsers> _ns = nullptr;
        _CallBackData(std::string name = "",std::shared_ptr<ge::util::sim::NamespaceWithUsers> const& ns = nullptr){
          this->_name = name;
          this->_ns   = ns;
        }
    };
    std::vector<_CallBackData*>_callbackData;
    template<typename T>
      static void _get(const void*value,void*data){
        //std::cerr<<"get data: "<<data<<std::endl;
        //std::cerr<<"get value: "<<value<<std::endl;
        _CallBackData*cd=(_CallBackData*)data;
        //std::cerr<<"alue of "<<cd->_name<<": "<<*((T*)value)<<std::endl;
        if((T&)(*cd->_ns->getVariable(cd->_name))!=*((T*)value)){
          (T&)(*cd->_ns->getVariable(cd->_name))=*((T*)value);
          cd->_ns->setAsChanged(cd->_name);
        }
      }
    template<typename T>
      static void _set(void*value,void*data){
        //std::cerr<<"set value: "<<value<<std::endl;
        //std::cerr<<"set data: "<<data<<std::endl;
        _CallBackData*cd=(_CallBackData*)data;
        //std::cerr<<"set cd->_name: "<<cd->_name<<std::endl;
        *((T*)value)=(T)(*cd->_ns->getVariable(cd->_name));
      }
  public:
    NamespaceManipulator(std::shared_ptr<ge::util::sim::NamespaceWithUsers> const&ns);
    virtual ~NamespaceManipulator();
};
