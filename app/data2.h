#pragma once

#include"core.h"
#include<iostream>
#include<vector>
#include<map>
#include<typeinfo>

namespace lang2{
  class TypeManager{
    public:
      DEF_ENUM(Type,VOID,INT,UINT,FLOAT,STRING,ARRAY,STRUCT,PTR,FCE,TYPEID);
      typedef unsigned TypeID;
    protected:
      std::vector<unsigned   >_typeStart;
      std::vector<unsigned   >_types    ;
      std::vector<std::string>_typeNames;
      std::map<std::string,TypeID>_name2Id;
    public:
      TypeManager();
      ~TypeManager();
      unsigned getTypeDescriptionLength(TypeID id);
      unsigned getTypeDescriptionElem  (TypeID id,unsigned i);
      Type     getType                 (TypeID id);
      unsigned getNofStructElements    (TypeID id);
      TypeID   getStructElementTypeId  (TypeID id,unsigned element);
      unsigned getArraySize            (TypeID id);
      TypeID   getArrayInnerTypeId     (TypeID id);
      TypeID   getPtrInnerTypeId       (TypeID id);
      TypeID   getFceReturnTypeId      (TypeID id);
      unsigned getNofFceArgs           (TypeID id);
      TypeID   getFceArgTypeId         (TypeID id,unsigned element);



      template<typename... Args>
        unsigned addType(Args... args,std::string name=""){
          std::vector<unsigned>typeConfig;
          _argsToVector(typeConfig,args...);
          return this->addType(typeConfig,name);
        }
      unsigned addType(std::vector<unsigned>&type,std::string name="");
      //unsigned getType(std::string name);
  };
}
