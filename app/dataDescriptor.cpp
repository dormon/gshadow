#include"dataDescriptor.h"

using namespace lang;

DataDescriptor::DataDescriptor(Type type){ 
  this->_type = type;
}
DataDescriptor::DataDescriptor(){
  this->_type = Type::VOID;
}
DataDescriptor::Type DataDescriptor::getType(){
  return this->_type;
}
bool DataDescriptor::operator==(DataDescriptor&b){
  return this->_type==b._type;
}

IntDescriptor::IntDescriptor      (): DataDescriptor(Type::INT){}
bool IntDescriptor::operator==(DataDescriptor&b){return this->_type==b.getType();}
UintDescriptor::UintDescriptor    (): DataDescriptor(Type::UINT){}
bool UintDescriptor::operator==(DataDescriptor&b){return this->_type==b.getType();}
FloatDescriptor::FloatDescriptor  (): DataDescriptor(Type::FLOAT){}
bool FloatDescriptor::operator==(DataDescriptor&b){return this->_type==b.getType();}
StringDescriptor::StringDescriptor(): DataDescriptor(Type::STRING){}
bool StringDescriptor::operator==(DataDescriptor&b){return this->_type==b.getType();}

ArrayDescriptor::ArrayDescriptor(
    DataDescriptor&innerType,
    unsigned       size     ):DataDescriptor(Type::ARRAY){
  this->_innerType = innerType;
  this->_size      = size     ;
}
bool ArrayDescriptor::operator==(DataDescriptor&b){
  if(this->_type!=b.getType())return false;
  return this->_size==((ArrayDescriptor&)b)._size;
}
DataDescriptor&ArrayDescriptor::getInnerType(){
  return this->_innerType;
}
unsigned ArrayDescriptor::getSize(){
  return this->_size;
}
StructDescriptor::StructDescriptor(): DataDescriptor(Type::STRUCT){}
void StructDescriptor::addElement(DataDescriptor&descriptor){
  this->_elements.push_back(descriptor);
}
bool StructDescriptor::operator==(DataDescriptor&b){
  if(this->_type!=b.getType())return false;
  if(this->getNofElements()!=((StructDescriptor&)b).getNofElements())return false;
  for(unsigned i=0;i<this->getNofElements();++i)
    if(!(this->getElement(i)==((StructDescriptor&)b).getElement(i)))return false;
  return true;
}
DataDescriptor&StructDescriptor::getElement(unsigned i){
  return this->_elements[i];
}
unsigned StructDescriptor::getNofElements(){
  return this->_elements.size();
}
PtrDescriptor::PtrDescriptor(
    DataDescriptor&innerType): DataDescriptor(Type::PTR){
  this->_innerType = innerType;
}
bool PtrDescriptor::operator==(DataDescriptor&b){
  if(this->_type!=b.getType())return false;
  return this->getInnerType()==((PtrDescriptor&)b).getInnerType();
}
DataDescriptor&PtrDescriptor::getInnerType(){
  return this->_innerType;
}

Data::Data(unsigned type,TypeManager*manager){
  this->_type    = type   ;
  this->_manager = manager;
}

ArrayData::ArrayData(
    TypeManager*     manager,
    ArrayDescriptor& descriptor): Data(manager->addType(descriptor),manager){  
  this->_data = new Data*[descriptor.getSize()];
  for(unsigned i=0;i<descriptor.getSize();++i)
    this->_data[i] = this->_manager->allocate(descriptor.getInnerType());
}

TypeManager::TypeManager(){
  this->addType(IntDescriptor   ());
  this->addType(UintDescriptor  ());
  this->addType(FloatDescriptor ());
  this->addType(StringDescriptor());
}

unsigned TypeManager::addType(DataDescriptor descriptor){
  for(unsigned i=0;i<this->_types.size();++i)
    if(this->_types[i]==descriptor)return i;
  this->_types.push_back(descriptor);
  return this->_types.size()-1;
}

DataDescriptor&TypeManager::getType(unsigned type){
  return this->_types[type];
}

Data*TypeManager::allocate(DataDescriptor&descriptor){
  if(descriptor.getType()==DataDescriptor::INT)return new BaseData<int        >(this);
  if(descriptor.getType()==DataDescriptor::INT)return new BaseData<unsigned   >(this);
  if(descriptor.getType()==DataDescriptor::INT)return new BaseData<float      >(this);
  if(descriptor.getType()==DataDescriptor::INT)return new BaseData<std::string>(this);
  return NULL;
}
