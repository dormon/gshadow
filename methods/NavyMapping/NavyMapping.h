#ifndef _NAVYMAPPING_HPP_
#define _NAVYMAPPING_HPP_

#include<glm/glm.hpp>
#define GLM_FORCE_RADIANS
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
    ge::gl::ProgramObject*DCM;
    ge::gl::ProgramObject*WCM;
    ge::gl::ProgramObject*IntegrateCountMapProgram;
    ge::gl::TextureObject*shadowMap;
    ge::gl::TextureObject*navyMap;
    ge::gl::TextureObject*countMap;
    ge::gl::TextureObject*prefixSumXMap;
    ge::gl::TextureObject*prefixSumYMap;
    ge::gl::TextureObject*gridX;
    ge::gl::TextureObject*gridY;
    ge::gl::ProgramObject*prefixSumXProgram;
    ge::gl::ProgramObject*prefixSumYProgram;
    ge::gl::TextureObject*smoothedGridX;
    ge::gl::TextureObject*smoothedGridY;
    ge::gl::ProgramObject*smoothGridProgram;
    ge::gl::ProgramObject*DPSM;
    ge::gl::ProgramObject*drawGridProgram;
    ge::gl::TextureObject*integratedCountMap;
    ge::gl::TextureObject*unwarpXCountMap;
    ge::gl::ProgramObject*unwarpXProgram;
    ge::gl::ProgramObject*drawUnwarpXProgram;
    ge::gl::FramebufferObject*fbo;
    ge::gl::FramebufferObject*nfbo;
    ge::gl::ProgramObject*prefixSumProgram;
		unsigned ShadowMapSize;//size of shadow map
    unsigned ScreenSize[2];
    SAdjecency*Adjacency;
    ge::gl::VertexArrayObject*sceneVAO;
    GLuint EmptyVAO;
    glm::mat4 B;
    glm::mat4 BPV;
  public:
    glm::mat4 LightProjection;
    glm::mat4 LightView;

    void minProjection(glm::mat4 p,glm::mat4 v,glm::mat4 lp,glm::mat4 lv);
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
    void drawUnwarpX(float x,float y,float w);
    void prefixSumY();
};

#endif//_NAVYMAPPING_HPP_
