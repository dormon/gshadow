#include"data2.h"

using namespace lang2;

TypeManager::TypeDescriptor::TypeDescriptor(
    std::vector<unsigned>*data   ,
    Type                  type   ,
    unsigned              start  ,
    TypeManager*          manager){
  this->data    = data   ;
  this->type    = type   ;
  this->start   = start  ;
  this->manager = manager;
}

bool TypeManager::TypeDescriptor::operator==(TypeManager::TypeDescriptor&b){
  if(this->type != b.type)return false;//TypeID
  
}

TypeManager::TypeManager(){
  
}

//0 VOID
//1 INT
//2 UINT
//3 FLOAT
//4 STRING
//5 Size T ARRAY
//6 Size T T T ... STRUCT
//7 T PTR
//
//
//
//
unsigned TypeManager::getTypeDescriptionLength(unsigned typeId){
  unsigned a=this->_typeStart[typeId];
  if(this->_typeStart.size()-1==typeId)return this->_types.size()-a;
  return this->_typeStart[typeId+1]-a;
}
unsigned TypeManager::getTypeDescriptionElem  (unsigned typeId,unsigned i){
  return this->_types[this->_typeStart[typeId]+i];
}

bool TypeManager::isTypeId(unsigned typeId){
  return typeId>=TYPEID;
}

/*
Type TypeManager::getType(unsigned typeId){
  return this->types[this->_typeStart[typeId]];
}*/

unsigned TypeManager::getNofStructElements(unsigned typeId){
  return this->_types[this->_typeStart[typeId]+1];
}

unsigned TypeManager::getArraySize(unsigned typeId){
  return this->_types[this->_typeStart[typeId]+1];
}

/*
unsigned TypeManager::getStructElementType(unsigned typeId,unsigned element){

}*/

unsigned TypeManager::addType(std::vector<unsigned>&type,std::string name){
  for(unsigned t=0;t<this->_typeStart.size();++t){
    this->_types[this->_typeStart[t]];
  }
  return 0;
}


