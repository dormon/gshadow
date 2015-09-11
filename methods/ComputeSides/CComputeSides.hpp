#ifndef _CCOMPUTESIDES_HPP_
#define _CCOMPUTESIDES_HPP_

#include<geGL/VertexArrayObject.h>
#include<geGL/BufferObject.h>

#include"../../app/adjacency.hpp"
#include"../../app/fastAdjacency.h"
#include"../../simulationData/SimulationData.h"

#include<geCore/Command.h>
//#include<geGL/UniformCommands.h>
//#include<geGL/ProgramCommands.h>

#include"shaderdir.hpp"

#include"../ShadowMethod.h"

class CComputeSides
{
  private:
    ge::gl::ProgramObject*     _computeProgram;
    ge::gl::ProgramObject*     _drawProgram;
    ge::gl::BufferObject*      _input;
    ge::gl::BufferObject*      _counter;
    ge::gl::BufferObject*      _output;
    ge::gl::VertexArrayObject* _vao;
    Adjacency*                 _adjacency;
    unsigned                   _workGroupSize;
    unsigned                   _nofEdges;
    ge::core::CommandList*       _computeList;
    unsigned                   _lightUniformCommand;
    unsigned                   _mvpUniformCommand  ;
  public:
    CComputeSides(Adjacency*adjacency,unsigned WorkGroupSize,bool CullSides);
    virtual ~CComputeSides();
    void ComputeSides(float*mvp,simulation::Light*Light);
    void DrawSides(float*mvp,simulation::Light*Light);
};



#endif//_CCOMPUTESIDES_HPP_
