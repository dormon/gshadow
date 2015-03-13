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


    ge::gl::TextureObject* _viewSamples;
    ge::gl::ProgramObject* _viewSamplesProgram;

    ge::gl::ProgramObject* _fastdv0Program;
    ge::gl::ProgramObject* _fastdvProgram;
    std::vector<ge::gl::TextureObject*>_dvsTex;
    std::vector<glm::uvec2>_dvsWorkSize;


    ge::gl::TextureObject* _dvTex;
    ge::gl::ProgramObject* _dvProgram;
    ge::gl::ProgramObject* _dvClearProgram;
    ge::gl::TextureObject* _countMap;
    ge::gl::ProgramObject* _createCountMapProgram;
    ge::gl::TextureObject* _integratedCountMap;
    ge::gl::TextureObject* _integratedCountMapCount;
    ge::gl::ProgramObject* _integrateProgram;
    ge::gl::TextureObject* _isoX;
    ge::gl::ProgramObject* _createIsoProgram;

    void _computeViewSamples();
    void _fastCreateDV();
    void _createDV();
    void _createCountMap();
    void _integrate(
        ge::gl::TextureObject*integral,
        ge::gl::TextureObject*integralCount,
        ge::gl::TextureObject*countMap,
        bool xDirection);
    void _createIso(
        ge::gl::TextureObject*iso,
        ge::gl::TextureObject*integral,
        ge::gl::TextureObject*integralCount,
        bool xDirection);

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
    ge::gl::TextureObject*getCountMap(){return this->_countMap;}
    ge::gl::TextureObject*getIntegratedCountMap(){return this->_integratedCountMap;}
    ge::gl::TextureObject*getViewSamples(){return this->_viewSamples;}

};





/*
#define GLM_FORCE_RADIANS
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>

#include<geGL/VertexArrayObject.h>
#include<geGL/BufferObject.h>
#include<geGL/TextureObject.h>
#include<geGL/FramebufferObject.h>

#include"../../app/adjacency.hpp"
#include"../../simulationData/SimulationData.h"

#include"shaderdir.hpp"

//1. positionMap
//2. countmap
//3. prefixSumXMap
//4. isolinesX
//5. unwarpXCountSwapXYMap
//6. prefixSumY map
//7. isolinesY
//
// countMap                = writeViewSamples(positionMap          )
// prefixSumXMap,isolinesX = prefixSum       (countMap             )
// unwarpXCountSwapXYMap   = unwarpX         (prefixSumXMap        )
// prefixSumYMap,isolinesY = prefixSum       (unwarpXCountSwapXYMap)
//
//
//
//

class NavyMapping
{
  private:
    ge::gl::ProgramObject*SM;
    ge::gl::ProgramObject*NSM;
    ge::gl::ProgramObject*CSM;
    ge::gl::ProgramObject*CNM;
    ge::gl::ProgramObject*DSM;
    ge::gl::ProgramObject*WCM;
    ge::gl::ProgramObject*IntegrateCountMapProgram;
    ge::gl::TextureObject*shadowMap;
    ge::gl::TextureObject*navyMap;
    ge::gl::TextureObject*prefixSumXMap;
    ge::gl::TextureObject*prefixSumYMap;
    ge::gl::TextureObject*gridX;
    ge::gl::TextureObject*gridY;
    ge::gl::ProgramObject*prefixSumXProgram;
    ge::gl::ProgramObject*prefixSumYProgram;
    ge::gl::TextureObject*smoothedGridX;
    ge::gl::TextureObject*smoothedGridY;
    ge::gl::TextureObject*countMap;
    ge::gl::ProgramObject*smoothGridProgram;
    ge::gl::ProgramObject*DPSM;
    ge::gl::ProgramObject*drawGridProgram;
    ge::gl::TextureObject*integratedCountMap;
    ge::gl::TextureObject*unwarpXCountMap;
    ge::gl::ProgramObject*unwarpXProgram;
    ge::gl::FramebufferObject*fbo;
    ge::gl::FramebufferObject*nfbo;
    ge::gl::ProgramObject*prefixSumProgram;
		unsigned ShadowMapSize;//size of shadow map
    unsigned ScreenSize[2];
    SAdjecency*Adjacency;
    ge::gl::VertexArrayObject*sceneVAO;
    GLuint EmptyVAO;
    glm::mat4 B;
    glm::mat4 _bpv;
  public:
    void setMatrices(glm::mat4 lp,glm::mat4 lv);

    ge::gl::TextureObject*getCountMap();
    glm::mat4 _lightProjection;
    glm::mat4 _lightView;

    void prefixSum (ge::gl::TextureObject*grid,ge::gl::TextureObject*prefix,ge::gl::TextureObject*count);
    void prefixSum (bool xAxis);
    void smoothGrid(ge::gl::TextureObject*smooth,ge::gl::TextureObject*grid);
    void smoothGrid(bool xAxis);
    void drawGrid(float x,float y,float w,bool xAxe);
    NavyMapping(unsigned Size,unsigned*ScreenSize,ge::gl::VertexArrayObject*sceneVAO,SAdjecency*Adjacency);
    ~NavyMapping();
    void setFocus(simulation::Light*Light,float*FocusPoint,float Fovy);
    void createShadowMap(float*M);
    void createNavyMap(float*M);
    void drawShadowed(float*CameraPosition,simulation::Light*Light);
    void drawNavyShadowed(bool wrap,float*cameraPosition,simulation::Light*light);
    void drawShadowMap(bool navyMap,float x,float y,float w);
    void drawCountMap(float x,float y,float w);
    void drawIntegratedCountMap(float x,float y,float w);

    void writeViewSamples(GLuint positionTexture);
    void integrateCountMap();
    void prefixSum();
    void drawPrefixSum(float x,float y,float w);
    void smoothGridX();
    void drawGridX(float x,float y,float w);
    void unwarpX(GLuint positionTexture);
    void prefixSumY();
};
*/
#endif//_NAVYMAPPING_HPP_
