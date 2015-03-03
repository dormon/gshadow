#ifndef _CGEOMETRYCAPS_HPP_
#define _CGEOMETRYCAPS_HPP_

#include<geGL/VertexArrayObject.h>
#include<geGL/BufferObject.h>

#include"../../app/adjacency.hpp"
#include"../../simulationData/SimulationData.h"

#include"../GeometrySides/CGeometrySides.hpp"

#include"shaderdir.hpp"

class CGeometryCaps
{
  private:
    ge::gl::BufferObject         *UVBO;
    ge::gl::VertexArrayObject    *UVAO;
    ge::gl::BufferObject        **SVBO;
    ge::gl::VertexArrayObject   **SVAO;
    ge::gl::ProgramObject        *UProgram;
    ge::gl::ProgramObject       **SProgram;
    bool                          Universal;
    SAdjecency                   *Adjacency;
  public:
    CGeometryCaps(
        SAdjecency        *Adjacency,
        SGeometryTemplate  Template);
    ~CGeometryCaps();
    void DrawCaps(float*mvp,simulation::Light*Light);
};


#endif//_CGEOMETRYCAPS_HPP_
