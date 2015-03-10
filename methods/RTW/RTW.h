#ifndef _RTW_H_
#define _RTW_H_

#include"../ShadowMethod.h"

class RTWBack: public simulation::SimulationObject, public ShadowMethod
{
  private:
    ge::gl::TextureObject*     _shadowMask;
    ge::gl::FramebufferObject* _shadowMaskFBO;
    ge::gl::TextureObject*     _shadowMap;

    ge::gl::FramebufferObject* _fbo;
    simulation::Gauge*               _createrShadowMapGauge;
    simulation::Gauge*               _createrShadowMaskGauge;
    ge::gl::AsynchronousQueryObject* _measureCreateShadowMap;
    ge::gl::AsynchronousQueryObject* _measureCreateShadowMask;

    ge::gl::TextureObject* _importanceMap;
    ge::gl::ProgramObject* _createImportanceMap;
    ge::gl::TextureObject* _importanceX;
    ge::gl::TextureObject* _importanceY;
    ge::gl::ProgramObject* _create1DImportance;
    ge::gl::TextureObject* _smoothX;
    ge::gl::TextureObject* _smoothY;
    ge::gl::ProgramObject* _smoothProgram;
    ge::gl::TextureObject* _sumX;
    ge::gl::TextureObject* _sumY;
    ge::gl::ProgramObject* _sumProgram;
    ge::gl::ProgramObject* _drawGridProgram;
    ge::gl::ProgramObject* _createRTWProgram;
    ge::gl::ProgramObject* _createRTWMaskProgram;

    ge::gl::VertexArrayObject* _emptyVAO;
    glm::mat4                  _lightProjection;
    glm::mat4                  _lightView;
    glm::mat4                  _bpv;
    void _setMeasureCreateShadowMap();
    void _setMeasureCreateShadowMask();
    void _createShadowMap();
    void _computeMatrices();
    void _createShadowMapFBO();
    void _createImportance();
    void _createImportance1D();
    void _smoothImportance1D();
    void _sumImportance1D();
    void _createRTWMap();
    void _createShadowMapMask();
    void _createRTWMask();
  public:
    void setMatrices(glm::mat4 lp,glm::mat4 lv);
    void createShadowMask();
    unsigned getNofDependentVariables();
    std::string getDependentVariable(unsigned id);
    void update();
    RTWBack(simulation::SimulationData*data);
    ~RTWBack();
    void CreateShadowMap();
    void drawShadowMap(float x,float y,float w);
    ge::gl::TextureObject*getImportanceMap();
    ge::gl::TextureObject*getImportanceX();
    ge::gl::TextureObject*getImportanceY();
    ge::gl::TextureObject*getSmoothX();
    ge::gl::TextureObject*getSmoothY();
    ge::gl::TextureObject*getSumX();
    ge::gl::TextureObject*getSumY();
    ge::gl::TextureObject*getShadowMap();
    void drawGrid(float x,float y,float sx,float sy);



};

#endif//_RTW_H_
