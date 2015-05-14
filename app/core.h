#pragma once 

#include<iostream>
#include<cstring>
#include<sstream>

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


