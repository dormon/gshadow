#ifndef _CCOMPUTESIDES_HPP_
#define _CCOMPUTESIDES_HPP_

#include<geGL/VertexArrayObject.h>
#include<geGL/BufferObject.h>

#include"../../app/adjacency.hpp"
#include"../../simulationData/SimulationData.h"

#include"shaderdir.hpp"

class CComputeSides
{
  private:
    ge::gl::ProgramObject*ComputeProgram;
    ge::gl::ProgramObject*DrawProgram;
    ge::gl::BufferObject*Input;
    ge::gl::BufferObject*Counter;
    ge::gl::BufferObject*Output;
    ge::gl::VertexArrayObject*VAO;
    SAdjecency*Adjacency;
    std::string GenerateCompute(
        unsigned WorkGroupSize,
        unsigned MaxMultiplicity,
        bool CullSides);
  public:
    CComputeSides(SAdjecency*Adjacency,
        unsigned WorkGroupSize,bool CullSides);
    ~CComputeSides();
    void ComputeSides(float*mvp,simulation::Light*Light);
    void DrawSides(float*mvp,simulation::Light*Light);
};



#endif//_CCOMPUTESIDES_HPP_
