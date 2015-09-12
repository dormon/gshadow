#ifndef _CGEOMETRYCAPSALT_HPP_
#define _CGEOMETRYCAPSALT_HPP_

#include<geGL/VertexArrayObject.h>
#include<geGL/BufferObject.h>

#include"../../app/adjacency.hpp"
#include"../../app/fastAdjacency.h"
#include"../../simulationData/SimulationData.h"

#include"../StencilMethod/StencilMethod.h"

#include"shaderdir.hpp"

class CGeometryCapsAlt
{
  private:
    ge::gl::BufferObject*VBO = nullptr;
    ge::gl::VertexArrayObject*VAO = nullptr;
    ge::gl::ProgramObject*Program = nullptr;
    unsigned _nofTriangles = 0;
    SAdjecency*_adjacency = nullptr;
  public:
    CGeometryCapsAlt(Adjacency*adjacency);
    ~CGeometryCapsAlt();
    void DrawCaps(float*mvp,simulation::Light*Light);
};

#endif//_CGEOMETRYCAPSALT_HPP_
