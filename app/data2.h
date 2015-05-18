#pragma once

#include"core.h"
#include<iostream>
#include<vector>
#include<map>
#include<typeinfo>

namespace lang2{
  class TypeManager{
    public:
      DEF_ENUM(Type,VOID,PTR,INT,UINT,FLOAT,STRING,ARRAY,STRUCT,TYPEID);
    protected:
      std::vector<unsigned   >_typeStart;
      std::vector<unsigned   >_types    ;
      std::vector<std::string>_typeNames;
      std::map<std::string,unsigned>_name2Id;
    public:
      TypeManager();
      ~TypeManager();
      unsigned getTypeDescriptionLength(unsigned typeId);
      unsigned getTypeDescriptionElem  (unsigned typeId,unsigned i);
      Type     getType                 (unsigned typeId);
      unsigned getTypeId               (unsigned typeId);
      bool     isTypeId                (unsigned typeId);
      unsigned getNofStructElements    (unsigned typeId);
      unsigned getArraySize            (unsigned typeId);
      unsigned getArrayInnerTypeId     (unsigned typeId);
      unsigned getStructElementTypeId  (unsigned typeId,unsigned element);

      Type     getArrayInnerType       (unsigned typeId);
      Type     getStructElementType    (unsigned typeId,unsigned element);



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
