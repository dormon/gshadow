#pragma once

#include"../ShadowMethod.h"
#include"../ShadowMethodMacro.h"

class DualParaboloid: public simulation::SimulationObject, public ShadowMethod
{
  private:
    ge::gl::TextureObject*     _shadowMask       = NULL;
    ge::gl::FramebufferObject* _shadowMaskFBO    = NULL;
    ge::gl::ProgramObject*     _createShadowMask = NULL;
    ge::gl::ProgramObject*     _csm              = NULL;
    ge::gl::TextureObject*     _shadowMap[2]     = {NULL,NULL};
    ge::gl::FramebufferObject* _fbo[2]           = {NULL,NULL};
    ge::gl::VertexArrayObject* _emptyVAO         = NULL;
    glm::mat4                  _lightView[2];
    DEF_UPDATEDATA(DualParaboloid);
  public:
    void createShadowMap();
    void _createShadowMap();
    void _computeMatrices();
    void _createShadowMaskFBO();
    DualParaboloid(simulation::SimulationData*data);
    ~DualParaboloid();
    void createShadowMask();
    void createShadowMask(GLuint mask);
    unsigned getNofDependentVariables();
    std::string getDependentVariable(unsigned id);
    virtual void update();
    ge::gl::TextureObject*getShadowMap(unsigned i){
      return this->_shadowMap[i];
    }
};


