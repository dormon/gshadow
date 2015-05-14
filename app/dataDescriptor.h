#pragma once

#include"core.h"
#include<iostream>
#include<vector>

namespace lang{
  class DataDescriptor{
    public:
      DEF_ENUM(Type,INT,UINT,FLOAT,STRING,ARRAY,STRUCT,PTR);
    protected:
      Type _type;
    public:
      DataDescriptor(Type type);
      DataDescriptor(){}
      virtual Type getType();
  };
  class IntDescriptor: public DataDescriptor{
    public:
      IntDescriptor();
  };
  class UintDescriptor: public DataDescriptor{
    public:
      UintDescriptor();
  };
  class FloatDescriptor: public DataDescriptor{
    public:
      FloatDescriptor();
  };
  class StringDescriptor: public DataDescriptor{
    public:
      StringDescriptor();
  };
  class ArrayDescriptor: public DataDescriptor{
    protected:
      DataDescriptor _innerType;
    public:
      ArrayDescriptor(DataDescriptor&innerType);
      DataDescriptor& getInnerType();
  };
  class StructDescriptor: public DataDescriptor{
    protected:
      std::vector<DataDescriptor>_elements;
    public:
      StructDescriptor();
      void addElement(DataDescriptor&descriptor);
      DataDescriptor&getElement(unsigned i);
      unsigned getNofElements();
  };
  class PtrDescriptor: public DataDescriptor{
    protected:
      DataDescriptor _innerType;
    public:
      PtrDescriptor(DataDescriptor&innerType);
      DataDescriptor&getInnerType();
  };
}
