#ifndef _CVERTEXSIDES_HPP_
#define _CVERTEXSIDES_HPP_

#if 0

#include<geGL/VertexArrayObject.h>
#include<geGL/BufferObject.h>
#include"../../app/CGenerateVertexShaderSilhouette.hpp"

#include"../../simulationData/SimulationData.h"

#include"shaderdir.hpp"

class CVertexSides
{
  private:
    ge::gl::VertexArrayObject*VAO;
    ge::gl::BufferObject*VBO;
    ge::gl::ProgramObject*Program;
    SAdjecency*Adjacency;

  public:
    CVertexSides(SAdjecency*Adjacency);
    ~CVertexSides();
    void DrawSides(float*mvp,simulation::Light*Light);
};

#endif

#endif//_CVERTEXSIDES_HPP_
