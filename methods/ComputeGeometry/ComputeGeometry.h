#ifndef _COMPUTEGEOMETRY_H_
#define _COMPUTEGEOMETRY_H_

#include<geGL/geGL.h>
#include<geUtil/CameraObject.h>

#include"../../app/adjacency.hpp"
#include"../../app/shaderdir.hpp"

#include"../ShadowMethod.h"
#include"../ComputeSides/CComputeSides.hpp"
#include"../GeometryCapsAlt/CGeometryCapsAlt.hpp"

class ComputeGeometry: public simulation::SimulationObject, public ShadowMethod
{
  private:
    CComputeSides*             _sides;
    CGeometryCapsAlt*          _caps;
    ge::gl::FramebufferObject* _maskFBO;
    ge::gl::ProgramObject*     _blit;
    ge::gl::VertexArrayObject* _emptyVAO;
  public:
    ComputeGeometry(simulation::SimulationData*data);
    ~ComputeGeometry();
    unsigned getNofDependentVariables();
    std::string getDependentVariable(unsigned id);
    void update();
    void updateData();
    void updateProgram();
    void createShadowMask();
    void createShadowMask(ge::gl::TextureObject*mask);
};

#endif//_COMPUTEGEOMETRY_H_
