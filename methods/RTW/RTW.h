#ifndef _RTW_H_
#define _RTW_H_

#include"../ShadowMethod.h"

class RTWBack: public simulation::SimulationObject, public ShadowMethod
{
  private:
    ge::gl::TextureObject*     _shadowMask           = NULL;
    ge::gl::FramebufferObject* _shadowMaskFBO        = NULL;
    ge::gl::TextureObject*     _shadowMap            = NULL;
    ge::gl::FramebufferObject* _fbo                  = NULL;
    ge::gl::TextureObject*     _importanceMap        = NULL;
    ge::gl::ProgramObject*     _createImportanceMap  = NULL;
    ge::gl::TextureObject*     _importanceX          = NULL;
    ge::gl::TextureObject*     _importanceY          = NULL;
    ge::gl::ProgramObject*     _create1DImportance   = NULL;
    ge::gl::TextureObject*     _smoothX              = NULL;
    ge::gl::TextureObject*     _smoothY              = NULL;
    ge::gl::ProgramObject*     _smoothProgram        = NULL;
    ge::gl::TextureObject*     _sumX                 = NULL;
    ge::gl::TextureObject*     _sumY                 = NULL;
    ge::gl::ProgramObject*     _sumProgram           = NULL;
    ge::gl::ProgramObject*     _drawGridProgram      = NULL;
    ge::gl::ProgramObject*     _createRTWProgram     = NULL;
    ge::gl::ProgramObject*     _createRTWMaskProgram = NULL;
    ge::gl::VertexArrayObject* _emptyVAO             = NULL;
    glm::mat4                  _lightProjection;
    glm::mat4                  _lightView;
    glm::mat4                  _bpv;
    void _createImportance();
    void _createImportance1D();
    void _smoothImportance1D();
    void _sumImportance1D();
    void _createRTWMap();
    void _createShadowMapMask();
    void _createRTWMask();
    void _deleteTextures();
  public:
    void _createShadowMap();
    void _computeMatrices();
    void _createShadowMapFBO();
    void _allocTextures();


    void setMatrices(glm::mat4 lp,glm::mat4 lv);
    void createShadowMask();
    void createShadowMask(GLuint mask);

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
