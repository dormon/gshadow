#ifndef _VERTEXZPASS_H_
#define _VERTEXZPASS_H_

#include"../ShadowMethod.h"
#include<geUtil/CameraObject.h>


class VertexZPass: public simulation::SimulationObject, public ShadowMethod
{
  private:
    ge::gl::VertexArrayObject*  _vaoSideU;
    ge::gl::BufferObject*       _vboSideU;
    ge::gl::VertexArrayObject** _vaoSideS;
    ge::gl::BufferObject**      _vboSideS;
    unsigned                    _nofS;
    ge::gl::ProgramObject*      _programSide;

    ge::gl::FramebufferObject*  _shadowMaskFBO;
    ge::gl::ProgramObject*      _createShadowMask;

    simulation::Gauge*               _drawSidesGauge;
    simulation::Gauge*               _blitGauge;
    ge::gl::AsynchronousQueryObject* _measureDrawSides;
    ge::gl::AsynchronousQueryObject* _measureBlit;

    void _setMeasureDrawSides();
    void _setMeasureBlit();

    void _updatedAdjacency();
    void _updateProgram   ();
    void _updateFBO       ();

    void _createUniversal ();
    void _createSpecific  ();
    void _updateData      ();
    void _deleteUniversal ();
    void _deleteSpecific  ();
    void _drawStencil();
  public:
    void createShadowMask();
    unsigned getNofDependentVariables();
    std::string getDependentVariable(unsigned id);
    void update();
    VertexZPass(simulation::SimulationData*data);
    ~VertexZPass();
};

#endif//_VERTEXZPASS_H_
