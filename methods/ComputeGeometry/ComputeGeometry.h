#pragma once

#include<geGL/geGL.h>
#include<geUtil/CameraObject.h>

#include"../ShadowMethod.h"
#include"../ComputeSides/CComputeSides.hpp"
#include"../GeometryCapsAlt/CGeometryCapsAlt.hpp"

#include<geUtil/User.h>
#include<geUtil/NamespaceWithUsers.h>

class ComputeGeometry: public ge::util::sim::User, public ShadowMethod
{
  private:
    CComputeSides*             _sides    = nullptr;
    CGeometryCapsAlt*          _caps     = nullptr;
    ge::gl::FramebufferObject* _maskFBO  = nullptr;
    ge::gl::ProgramObject*     _blit     = nullptr;
    ge::gl::VertexArrayObject* _emptyVAO = nullptr;
  public:
    ComputeGeometry(std::shared_ptr<ge::util::sim::NamespaceWithUsers>const&data);
    ~ComputeGeometry();
    unsigned getNofDependentVariables();
    std::string getDependentVariable(unsigned id);
    void update();
    void updateData();
    void updateProgram();
    void createShadowMask();
    void createShadowMask(GLuint mask);
    void createShadowMask(ge::gl::TextureObject*mask);
};
