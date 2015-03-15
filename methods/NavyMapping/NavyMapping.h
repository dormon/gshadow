#ifndef _NAVYMAPPING_HPP_
#define _NAVYMAPPING_HPP_

#include"../ShadowMethod.h"

class NavyMapping: public simulation::SimulationObject, public ShadowMethod
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


    //1. viewMap
    ge::gl::TextureObject* _viewSamples;
    ge::gl::ProgramObject* _viewSamplesProgram;

    //2. desired view
    ge::gl::ProgramObject* _fastdv0Program;
    ge::gl::ProgramObject* _fastdvProgram;
    std::vector<ge::gl::TextureObject*>_dvsTex;
    std::vector<glm::uvec2>_dvsWorkSize;

    //3. countMap
    ge::gl::TextureObject* _countMapX;
    ge::gl::TextureObject* _countMapY;
    ge::gl::ProgramObject* _fastCreateCountMapProgram;

    //4. integratedCountMap
    ge::gl::TextureObject* _integratedX;
    ge::gl::TextureObject* _integratedY;
    ge::gl::TextureObject* _integratedXCount;
    ge::gl::TextureObject* _integratedYCount;
    ge::gl::ProgramObject* _integrateProgram;

    //5. offsetX
    ge::gl::TextureObject* _offsetX;
    ge::gl::TextureObject* _offsetY;
    ge::gl::ProgramObject* _offsetProgram;

    //6. smoothX
    ge::gl::TextureObject* _smoothX;
    ge::gl::TextureObject* _smoothY;
    ge::gl::ProgramObject* _smoothProgram;

    //7. unwarpX
    ge::gl::ProgramObject* _unwarpProgram;

    //8. tests
    ge::gl::ProgramObject* _drawGridProgram;

    ge::gl::TextureObject* _uall;
    ge::gl::ProgramObject* _uallProgram;

    ge::gl::ProgramObject* _createNVMapProgram;
    ge::gl::ProgramObject* _createNVMaskProgram;


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
    void _unwarp();
    void _unwarpAll();

    void _createNVMap();
    void _createNVMask();

    void _createShadowMap();
    void _computeMatrices();
    void _createShadowMapFBO();
    void CreateShadowMap();
    unsigned _getDiv(unsigned a,unsigned b){return a%b?a/b+1:a/b;}
  public:
    ge::gl::TextureObject*getShadowMap();
    void setMatrices(glm::mat4 lp,glm::mat4 lv);
    void createShadowMask();
    void createShadowMask(GLuint mask);
    unsigned getNofDependentVariables();
    std::string getDependentVariable(unsigned id);
    void update();
    NavyMapping(simulation::SimulationData*data);
    ~NavyMapping();
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
