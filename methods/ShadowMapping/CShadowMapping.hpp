#ifndef _SHADOWMAPPING_HPP_
#define _SHADOWMAPPING_HPP_

#include"../ShadowMethod.h"

class CShadowMapping: public simulation::SimulationObject, public ShadowMethod
{
  private:
    ge::gl::TextureObject*     _shadowMask;
    ge::gl::FramebufferObject* _shadowMaskFBO;
    ge::gl::ProgramObject*     _createShadowMask;
    ge::gl::ProgramObject*     _csm;
    ge::gl::TextureObject*     _shadowMap;
    ge::gl::FramebufferObject* _fbo;
    ge::gl::VertexArrayObject* _emptyVAO;
    glm::mat4                  _lightProjection;
    glm::mat4                  _lightView;
    glm::mat4                  _bpv;
    void CreateShadowMap();

    /*
    template<const char*name,void(CShadowMapping::*fce)()>
    void _update(){
      if(this->_changed[name]){
        (this->*fce)();
        this->_changed[name]=false;
      }
    }
    template<const char*name,void(CShadowMapping::*fce)(),typename... Args>
    void _update(){
      if(this->_changed[name]){
        (this->*fce)();
        this->_changed[name]=false;
      }
      this->_update<Args...>();
    }*/

  public:
    void _createShadowMap();
    void _computeMatrices();
    void _createShadowMapFBO();

    ge::gl::TextureObject*getShadowMap();
    void setMatrices(glm::mat4 lp,glm::mat4 lv);
    void createShadowMask();
    void createShadowMask(GLuint mask);
    unsigned getNofDependentVariables();
    std::string getDependentVariable(unsigned id);
    virtual void update();
    CShadowMapping(simulation::SimulationData*data);
    ~CShadowMapping();
};

#endif//_SHADOWMAPPING_HPP_
