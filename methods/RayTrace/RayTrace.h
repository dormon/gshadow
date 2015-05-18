#pragma once

#include"../ShadowMethod.h"

class RayTrace: public simulation::SimulationObject, public ShadowMethod{
  private:
    unsigned              _nofTriangles;
    ge::gl::ProgramObject*_rayProgram;
    ge::gl::BufferObject*_triangles;
  public:
    virtual void createShadowMask();
    virtual void createShadowMask(GLuint mask);
    virtual void update();
    virtual unsigned getNofDependentVariables();
    virtual std::string getDependentVariable(unsigned id);
    RayTrace(simulation::SimulationData*data);
    ~RayTrace();
};
