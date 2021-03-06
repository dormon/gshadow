#pragma once

#include"../ShadowMethod.h"
#include"../ShadowMethodMacro.h"

#include"../NavyMapping/CreateWarping.h"
#include"../NavyMapping/CreateNavyShadowMap.h"
#include"../NavyMapping/UnwarpAll.h"

class NavyDualParaboloid: public simulation::SimulationObject, public ShadowMethod
{
  private:
    ge::gl::TextureObject*     _shadowMask       = NULL;
    ge::gl::FramebufferObject* _shadowMaskFBO    = NULL;
    ge::gl::ProgramObject*     _createShadowMask = NULL;


    ge::gl::TextureObject*     _shadowMap[2]     = {NULL,NULL};
    ge::gl::TextureObject*_desiredView[2] = {NULL,NULL};
    ge::gl::TextureObject*_smoothX    [2] = {NULL,NULL};
    ge::gl::TextureObject*_smoothY    [2] = {NULL,NULL};
    glm::mat4                  _lightView[2];

    ge::gl::VertexArrayObject* _emptyVAO         = NULL;

    CreateWarping*       _createWarping       = NULL;
    CreateNavyShadowMap* _createNavyShadowMap = NULL;


    bool _computeVisualisation = false;
    UnwarpAll*            _unwarpAll         = NULL;
    ge::gl::TextureObject*_countMap[2]       = {NULL,NULL};
    ge::gl::TextureObject*_warpedCountMap[2] = {NULL,NULL};


    DEF_UPDATEDATA(NavyDualParaboloid);
  public:
    void createShadowMap();
    void _createShadowMap();
    void _computeMatrices();
    void _createShadowMaskFBO();
    NavyDualParaboloid(simulation::SimulationData*data);
    ~NavyDualParaboloid();
    void createShadowMask();
    void createShadowMask(GLuint mask);
    unsigned getNofDependentVariables();
    std::string getDependentVariable(unsigned id);
    virtual void update();
    ge::gl::TextureObject*getShadowMap(unsigned i){
      return this->_shadowMap[i];
    }
    void setComputeVisualisation(bool vis){this->_computeVisualisation = vis;}
    ge::gl::TextureObject*getCountMap      (unsigned i){return this->_countMap      [i];}
    ge::gl::TextureObject*getWarpedCountMap(unsigned i){return this->_warpedCountMap[i];}
    ge::gl::TextureObject*getSmoothX(unsigned i){return this->_smoothX[i];}
    ge::gl::TextureObject*getSmoothY(unsigned i){return this->_smoothY[i];}

};




