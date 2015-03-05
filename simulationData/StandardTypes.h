#ifndef _STANDARDTYPES_H_
#define _STANDARDTYPES_H_

#define GLM_FORCE_RADIANS
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>

#include<iostream>
#include<vector>
#include<map>

#include<geGL/geGL.h>

namespace simulation{
  enum SimType{
    BASE=0,
    BOOL,
    INT,
    IVEC2,
    IVEC3,
    IVEC4,
    UINT,
    UVEC2,
    UVEC3,
    UVEC4,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    MAT2,
    MAT3,
    MAT4,
    STRING,
    OBJECT,
    LIGHT,
    GAUGE
  };
  class Data{
    public:
      SimType type;
      virtual std::string toStr()=0;
      virtual bool operator==(Data*)=0;
      virtual ~Data(){}
  };

  class Bool: public Data{
    public:
      bool data;
      Bool(std::string input);
      Bool(bool input);
      std::string toStr();
      bool operator==(Data*);
  };
  class Int: public Data{
    public:
      int data;
      Int(std::string input);
      Int(int input);
      std::string toStr();
      bool operator==(Data*);

  };
  class IVec2: public Data{
    public:
      glm::ivec2 data;
      IVec2(std::string input);
      IVec2(glm::ivec2 input);
      std::string toStr();
      bool operator==(Data*);

  };
  class IVec3: public Data{
    public:
      glm::ivec3 data;
      IVec3(std::string input);
      IVec3(glm::ivec3 input);
      std::string toStr();
      bool operator==(Data*);

  };
  class IVec4: public Data{
    public:
      glm::ivec4 data;
      IVec4(std::string input);
      IVec4(glm::ivec4 input);
      std::string toStr();
      bool operator==(Data*);

  };
  class Uint: public Data{
    public:
      unsigned data;
      Uint(std::string input);
      Uint(unsigned input);
      std::string toStr();
      bool operator==(Data*);
  };
  class UVec2: public Data{
    public:
      glm::uvec2 data;
      UVec2(std::string input);
      UVec2(glm::uvec2 input);
      std::string toStr();
      bool operator==(Data*);
  };
  class UVec3: public Data{
    public:
      glm::uvec3 data;
      UVec3(std::string input);
      UVec3(glm::uvec3 input);
      std::string toStr();
      bool operator==(Data*);
  };
  class UVec4: public Data{
    public:
      glm::uvec4 data;
      UVec4(std::string input);
      UVec4(glm::uvec4 input);
      std::string toStr();
      bool operator==(Data*);
  };
  class Float: public Data{
    public:
      float data;
      Float(std::string input);
      Float(float input);
      std::string toStr();
      bool operator==(Data*);
  };
  class Vec2: public Data{
    public:
      glm::vec2 data;
      Vec2(std::string input);
      Vec2(glm::vec2 input);
      std::string toStr();
      bool operator==(Data*);
  };
  class Vec3: public Data{
    public:
      glm::vec3 data;
      Vec3(std::string input);
      Vec3(glm::vec3 input);
      std::string toStr();
      bool operator==(Data*);
  };
  class Vec4: public Data{
    public:
      glm::vec4 data;
      Vec4(std::string input);
      Vec4(glm::vec4 input);
      std::string toStr();
      bool operator==(Data*);
  };
  class String: public Data{
    public:
      std::string data;
      String(std::string input);
      std::string toStr();
      bool operator==(Data*);
  };
  class Mat4: public Data{
    public:
      glm::mat4 data;
      Mat4(glm::mat4 input);
      Mat4();
      std::string toStr();
      bool operator==(Data*);
  };
  class Object: public Data{
    public:
      void* data;
      Object(void*data);
      std::string toStr();
      bool operator==(Data*);
  };
  class Light: public Data{
    public:
      glm::vec4 position;
      glm::vec3 diffuse;
      glm::vec3 specular;
      Light(glm::vec4 position,glm::vec3 diffuse,glm::vec3 specular);
      std::string toStr();
      bool operator==(Data*);
  };

  class Gauge: public Data{
    public:
      std::vector<float>values;
      std::vector<float>*getData();
      void insert(float value);
      Gauge();
      std::string toStr();
      bool operator==(Data*);
  };

}

#endif//_STANDARDTYPES_H_
