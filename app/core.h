#pragma once 

#include<iostream>
#include<cstring>
#include<sstream>
#include<vector>

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

unsigned getDispatchSize(unsigned workSize,unsigned workGroupSize);
