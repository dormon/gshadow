#ifndef _CVERTEXCAPS_HPP_
#define _CVERTEXCAPS_HPP_

#include<geGL/VertexArrayObject.h>
#include<geGL/BufferObject.h>
#include"../../app/CGenerateVertexShaderSilhouette.hpp"

#include"../../app/adjacency.hpp"
#include"../../simulationData/SimulationData.h"

#include"shaderdir.hpp"

class CVertexCaps
{
  private:
    ge::gl::VertexArrayObject*VAO;
    ge::gl::BufferObject*VBO;
    ge::gl::ProgramObject*Program;
    SAdjecency*Adjacency;

  public:
    CVertexCaps(SAdjecency*Adjacency);
    ~CVertexCaps();
    void DrawCaps(float*mvp,simulation::Light*Light);
};


#endif//_CVERTEXCAPS_HPP_
