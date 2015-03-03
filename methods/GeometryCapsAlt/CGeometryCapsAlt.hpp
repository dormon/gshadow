#ifndef _CGEOMETRYCAPSALT_HPP_
#define _CGEOMETRYCAPSALT_HPP_

#include<geGL/VertexArrayObject.h>
#include<geGL/BufferObject.h>

#include"../../app/adjacency.hpp"
#include"../../simulationData/SimulationData.h"

#include"../StencilMethod/StencilMethod.h"

#include"shaderdir.hpp"

class CGeometryCapsAlt
{
  private:
    ge::gl::BufferObject*VBO;
    ge::gl::VertexArrayObject*VAO;
    ge::gl::ProgramObject*Program;
    SAdjecency*Adjacency;
  public:
    CGeometryCapsAlt(SAdjecency*Adjacency);
    ~CGeometryCapsAlt();
    void DrawCaps(float*mvp,simulation::Light*Light);
};

#endif//_CGEOMETRYCAPSALT_HPP_
