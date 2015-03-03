#ifndef _STENCILMETHOD_H_
#define _STENCILMETHOD_H_

#include"../../app/adjacency.hpp"
#include"../../simulationData/SimulationData.h"

#include"shaderdir.hpp"

class StencilMethod
{
  private:
    SAdjecency*adjacency;
  public:
    StencilMethod(SAdjecency*adjacency);
    virtual void drawGeometry(float*mvp,simulation::Light*light)=0;
    virtual void draw(float*mvp,simulation::Light*light,bool zfail);
};

#endif//_STENCILMETHOD_H_
