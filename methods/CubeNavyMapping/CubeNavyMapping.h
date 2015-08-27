#pragma once

#include"../ShadowMethod.h"
#include"../ShadowMethodMacro.h"

#include"../NavyMapping/CreateWarping.h"
#include"../NavyMapping/CreateNavyShadowMap.h"

class CubeNavyMapping: public simulation::SimulationObject, public ShadowMethod
{
  private:
    ge::gl::TextureObject*     _shadowMask       = NULL;
    ge::gl::FramebufferObject* _shadowMaskFBO    = NULL;
    ge::gl::ProgramObject*     _createShadowMask = NULL;
    ge::gl::TextureObject*     _shadowMap        = NULL;
    ge::gl::FramebufferObject* _fbo[6]           ={NULL,NULL,NULL,NULL,NULL,NULL};
    ge::gl::VertexArrayObject* _emptyVAO         = NULL;

    ge::gl::TextureObject*_desiredView[6] = {NULL,NULL,NULL,NULL,NULL,NULL};
    ge::gl::TextureObject*_smoothX    [6] = {NULL,NULL,NULL,NULL,NULL,NULL};
    ge::gl::TextureObject*_smoothY    [6] = {NULL,NULL,NULL,NULL,NULL,NULL};
    CreateWarping*       _createWarping       = NULL;
    CreateNavyShadowMap* _createNavyShadowMap = NULL;

    glm::mat4                  _lightProjection;
    glm::mat4                  _lightView[6];
    glm::mat4                  _bpv[6];
    DEF_UPDATEDATA(CubeNavyMapping);
  public:
    void createShadowMap();
    void _createShadowMap();
    void _computeMatrices();
    void _createShadowMaskFBO();
    CubeNavyMapping(simulation::SimulationData*data);
    ~CubeNavyMapping();
    void createShadowMask();
    void createShadowMask(GLuint mask);
    unsigned getNofDependentVariables();
    std::string getDependentVariable(unsigned id);
    virtual void update();
};




