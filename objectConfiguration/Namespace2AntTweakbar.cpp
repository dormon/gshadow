#include"Namespace2AntTweakbar.h"

NamespaceManipulator::NamespaceManipulator(std::shared_ptr<ge::util::sim::NamespaceWithUsers> const&ns){
  this->_bar = TwNewBar("*");
  this->_ns = ns;
  for(auto x=this->_ns->varsBegin();x!=this->_ns->varsEnd();++x){
    auto type=x->second->getManager()->getTypeIdType(x->second->getId());
#define CASE(TYPE,ENUM)\
    this->_callbackData.push_back(new _CallBackData(x->first,ns));\
    TwAddVarCB(this->_bar,x->first.c_str(),ENUM,\
        NamespaceManipulator::_get<TYPE>,\
        NamespaceManipulator::_set<TYPE>,\
        (void*)this->_callbackData[this->_callbackData.size()-1]," group=asd");\
        break
    switch(type){
      case ge::core::TypeRegister::BOOL:
        CASE(bool,TW_TYPE_BOOLCPP);
      case ge::core::TypeRegister::I8:
        CASE(char,TW_TYPE_INT8);
      case ge::core::TypeRegister::I16:
        CASE(short,TW_TYPE_INT16);
      case ge::core::TypeRegister::I32:
        CASE(int,TW_TYPE_INT32);
      case ge::core::TypeRegister::I64:
        break;
      case ge::core::TypeRegister::U8:
        CASE(unsigned char,TW_TYPE_UINT8);
      case ge::core::TypeRegister::U16:
        CASE(unsigned short,TW_TYPE_UINT16);
      case ge::core::TypeRegister::U32:
        CASE(unsigned,TW_TYPE_UINT32);
      case ge::core::TypeRegister::U64:
        break;
      case ge::core::TypeRegister::F32:
        CASE(unsigned,TW_TYPE_FLOAT);
      case ge::core::TypeRegister::F64:
        CASE(unsigned,TW_TYPE_DOUBLE);
      case ge::core::TypeRegister::STRING:
        break;
      case ge::core::TypeRegister::ARRAY:
        break;
      case ge::core::TypeRegister::STRUCT:
        break;
      default:
        break;
    }
  }
}

NamespaceManipulator::~NamespaceManipulator(){
  for(auto x:this->_callbackData)
    delete x;
}
