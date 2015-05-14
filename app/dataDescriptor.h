#pragma once

#include"core.h"
#include<iostream>
#include<vector>
#include<typeinfo>

namespace lang{
  class DataDescriptor{
    public:
      DEF_ENUM(Type,VOID,INT,UINT,FLOAT,STRING,ARRAY,STRUCT,PTR);
    protected:
      Type _type;
    public:
      DataDescriptor(Type type);
      DataDescriptor();
      virtual Type getType();
      virtual bool operator==(DataDescriptor&b);
  };
  class IntDescriptor: public DataDescriptor{
    public:
      IntDescriptor();
      virtual bool operator==(DataDescriptor&b);
  };
  class UintDescriptor: public DataDescriptor{
    public:
      UintDescriptor();
      virtual bool operator==(DataDescriptor&b);
  };
  class FloatDescriptor: public DataDescriptor{
    public:
      FloatDescriptor();
      virtual bool operator==(DataDescriptor&b);
  };
  class StringDescriptor: public DataDescriptor{
    public:
      StringDescriptor();
      virtual bool operator==(DataDescriptor&b);
  };
  class ArrayDescriptor: public DataDescriptor{
    protected:
      DataDescriptor _innerType;
      unsigned       _size;
    public:
      ArrayDescriptor(DataDescriptor&innerType,unsigned size);
      DataDescriptor& getInnerType();
      unsigned        getSize();
      virtual bool operator==(DataDescriptor&b);
  };
  class StructDescriptor: public DataDescriptor{
    protected:
      std::vector<DataDescriptor>_elements;
    public:
      StructDescriptor();
      void addElement(DataDescriptor&descriptor);
      DataDescriptor&getElement(unsigned i);
      unsigned getNofElements();
      virtual bool operator==(DataDescriptor&b);
  };
  class PtrDescriptor: public DataDescriptor{
    protected:
      DataDescriptor _innerType;
    public:
      PtrDescriptor(DataDescriptor&innerType);
      DataDescriptor&getInnerType();
      virtual bool operator==(DataDescriptor&b);
  };
  class TypeManager;
  class Data{
    protected:
      unsigned    _type;
      TypeManager*_manager;
    public:
      Data(unsigned type,TypeManager*manager);
  };
  template<typename T>
  class BaseData: public Data{
    protected:
      T _data;
    public:
      BaseData(TypeManager*manager,T data=0):
        Data(0,manager){
        this->_manager=manager;
        if(typeid(T)==typeid(int        ))this->_type = this->_manager->addType(IntDescriptor   ());
        if(typeid(T)==typeid(unsigned   ))this->_type = this->_manager->addType(UintDescriptor  ());
        if(typeid(T)==typeid(float      ))this->_type = this->_manager->addType(FloatDescriptor ());
        if(typeid(T)==typeid(std::string))this->_type = this->_manager->addType(StringDescriptor());
        this->_data=data;
      }
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
      ArrayData(TypeManager*manager,ArrayDescriptor&descriptor);
  };

  class TypeManager{
    protected:
      std::vector<DataDescriptor>_types;
    public:
      TypeManager();
      unsigned       addType(DataDescriptor descriptor);
      DataDescriptor&getType(unsigned type);
      Data*          allocate(DataDescriptor&descriptor);
  };
}
