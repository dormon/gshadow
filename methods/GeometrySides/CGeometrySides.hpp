#ifndef _CGEOMETRYSIDES_HPP_
#define _CGEOMETRYSIDES_HPP_

#include<geGL/VertexArrayObject.h>
#include<geGL/BufferObject.h>
#include<geGL/VertexArrayObject.h>

#include"../../app/adjacency.hpp"
#include"../../simulationData/SimulationData.h"

#include"shaderdir.hpp"

typedef struct{
  bool Deterministic;          ///< deterministic version
  bool ReferenceEdge;          ///< Reference edge version
  bool Universal;              ///< universal shader
  bool UseVS2GSArray;          ///< toggle usage of array as way of transfer
  bool UseVertexArray;         ///< inputs into vertex shader are in array
  bool UseLayouts;             ///< inputs have layout interface
  bool UseStencilValueExport;  ///< toggle usage of AMD extension
  bool CCWIsFrontFace;         ///< Front Face is CCW
  bool FrontFaceInside;        ///< Front Face are oriented inside Shadow Volume
  bool CullSides;              ///< toggle culling of sides
  bool Visualize;              ///< visualize SV with colors
  bool FrontCapToInfinity;     ///< shift front cap to infinity
  bool GenerateSides;          ///< toggle generation of sides
  bool GenerateCaps;           ///< toggle generation of caps
  std::string Matrix;          ///< mvp matrix uniform name
  unsigned    Version;         ///< version of shader
  std::string Profile;         ///< profile version
  unsigned    MaxMultiplicity; ///< maximal number of opposite vertices
  std::string LightUniform;    ///< light position uniform name
  std::string VertexAttribName;///< name of vertex attribs
}SGeometryTemplate;

bool operator!=(const SGeometryTemplate&a, const SGeometryTemplate&b);

ge::gl::ProgramObject*GenerateGeometryShader(SGeometryTemplate Template);

class CGeometrySides
{
  private:
    ge::gl::BufferObject       *UVBO     ;
    ge::gl::VertexArrayObject  *UVAO     ;
    ge::gl::VertexArrayObject  *uvao     ;
    ge::gl::BufferObject      **SVBO     ;
    ge::gl::VertexArrayObject **SVAO     ;
    ge::gl::ProgramObject      *UProgram ;
    ge::gl::ProgramObject     **SProgram ;
    bool                        Universal;
    SAdjecency                 *Adjacency;
  public:
    CGeometrySides(
        SAdjecency        *Adjacency,
        SGeometryTemplate  Template);
    ~CGeometrySides();
    void DrawSides(
        float  *mvp,
        simulation::Light *Light);
};

#endif//_CGEOMETRYSIDES_HPP_
