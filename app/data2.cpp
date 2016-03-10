#include"data2.h"

using namespace lang2;
/*
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
//8 T Size T .... FCE
//
//
//
//
unsigned TypeManager::getTypeDescriptionLength(TypeID id){
  unsigned a=this->_typeStart[id];
  if(this->_typeStart.size()-1==id)return this->_types.size()-a;
  return this->_typeStart[id+1]-a;
}
unsigned TypeManager::getTypeDescriptionElem  (TypeID id,unsigned i){
  return this->_types[this->_typeStart[id]+i];
}

TypeManager::Type TypeManager::getType(TypeID id){
  unsigned t=this->getTypeDescriptionElem(id,0);
  if(t>=TYPEID)return TYPEID;
  return (TypeManager::Type)t;
}

unsigned TypeManager::getNofStructElements(TypeID id){
  return this->getTypeDescriptionElem(id,1);
}

TypeManager::TypeID TypeManager::getStructElementTypeId(TypeID id,unsigned element){
  return this->getTypeDescriptionElem(id,2+element);
}

unsigned TypeManager::getArraySize(TypeID id){
  return this->getTypeDescriptionElem(id,1);
}

TypeManager::TypeID TypeManager::getArrayInnerTypeId(TypeID id){
  return this->getTypeDescriptionElem(id,2);
}

TypeManager::TypeID TypeManager::getPtrInnerTypeId(TypeID id){
  return this->getTypeDescriptionElem(id,1);
}

TypeManager::TypeID TypeManager::getFceReturnTypeId(TypeID id){
  return this->getTypeDescriptionElem(id,1);
}

unsigned TypeManager::getNofFceArgs(TypeID id){
  return this->getTypeDescriptionElem(id,2);
}

TypeManager::TypeID TypeManager::getFceArgTypeId(TypeID id,unsigned element){
  return this->getTypeDescriptionElem(id,3+element);
}
*/
/*
unsigned TypeManager::getStructElementType(unsigned typeId,unsigned element){

}*/
/*
unsigned TypeManager::addType(std::vector<unsigned>&type,std::string name){
  for(unsigned t=0;t<this->_typeStart.size();++t){
    this->_types[this->_typeStart[t]];
  }
  return 0;
}
*/

