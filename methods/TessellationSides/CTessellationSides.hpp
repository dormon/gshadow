#ifndef _CTESSELLATIONSILHOUETTE_HPP_
#define _CTESSELLATIONSILHOUETTE_HPP_

#include<geGL/VertexArrayObject.h>
#include<geGL/ProgramObject.h>
#include<geGL/BufferObject.h>

#include"../../app/fastAdjacency.h"
#include"../../simulationData/SimulationData.h"

#include"shaderdir.hpp"
class CTessellationSides
{
  private:
    ge::gl::BufferObject*VBO;
    ge::gl::BufferObject*EBO;
    ge::gl::VertexArrayObject*VAO;
    ge::gl::ProgramObject*Program;
    unsigned   _patchVertices;
    unsigned   _nofEdges;
  public:
    CTessellationSides(Adjacency*adjacency,
        bool UseReferenceEdge,bool CullSides,
        bool UseStencilValueExport);
    ~CTessellationSides();
    void DrawSides(float*mvp,simulation::Light*Light);
};

#endif//_CTESSELLATIONSILHOUETTE_HPP_
