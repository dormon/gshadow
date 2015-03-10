#ifndef _SHADOWMAPPING_HPP_
#define _SHADOWMAPPING_HPP_

#include"../ShadowMethod.h"

class CShadowMapping: public simulation::SimulationObject, public ShadowMethod
{
  private:
    ge::gl::TextureObject*     _position;
    ge::gl::TextureObject*     _shadowMask;
    ge::gl::FramebufferObject* _shadowMaskFBO;
    ge::gl::ProgramObject*     _createShadowMask;
    ge::gl::ProgramObject*     _csm;
    ge::gl::ProgramObject*     _dsm;
    ge::gl::TextureObject*     _shadowMap;
    ge::gl::FramebufferObject* _fbo;
    simulation::Gauge*               _createrShadowMapGauge;
    simulation::Gauge*               _createrShadowMaskGauge;
    ge::gl::AsynchronousQueryObject* _measureCreateShadowMap;
    ge::gl::AsynchronousQueryObject* _measureCreateShadowMask;
		unsigned                   _shadowMapSize;
    unsigned                   _screenSize[2];
    SAdjecency*                _adjacency;
    ge::gl::VertexArrayObject* _sceneVAO;
    ge::gl::VertexArrayObject* _emptyVAO;
    glm::mat4                  _bias;
    glm::mat4                  _lightProjection;
    glm::mat4                  _lightView;
    glm::mat4                  _bpv;
    simulation::Light*         _light;
    glm::vec3                  _focusPoint;
    float                      _fovy;
    float                      _near;
    float                      _far;
    void _setMeasureCreateShadowMap();
    void _setMeasureCreateShadowMask();
    void _createShadowMap();
    void _setWindowSize  ();
    void _computeMatrices();
    void _createShadowMapFBO();
  public:
    ge::gl::TextureObject*getShadowMap();
    void setMatrices(glm::mat4 lp,glm::mat4 lv);
    void createShadowMask();
    unsigned getNofDependentVariables();
    std::string getDependentVariable(unsigned id);
    void update();
    CShadowMapping(simulation::SimulationData*data);
    ~CShadowMapping();
    void CreateShadowMap();
    void drawShadowMap(float x,float y,float w);
};

#endif//_SHADOWMAPPING_HPP_
