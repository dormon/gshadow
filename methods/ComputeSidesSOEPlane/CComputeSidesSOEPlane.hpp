#ifndef _COMPUTESIDESSOEPLANE_HPP_
#define _COMPUTESIDESSOEPLANE_HPP_

#include<geGL/VertexArrayObject.h>
#include<geGL/BufferObject.h>

#include"../../simulationData/SimulationData.h"

#include"shaderdir.hpp"

/*

class CComputeSidesSOEPlane
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
    CComputeSidesSOEPlane(SAdjecency*Adjacency,
        unsigned WorkGroupSize,bool CullSides);
    ~CComputeSidesSOEPlane();
    void ComputeSides(float*mvp,simulation::Light*Light);
    void DrawSides(float*mvp,simulation::Light*Light);
};

*/

#endif//_COMPUTESIDESSOEPLANE_HPP_
