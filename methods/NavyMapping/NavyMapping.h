#ifndef _NAVYMAPPING_HPP_
#define _NAVYMAPPING_HPP_

#include"../ShadowMethod.h"

class NavyMapping: public simulation::SimulationObject, public ShadowMethod
{
  private:
    ge::gl::TextureObject*     _shadowMask    = NULL;
    ge::gl::FramebufferObject* _shadowMaskFBO = NULL;
    ge::gl::TextureObject*     _shadowMap     = NULL;
    ge::gl::FramebufferObject* _fbo           = NULL;
    ge::gl::VertexArrayObject* _emptyVAO      = NULL;


    //1. viewMap
    ge::gl::TextureObject* _viewSamples        = NULL;
    ge::gl::ProgramObject* _viewSamplesProgram = NULL;

    //2. desired view
    ge::gl::ProgramObject* _fastdv0Program = NULL;
    ge::gl::ProgramObject* _fastdvProgram  = NULL;
    std::vector<ge::gl::TextureObject*>_dvsTex;
    std::vector<glm::uvec2>_dvsWorkSize;

    //3. countMap
    ge::gl::TextureObject* _countMapX = NULL;
    ge::gl::TextureObject* _countMapY = NULL;
    ge::gl::ProgramObject* _fastCreateCountMapProgram = NULL;

    //4. integratedCountMap
    ge::gl::TextureObject* _integratedX = NULL;
    ge::gl::TextureObject* _integratedY = NULL;
    ge::gl::TextureObject* _integratedXCount = NULL;
    ge::gl::TextureObject* _integratedYCount = NULL;
    ge::gl::ProgramObject* _integrateProgram = NULL;

    //5. offsetX
    ge::gl::TextureObject* _offsetX = NULL;
    ge::gl::TextureObject* _offsetY = NULL;
    ge::gl::ProgramObject* _offsetProgram = NULL;

    //6. smoothX
    ge::gl::TextureObject* _smoothX = NULL;
    ge::gl::TextureObject* _smoothY = NULL;
    ge::gl::ProgramObject* _smoothProgram = NULL;

    //6.b smooth
    ge::gl::TextureObject* _integratedOffsetX = NULL;
    ge::gl::TextureObject* _integratedOffsetY = NULL;
    ge::gl::ProgramObject* _integrateOffsetProgram = NULL;
    ge::gl::ProgramObject* _smoothUsingIntegratedOffsetProgram = NULL;

    //7. unwarpX
    ge::gl::ProgramObject* _unwarpProgram = NULL;

    //8. tests
    ge::gl::ProgramObject* _drawGridProgram = NULL;

    ge::gl::TextureObject* _uall = NULL;
    ge::gl::ProgramObject* _uallProgram = NULL;

    ge::gl::ProgramObject* _createNVMapProgram = NULL;
    ge::gl::ProgramObject* _createNVMaskProgram = NULL;

    glm::mat4                  _lightProjection;
    glm::mat4                  _lightView;
    glm::mat4                  _bpv;

    void _computeViewSamples();
    void _fastCreateDV();
    void _fastCreateCountMap();
    void _integrate(
        ge::gl::TextureObject*integral,
        ge::gl::TextureObject*integralCount,
        ge::gl::TextureObject*countMap);
    void _createOffset(
        ge::gl::TextureObject*offset,
        ge::gl::TextureObject*integral,
        ge::gl::TextureObject*integralCount);
    void _smooth(
        ge::gl::TextureObject*smooth,
        ge::gl::TextureObject*offset,
        ge::gl::TextureObject*integralCount);

    void _integrateOffset(
        ge::gl::TextureObject*integratedOffset,
        ge::gl::TextureObject*offset);
    void _smoothUsingIntegratedOffset(
        ge::gl::TextureObject*smooth,
        ge::gl::TextureObject*integratedOffset);

    void _unwarp();
    void _unwarpAll();

    void _createNVMap();
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
    ge::gl::TextureObject*getCountMapX(){return this->_countMapX;}
    ge::gl::TextureObject*getCountMapY(){return this->_countMapY;}
    ge::gl::TextureObject*getIntegratedX(){return this->_integratedX;}
    ge::gl::TextureObject*getIntegratedY(){return this->_integratedY;}
    ge::gl::TextureObject*getOffsetX(){return this->_offsetX;}
    ge::gl::TextureObject*getOffsetY(){return this->_offsetY;}
    ge::gl::TextureObject*getSmoothX(){return this->_smoothX;}
    ge::gl::TextureObject*getSmoothY(){return this->_smoothY;}
    ge::gl::TextureObject*getViewSamples(){return this->_viewSamples;}
    void drawGrid(float x,float y,float sx,float sy);
    ge::gl::TextureObject*getuall(){return this->_uall;}

};

#endif//_NAVYMAPPING_HPP_
