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

Data::~Data(){}

Data*Data::operator&(){
  return this;
}

ArrayData::ArrayData(
    TypeManager*     manager,
    ArrayDescriptor& descriptor): Data(manager->addType(descriptor),manager){  
  this->_data = new Data*[descriptor.getSize()];
  for(unsigned i=0;i<descriptor.getSize();++i)
    this->_data[i] = this->_manager->allocate(descriptor.getInnerType());
}

ArrayData::~ArrayData(){
  unsigned size=((ArrayDescriptor&)this->_manager->getType(this->_type)).getSize();
  for(unsigned i=0;i<size;++i)
    delete this->_data[i];
  delete this->_data;
}

Data*ArrayData::operator[](unsigned i){
  return this->_data[i];
}

StructData::StructData(
    TypeManager*      manager   ,
    StructDescriptor& descriptor): Data(manager->addType(descriptor),manager){
  this->_data = new Data*[descriptor.getNofElements()];
  for(unsigned i=0;i<descriptor.getNofElements();++i)
    this->_data[i] = this->_manager->allocate(descriptor.getElement(i));
}

StructData::~StructData(){
  unsigned elems=((StructDescriptor&)this->_manager->getType(this->_type)).getNofElements();
  for(unsigned i=0;i<elems;++i)
    delete this->_data[i];
  delete this->_data;
}

Data*StructData::operator[](unsigned i){
  return this->_data[i];
}

PtrData::PtrData(
    TypeManager*manager,
    PtrDescriptor&descriptor,
    Data*ptr): Data(manager->addType(descriptor),manager){
  this->_data = ptr;
}

PtrData::~PtrData(){}

Data*PtrData::operator*(){
  return this->_data;
}

TypeManager::TypeManager(){
  this->addType(IntDescriptor   (),"int"   );
  this->addType(UintDescriptor  (),"uint"  );
  this->addType(FloatDescriptor (),"float" );
  this->addType(StringDescriptor(),"string");
}

TypeManager::~TypeManager(){
}

unsigned TypeManager::addType(DataDescriptor descriptor,std::string name){
  for(unsigned i=0;i<this->_types.size();++i)
    if(this->_types[i]==descriptor)return i;

  if(descriptor.getType()==DataDescriptor::ARRAY)
    this->addType(((ArrayDescriptor&)descriptor).getInnerType());
  if(descriptor.getType()==DataDescriptor::STRUCT){
    unsigned elems=((StructDescriptor&)descriptor).getNofElements();
    for(unsigned e=0;e<elems;++e)
      this->addType(((StructDescriptor&)descriptor).getElement(e));
  }
  if(descriptor.getType()==DataDescriptor::PTR)
    this->addType(((PtrDescriptor&)descriptor).getInnerType());

  this->_types.push_back(descriptor);
  unsigned typeNumber=this->_types.size()-1;
  if(name==""){
    std::stringstream ss;
    ss<<"type"<<typeNumber;
    name=ss.str();
  }
  this->_typeName.push_back(name);
  this->_name2Id.insert(std::pair<std::string,unsigned>(name,typeNumber));
  return typeNumber;
}

std::string TypeManager::getName(unsigned id){
  return this->_typeName[id];
}

DataDescriptor&TypeManager::getType(unsigned id){
  return this->_types[id];
}

DataDescriptor&TypeManager::getType(std::string name){
  return this->_types[this->_name2Id[name]];
}

unsigned TypeManager::getId(std::string name){
  return this->_name2Id[name];
}

Data*TypeManager::allocate(DataDescriptor&descriptor){
  if(descriptor.getType()==DataDescriptor::INT   )return new BaseData<int        >(this);
  if(descriptor.getType()==DataDescriptor::UINT  )return new BaseData<unsigned   >(this);
  if(descriptor.getType()==DataDescriptor::FLOAT )return new BaseData<float      >(this);
  if(descriptor.getType()==DataDescriptor::STRING)return new BaseData<std::string>(this);
  if(descriptor.getType()==DataDescriptor::STRUCT)
    return new StructData(this,(StructDescriptor&)descriptor);
  if(descriptor.getType()==DataDescriptor::ARRAY )
    return new ArrayData (this,(ArrayDescriptor &)descriptor);
  if(descriptor.getType()==DataDescriptor::PTR   )
    return new PtrData   (this,(PtrDescriptor   &)descriptor);
  return NULL;
}

Data*TypeManager::allocate(unsigned id){
  return this->allocate(this->getType(id));
}

Data*TypeManager::allocate(std::string name){
  return this->allocate(this->_name2Id[name]);
}
