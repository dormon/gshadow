#pragma once

#include<iostream>
#include<fstream>
#include<glm/glm.hpp>
#include<vector>

namespace svg{
  class Triangle3D{
    public:
      enum TriangleNDCCollision{
        NONE=0,
        EDGE,
        FULL
      };
      glm::vec4 p[3];
      glm::vec3 color;
      Triangle3D(glm::vec4 a,glm::vec4 b,glm::vec4 c,glm::vec3 color);
      void toNDC();
      std::string toSvg(unsigned size[2]);
      int operator<(const Triangle3D&b)const;
      TriangleNDCCollision useful();
      glm::vec4 getPoint(glm::vec2 xycoef);
      glm::vec2 getXYCoef(glm::vec2 point);
  };

  class Print
  {
    protected:
      unsigned _size[2];
      std::ofstream _f;
      std::vector<Triangle3D>_triangles;
      void _clip(std::vector<Triangle3D>&clipped,Triangle3D triangle);
    public:
      Print(std::string name,unsigned windowSize[2]);
      void addTriangle(Triangle3D triangle);
      ~Print();
  };

}
