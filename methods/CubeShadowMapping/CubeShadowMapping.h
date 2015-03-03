#ifndef _CUBESHADOWMAPPING_H_
#define _CUBESHADOWMAPPING_H_

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

typedef struct{
  unsigned resolution;
  unsigned screenSize[2];
  float    near;
  float    far;
}CubeShadowMappingTemplate;

bool operator!=(const CubeShadowMappingTemplate&a,CubeShadowMappingTemplate&b);

class CubeShadowMapping
{
  private:
    ge::gl::ProgramObject     *_createProgram;
    ge::gl::ProgramObject     *_drawProgram;
    ge::gl::TextureObject     *_cubeShadowMap;
    ge::gl::FramebufferObject *_fbo[6];
    ge::gl::VertexArrayObject *_sceneVAO;
    ge::gl::VertexArrayObject *_emptyVAO;
    unsigned                   _resolution;
    unsigned                   _screenSize[2];
    SAdjecency                *_adjacency;
    glm::mat4                  _bpvMatrix[6];
    glm::mat4                  _lightViewMatrix[6];
    glm::mat4                  _lightProjectionMatrix;
    float                      _near;
    float                      _far;
  public:
    CubeShadowMapping(
        SAdjecency*adjacency,
        ge::gl::VertexArrayObject*sceneVAO,
        CubeShadowMappingTemplate Template
        );
    ~CubeShadowMapping();
    void createShadowMap(float*M,simulation::Light*light);
    void drawShadowed(float*pos,simulation::Light*light);

};


#endif//_CUBESHADOWMAPPING_H_
