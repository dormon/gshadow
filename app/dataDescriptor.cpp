#include"dataDescriptor.h"
/*
using namespace lang;

DataT::DataT(Type type){ 
  this->_type = type;
}
DataT::DataT(){
  this->_type = Type::VOID;
}
DataT::Type DataT::getType(){
  return this->_type;
}
bool DataT::operator==(DataT&b){
  return this->_type==b._type;
}
ArrayT::ArrayT(
    DataT innerType,
    unsigned       size     ):DataT(Type::ARRAY){
  this->_innerType = innerType;
  this->_size      = size     ;
}
bool ArrayT::operator==(DataT&b){
  if(this->_type!=b.getType())return false;
  return this->_size==((ArrayT&)b)._size;
}
DataT&ArrayT::getInnerType(){
  return this->_innerType;
}
unsigned ArrayT::getSize(){
  return this->_size;
}
StructT::StructT(): DataT(Type::STRUCT){}
void StructT::addElement(DataT&descriptor){
  this->_elements.push_back(descriptor);
}
bool StructT::operator==(DataT&b){
  if(this->_type!=b.getType())return false;
  if(this->getNofElements()!=((StructT&)b).getNofElements())return false;
  for(unsigned i=0;i<this->getNofElements();++i)
    if(!(this->getElement(i)==((StructT&)b).getElement(i)))return false;
  return true;
}
DataT&StructT::getElement(unsigned i){
  return this->_elements[i];
}
unsigned StructT::getNofElements(){
  return this->_elements.size();
}
PtrT::PtrT(
    DataT&innerType): DataT(Type::PTR){
  this->_innerType = innerType;
}
PtrT::PtrT(
    DataT innerType): DataT(Type::PTR){
  this->_innerType = innerType;
}
bool PtrT::operator==(DataT&b){
  if(this->_type!=b.getType())return false;
  return this->getInnerType()==((PtrT&)b).getInnerType();
}
DataT&PtrT::getInnerType(){
  return this->_innerType;
}

FunctionDescriptor::FunctionDescriptor(DataT&returnType): DataT(DataT::FCE){
  this->_returnType     = returnType;
}

DataT&FunctionDescriptor::getReturnType(){
  return this->_returnType;
}
DataT&FunctionDescriptor::getParameterType(unsigned i){
  return this->_parametersType[i];
}
unsigned FunctionDescriptor::getNofParameters(){
  return this->_parametersType.size();
}

bool FunctionDescriptor::operator==(DataT&b){
  if(this->getType()!=b.getType())return false;
  if(this->getNofParameters()!=((FunctionDescriptor&)b).getNofParameters())return false;
  if(!(this->getReturnType() ==((FunctionDescriptor&)b).getReturnType()))return false;
  for(unsigned i=0;i<this->_parametersType.size();++i)
    if(!(this->getParameterType(i)==((FunctionDescriptor&)b).getParameterType(i)))return false;
  return true;
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
    ArrayT& descriptor): Data(manager->addType(descriptor),manager){  
  this->_data = new Data*[descriptor.getSize()];
  for(unsigned i=0;i<descriptor.getSize();++i)
    this->_data[i] = this->_manager->allocate(descriptor.getInnerType());
}

ArrayData::~ArrayData(){
  unsigned size=((ArrayT&)this->_manager->getType(this->_type)).getSize();
  for(unsigned i=0;i<size;++i)
    delete this->_data[i];
  delete this->_data;
}

Data*ArrayData::operator[](unsigned i){
  return this->_data[i];
}

StructData::StructData(
    TypeManager*      manager   ,
    StructT& descriptor): Data(manager->addType(descriptor),manager){
  this->_data = new Data*[descriptor.getNofElements()];
  for(unsigned i=0;i<descriptor.getNofElements();++i)
    this->_data[i] = this->_manager->allocate(descriptor.getElement(i));
}

StructData::~StructData(){
  unsigned elems=((StructT&)this->_manager->getType(this->_type)).getNofElements();
  for(unsigned i=0;i<elems;++i)
    delete this->_data[i];
  delete this->_data;
}

Data*StructData::operator[](unsigned i){
  return this->_data[i];
}

PtrData::PtrData(
    TypeManager*manager,
    PtrT&descriptor,
    Data*ptr): Data(manager->addType(descriptor),manager){
  this->_data = ptr;
}

PtrData::~PtrData(){}

Data*PtrData::operator*(){
  return this->_data;
}

TypeManager::TypeManager(){
  this->addType(BaseT<DataT::VOID  >(),"void"  );
  this->addType(BaseT<DataT::INT   >(),"int"   );
  this->addType(BaseT<DataT::UINT  >(),"uint"  );
  this->addType(BaseT<DataT::FLOAT >(),"float" );
  this->addType(BaseT<DataT::STRING>(),"string");
  this->addType(PtrT(BaseT<DataT::INT   >()),"int*"   );
  this->addType(PtrT(BaseT<DataT::UINT  >()),"uint*"  );
  this->addType(PtrT(BaseT<DataT::FLOAT >()),"float*" );
  this->addType(PtrT(BaseT<DataT::STRING>()),"string*");
  this->addType(ArrayT(BaseT<DataT::FLOAT>(),2),"vec2");
  this->addType(ArrayT(BaseT<DataT::FLOAT>(),3),"vec3");
  this->addType(ArrayT(BaseT<DataT::FLOAT>(),4),"vec4");
}

TypeManager::~TypeManager(){
}

unsigned TypeManager::addType(DataT descriptor,std::string name){
  for(unsigned i=0;i<this->_types.size();++i)
    if(this->_types[i]==descriptor)return i;

  if(descriptor.getType()==DataT::ARRAY)
    this->addType(((ArrayT&)descriptor).getInnerType());
  if(descriptor.getType()==DataT::STRUCT){
    unsigned elems=((StructT&)descriptor).getNofElements();
    for(unsigned e=0;e<elems;++e)
      this->addType(((StructT&)descriptor).getElement(e));
  }
  if(descriptor.getType()==DataT::PTR)
    this->addType(((PtrT&)descriptor).getInnerType());

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

DataT&TypeManager::getType(unsigned id){
  return this->_types[id];
}

DataT&TypeManager::getType(std::string name){
  return this->_types[this->_name2Id[name]];
}

unsigned TypeManager::getId(std::string name){
  return this->_name2Id[name];
}

unsigned TypeManager::getId(DataT&descriptor){
  for(unsigned i=0;i<this->_types.size();++i)
    if(this->_types[i]==descriptor)return i;
  return this->getId("void");
}
Data*TypeManager::allocate(DataT&descriptor){
  if(descriptor.getType()==DataT::INT   )return new BaseData<int        >(this);
  if(descriptor.getType()==DataT::UINT  )return new BaseData<unsigned   >(this);
  if(descriptor.getType()==DataT::FLOAT )return new BaseData<float      >(this);
  if(descriptor.getType()==DataT::STRING)return new BaseData<std::string>(this);
  if(descriptor.getType()==DataT::STRUCT)return new StructData(this,(StructT&)descriptor);
  if(descriptor.getType()==DataT::ARRAY )return new ArrayData (this,(ArrayT &)descriptor);
  if(descriptor.getType()==DataT::PTR   )return new PtrData   (this,(PtrT   &)descriptor);
  return NULL;
}

Data*TypeManager::allocate(unsigned id){
  return this->allocate(this->getType(id));
}

Data*TypeManager::allocate(std::string name){
  return this->allocate(this->_name2Id[name]);
}*/
