#include"dataDescriptor.h"

using namespace lang;

DataDescriptor::DataDescriptor(Type type){
  this->_type = type;
}
DataDescriptor::Type DataDescriptor::getType(){
  return this->_type;
}
IntDescriptor::IntDescriptor      (): DataDescriptor(Type::INT   ){}
UintDescriptor::UintDescriptor    (): DataDescriptor(Type::UINT  ){}
FloatDescriptor::FloatDescriptor  (): DataDescriptor(Type::FLOAT ){}
StringDescriptor::StringDescriptor(): DataDescriptor(Type::STRING){}

ArrayDescriptor::ArrayDescriptor(DataDescriptor&innerType): DataDescriptor(Type::ARRAY){
  this->_innerType = innerType;
}
DataDescriptor&ArrayDescriptor::getInnerType(){
  return this->_innerType;
}
StructDescriptor::StructDescriptor(): DataDescriptor(Type::STRUCT){}
void StructDescriptor::addElement(DataDescriptor&descriptor){
  this->_elements.push_back(descriptor);
}
DataDescriptor&StructDescriptor::getElement(unsigned i){
  return this->_elements[i];
}
unsigned StructDescriptor::getNofElements(){
  return this->_elements.size();
}
PtrDescriptor::PtrDescriptor(DataDescriptor&innerType): DataDescriptor(Type::PTR){
  this->_innerType = innerType;
}
DataDescriptor&PtrDescriptor::getInnerType(){
  return this->_innerType;
}

