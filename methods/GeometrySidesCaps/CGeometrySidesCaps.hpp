#ifndef _CGEOMETRYSIDESCAPS_HPP_
#define _CGEOMETRYSIDESCAPS_HPP_

#if 0

#include<geGL/VertexArrayObject.h>
#include<geGL/BufferObject.h>

#include"../../simulationData/SimulationData.h"

#include"shaderdir.hpp"
class CGeometrySidesCaps
{
  private:
    ge::gl::BufferObject*UVBO;
    ge::gl::VertexArrayObject*UVAO;
    ge::gl::BufferObject**SVBO;
    ge::gl::VertexArrayObject**SVAO;
    ge::gl::ProgramObject*UProgram;
    ge::gl::ProgramObject**SProgram;
    bool Universal;
    SAdjecency*Adjacency;
  public:
    CGeometrySidesCaps(SAdjecency*Adjacency,
        bool Deterministic,
        bool ReferenceEdge,
        bool Universal,
        bool UseVS2GSArray,
        bool UseVertexArray,
        bool UseStencilValueExport,
        bool CullSides,
        bool FrontCapToInfinity,
        unsigned Version);
    ~CGeometrySidesCaps();
    void DrawSidesCaps(float*mvp,simulation::Light*Light);

};

#endif

#endif//_CGEOMETRYSIDESCAPS_HPP_
