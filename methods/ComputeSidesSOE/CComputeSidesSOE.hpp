#ifndef _COMPUTESIDESSOE_HPP_
#define _COMPUTESIDESSOE_HPP_

#include<geGL/VertexArrayObject.h>
#include<geGL/BufferObject.h>

#include"../../simulationData/SimulationData.h"

#include"shaderdir.hpp"

/*

class CComputeSidesSOE
{
  private:
    ge::gl::ProgramObject*ComputeProgram;
    ge::gl::ProgramObject*DrawProgram;
    ge::gl::BufferObject*Input;
    ge::gl::BufferObject*Counter;
    ge::gl::BufferObject*Output;
    ge::gl::BufferObject*Time;
    ge::gl::VertexArrayObject*VAO;
    SAdjecency*Adjacency;
    std::string GenerateCompute(
        unsigned WorkGroupSize,
        unsigned MaxMultiplicity,
        bool CullSides);
  public:
    CComputeSidesSOE(SAdjecency*Adjacency,
        unsigned WorkGroupSize,bool CullSides);
    ~CComputeSidesSOE();
    void ComputeSides(float*mvp,simulation::Light*Light);
    void DrawSides(float*mvp,simulation::Light*Light);
};

*/

#endif//_COMPUTESIDESSOE_HPP_
