#pragma once

#include<geGL/geGL.h>
#include<glm/glm.hpp>

ge::gl::TextureObject*createCubeShadowMapTexture(unsigned resolution);

GLenum id2CubeMapSide(unsigned i);

void computeCubeMapViewProjection(glm::mat4&projection,glm::mat4*view,float near,float far);


