#pragma once

#include<geGL/geGL.h>
#include"../ShadowMethod.h"

#include<geUtil/User.h>
#include<geUtil/NamespaceWithUsers.h>

#include<geCore/interpret.h>
#include<geCore/stdFunctions.h>


/*
class ShadowMapping: public ge::core::Function{
  protected:
    std::shared_ptr<ge::gl::ProgramObject>createShadowMask;
  public:
    ShadowMapping(std::shared_ptr<ge::core::Accessor>shadowMask = nullptr);
    virtual void operator()();
};

class CreateProgramObject: public ge::core::Function{
  public:
    CreateProgramObject(unsigned stages,std::shared_ptr<ge::core::Accessor>const&program = nullptr);
    virtual void operator()();
};
*/


class CShadowMapping: public ge::util::sim::User, public ShadowMethod
{
  private:
    ge::gl::TextureObject*     _shadowMask       = nullptr;
    ge::gl::FramebufferObject* _shadowMaskFBO    = nullptr;
    ge::gl::ProgramObject*     _createShadowMask = nullptr;
    ge::gl::ProgramObject*     _csm              = nullptr;
    ge::gl::TextureObject*     _shadowMap        = nullptr;
    ge::gl::FramebufferObject* _fbo              = nullptr;
    ge::gl::VertexArrayObject* _emptyVAO         = nullptr;
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
    //DEF_UPDATEDATA(CShadowMapping);
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
    CShadowMapping(std::shared_ptr<ge::util::sim::NamespaceWithUsers>const&data);
    ~CShadowMapping();
};

