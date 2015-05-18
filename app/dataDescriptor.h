#pragma once

#include"core.h"
#include<iostream>
#include<vector>
#include<map>
#include<typeinfo>

namespace lang{
  class ExpressionDescriptor{
    public:
      DEF_ENUM(Type,ADD,SUB,MUL,DIV,MOD);
  };
  class DataT{
    public:
      DEF_ENUM(Type,VOID,INT,UINT,FLOAT,STRING,ARRAY,STRUCT,PTR,FCE);
    protected:
      Type _type;
    public:
      DataT(Type type);
      DataT();
      virtual Type getType();
      virtual bool operator==(DataT&b);
  };
  template <DataT::Type type>
  class BaseT: public DataT{
    public:
      BaseT(): DataT(type){}
  };
  class ArrayT: public DataT{
    protected:
      DataT _innerType;
      unsigned       _size;
    public:
      ArrayT(DataT&innerType,unsigned size);
      ArrayT(DataT innerType,unsigned size);
      DataT& getInnerType();
      unsigned        getSize();
      virtual bool operator==(DataT&b);
  };
  class StructT: public DataT{
    protected:
      std::vector<DataT>_elements;
    public:
      StructT();
      void addElement(DataT&descriptor);
      DataT&getElement(unsigned i);
      unsigned getNofElements();
      virtual bool operator==(DataT&b);
  };
  class PtrT: public DataT{
    protected:
      DataT _innerType;
    public:
      PtrT(DataT&innerType);
      PtrT(DataT innerType);
      DataT&getInnerType();
      virtual bool operator==(DataT&b);
  };
  class FunctionDescriptor: public DataT{
    protected:
      DataT             _returnType    ;
      std::vector<DataT>_parametersType;
    public:
      FunctionDescriptor(DataT&returnType);
      void addParameter(DataT&param);
      DataT&getParameterType(unsigned i);
      unsigned getNofParameters();
      DataT&getReturnType();
      virtual bool operator==(DataT&b);
  };
  class TypeManager;
  class PtrData;
  class Data{
    protected:
      unsigned    _type;
      TypeManager*_manager;
    public:
      Data(unsigned type,TypeManager*manager);
      virtual ~Data();
      Data*operator&();
  };
  template<typename T>
  class BaseData: public Data{
    protected:
      T _data;
    public:
      BaseData(TypeManager*manager,T data=0):Data(0,manager){
        this->_manager=manager;
        if(typeid(T)==typeid(int        ))this->_type = this->_manager->addType(BaseT<DataT::INT   >());
        if(typeid(T)==typeid(unsigned   ))this->_type = this->_manager->addType(BaseT<DataT::UINT  >());
        if(typeid(T)==typeid(float      ))this->_type = this->_manager->addType(BaseT<DataT::FLOAT >());
        if(typeid(T)==typeid(std::string))this->_type = this->_manager->addType(BaseT<DataT::STRING>());
        this->_data=data;
      }
      virtual ~BaseData(){}
      T getData(){
        return this->_data;
      }
      T*getPtr(){
        return &this->_data;
      }
  };
  class ArrayData: public Data{
    protected:
      Data**_data;
    public:
      ArrayData(TypeManager*manager,ArrayT&descriptor);
      virtual ~ArrayData();
      Data*operator[](unsigned i);
  };
  class StructData: public Data{
    protected:
      Data**_data;
    public:
      StructData(TypeManager*manager,StructT&descriptor);
      virtual ~StructData();
      Data*operator[](unsigned i);
  };
  class PtrData: public Data{
    protected:
      Data*_data;
    public:
      PtrData(TypeManager*manager,PtrT&descriptor,Data*ptr=NULL);
      virtual ~PtrData();
      Data*operator*();
  };
  class Function: public Data{
    protected:
      PtrData*_return;
      PtrData*_parameters;
    public:
      Function(unsigned type,TypeManager*manager);
      virtual void apply()=0;
  };

  template<typename T>
  class Add: Function{
    protected:
      PtrData*_a;
      PtrData*_b;
      PtrData*_o;
    public:
      Add(PtrData*o,PtrData*a,PtrData*b){
        this->_a=a;
        this->_b=b;
        this->_o=o;
      }
      virtual void apply();
  };

  class TypeManager{
    protected:
      std::vector<DataT>_types   ;
      std::vector<std::string   >_typeName;
      std::map<std::string,unsigned>_name2Id;
    public:
      TypeManager();
      ~TypeManager();
      unsigned    addType(DataT descriptor,std::string name="");
      std::string getName(unsigned id);
      DataT&      getType(unsigned id);
      DataT&      getType(std::string name);
      unsigned    getId(std::string name);
      unsigned    getId(DataT&descriptor);
      Data*       allocate(DataT&descriptor);
      Data*       allocate(unsigned id);
      Data*       allocate(std::string name);
  };
}
