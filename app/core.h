#pragma once 

#include<iostream>
#include<cstring>
#include<sstream>
#include<vector>
#include<map>

/*
#define DEF_ENUM(name,...)\
  enum name{\
    __VA_ARGS__\
  };\
friend std::ostream& operator<<(std::ostream& os,const name&val){\
  char data[]=#__VA_ARGS__;\
  char*token=std::strtok(data,",");\
  unsigned counter=0;\
  while(counter<unsigned(val)){\
    token=std::strtok(NULL,",");\
    counter++;\
  }\
  os << token;\
  return os;\
}
*/
template<typename T>
void _argsToVector(std::vector<T>&vec,T v){
  vec.push_back(v);
}
template<typename T,typename... Args>
void _argsToVector(std::vector<T>&vec,T v,Args... args){
  vec.push_back(v);
  _argsToVector(vec,args...);
}

template<typename TPtr>
void deleteSetNull(TPtr&b){
  if(b)delete b;b=NULL;
}

template<typename TPtr,typename...Args>
void deleteSetNull(TPtr&b,Args... args){
  if(b)delete b;b=NULL;
  deleteSetNull(args...);
}

template<typename T>
void filterArgsToVector(std::vector<T>&data,T t){
  data.push_back(t);
}

template<typename T,typename T2>
void filterArgsToVector(std::vector<T>&,T2){}

template<typename T,typename T2,typename... Args>
void filterArgsToVector(std::vector<T>&data,T2,Args... args);

template<typename T,typename... Args>
void filterArgsToVector(std::vector<T>&data,T t,Args... args){
  filterArgsToVector(data,t);
  filterArgsToVector(data,args...);
}

template<typename T,typename T2,typename... Args>
void filterArgsToVector(std::vector<T>&data,T2,Args... args){
  filterArgsToVector(data,args...);
}

template<typename KEY,typename VAL>
void argsToMapOfVectors(std::map<KEY,std::vector<VAL>>&data,KEY n,VAL t){
  if(!data.count(n))data.insert(std::pair<KEY,std::vector<VAL>>(n,std::vector<VAL>()));
  data[n].push_back(t);
}

template<typename KEY,typename VAL,typename... Args>
void argsToMapOfVectors(std::map<KEY,std::vector<VAL>>&data,KEY n,VAL t,Args... args);

template<typename KEY,typename VAL,typename... Args>
void argsToMapOfVectors(std::map<KEY,std::vector<VAL>>&data,KEY,KEY n2,VAL t,Args... args){
  argsToMapOfVectors(data,n2,t,args...);
}

template<typename KEY,typename VAL,typename... Args>
void argsToMapOfVectors(std::map<KEY,std::vector<VAL>>&data,KEY n,VAL t,Args... args){
  argsToMapOfVectors(data,n,t);
  argsToMapOfVectors(data,n,args...);
}



unsigned getDispatchSize(unsigned workSize,unsigned workGroupSize);


