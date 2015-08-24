#pragma once

#include"../ShadowMethod.h"
#include"../ShadowMethodMacro.h"

class CubeShadowMapping: public simulation::SimulationObject, public ShadowMethod
{
  private:
    ge::gl::TextureObject*     _shadowMask       = NULL;
    ge::gl::FramebufferObject* _shadowMaskFBO    = NULL;
    ge::gl::ProgramObject*     _createShadowMask = NULL;
    ge::gl::ProgramObject*     _csm              = NULL;
    ge::gl::TextureObject*     _shadowMap        = NULL;
    ge::gl::FramebufferObject* _fbo[6]           ={NULL,NULL,NULL,NULL,NULL,NULL};
    ge::gl::VertexArrayObject* _emptyVAO         = NULL;
    glm::mat4                  _lightProjection;
    glm::mat4                  _lightView[6];
    glm::mat4                  _bpv[6];
    DEF_UPDATEDATA(CubeShadowMapping);
  public:
    void createShadowMap();
    void _createShadowMap();
    void _computeMatrices();
    void _createShadowMaskFBO();
    CubeShadowMapping(simulation::SimulationData*data);
    ~CubeShadowMapping();
    void createShadowMask();
    void createShadowMask(GLuint mask);
    unsigned getNofDependentVariables();
    std::string getDependentVariable(unsigned id);
    virtual void update();
};


