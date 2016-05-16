#pragma once

#include<geGL/geGL.h>
#include<geUtil/CameraObject.h>

#include"../ShadowMethod.h"
#include"../ComputeSides/CComputeSides.hpp"
#include"../GeometryCapsAlt/CGeometryCapsAlt.hpp"

#include<geUtil/User.h>
#include<geUtil/NamespaceWithUsers.h>

class DormonShadowTest0: public ge::util::sim::User, public ShadowMethod
{
  private:
    std::shared_ptr<ge::gl::ProgramObject>_sidesProgram = nullptr;
    std::shared_ptr<ge::gl::ProgramObject>_capsProgram  = nullptr;
    std::shared_ptr<ge::gl::ProgramObject>_fillProgram  = nullptr;
    //ge::gl::BufferObject* _edges   = nullptr;
    std::shared_ptr<ge::gl::BufferObject> _edges   = nullptr;
    std::shared_ptr<ge::gl::TextureObject>_contour = nullptr;
    CComputeSides*             _sides    = nullptr;
    CGeometryCapsAlt*          _caps     = nullptr;
    std::shared_ptr<ge::gl::FramebufferObject> _maskFBO  = nullptr;
    ge::gl::ProgramObject*     _blit     = nullptr;
    ge::gl::VertexArrayObject* _emptyVAO = nullptr;
  public:
    DormonShadowTest0(std::shared_ptr<ge::util::sim::NamespaceWithUsers>const&data);
    ~DormonShadowTest0();
    unsigned getNofDependentVariables();
    std::string getDependentVariable(unsigned id);
    void update();
    void updateData();
    void updateProgram();
    void createShadowMask();
    void createShadowMask(GLuint mask);
    void createShadowMask(ge::gl::TextureObject*mask);
};
