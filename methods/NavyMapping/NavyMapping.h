#ifndef _NAVYMAPPING_HPP_
#define _NAVYMAPPING_HPP_

#include"../ShadowMethod.h"
#include"CreateWarping.h"
#include"CreateNavyShadowMap.h"

class NavyMapping: public simulation::SimulationObject, public ShadowMethod
{
  private:
    ge::gl::TextureObject*     _shadowMask    = NULL;
    ge::gl::FramebufferObject* _shadowMaskFBO = NULL;
    ge::gl::TextureObject*     _shadowMap     = NULL;
    ge::gl::VertexArrayObject* _emptyVAO      = NULL;


    //1. viewMap

    //2. desired view
    ge::gl::TextureObject*_desiredView = NULL;

    //3. countMap

    //4. integratedCountMap

    //5. offsetX

    //6. smoothX
    ge::gl::TextureObject* _smoothX = NULL;
    ge::gl::TextureObject* _smoothY = NULL;

    //6.b smooth

    //7. unwarpX
    
    CreateWarping*_createWarping = NULL;

    //8. tests
    ge::gl::ProgramObject* _drawGridProgram = NULL;

    ge::gl::TextureObject* _uall = NULL;
    ge::gl::ProgramObject* _uallProgram = NULL;

    CreateNavyShadowMap* _createNavyShadowMap = NULL;
    ge::gl::ProgramObject* _createNVMaskProgram = NULL;

    glm::mat4                  _lightProjection;
    glm::mat4                  _lightView;
    glm::mat4                  _bpv;

    void _unwarpAll();

    void _createNVMask();

    void _setNVParam(ge::gl::ProgramObject*prog);

    unsigned _getDiv(unsigned a,unsigned b){return a%b?a/b+1:a/b;}
    void _deleteDVTextures();
    void _deleteWarpingTextures();
  public:
    void _createShadowMap();
    void _computeMatrices();
    void _createShadowMapFBO();
    void _allocDVTextures();
    void _allocWarpingTextures();
    void setMatrices(glm::mat4 lp,glm::mat4 lv);
    void createShadowMask();
    void createShadowMask(GLuint mask);
    unsigned getNofDependentVariables();
    std::string getDependentVariable(unsigned id);
    void update();
    NavyMapping(simulation::SimulationData*data);
    ~NavyMapping();
    ge::gl::TextureObject*getShadowMap(){return this->_shadowMap;}
    ge::gl::TextureObject*getSmoothX(){return this->_smoothX;}
    ge::gl::TextureObject*getSmoothY(){return this->_smoothY;}
    void drawGrid(float x,float y,float sx,float sy);
    ge::gl::TextureObject*getuall(){return this->_uall;}

};

#endif//_NAVYMAPPING_HPP_
