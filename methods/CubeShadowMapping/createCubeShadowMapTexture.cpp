#include"createCubeShadowMapTexture.h"

#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>

ge::gl::TextureObject*createCubeShadowMapTexture(unsigned resolution){
  ge::gl::TextureObject*shadowMap = new ge::gl::TextureObject(
      GL_TEXTURE_CUBE_MAP,
      GL_DEPTH_COMPONENT24,
      1,
      resolution,
      resolution);
  float ones[]={1,1,1,1};
  shadowMap->texParameteri (GL_TEXTURE_MIN_FILTER  ,GL_NEAREST             );
  shadowMap->texParameteri (GL_TEXTURE_MAG_FILTER  ,GL_NEAREST             );
  shadowMap->texParameteri (GL_TEXTURE_WRAP_S      ,GL_CLAMP_TO_BORDER     );
  shadowMap->texParameteri (GL_TEXTURE_WRAP_T      ,GL_CLAMP_TO_BORDER     );
  shadowMap->texParameteri (GL_TEXTURE_COMPARE_FUNC,GL_LEQUAL              );
  shadowMap->texParameteri (GL_TEXTURE_COMPARE_MODE,GL_COMPARE_R_TO_TEXTURE);
  shadowMap->texParameterfv(GL_TEXTURE_BORDER_COLOR,ones                   );
  return shadowMap;
}

GLenum id2CubeMapSide(unsigned i){
  GLenum sides[]={
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z
  };
  return sides[i];
}

void computeCubeMapViewProjection(glm::mat4&projection,glm::mat4*view,float near,float far){
  //table 8.19
  //{x,y,z}
  const float axes[]={
    +0,+0,+1, +0,-1,+0, +1,+0,+0,
    +0,+0,-1, +0,-1,+0, -1,+0,+0,
    +1,+0,+0, +0,+0,+1, +0,-1,+0,
    +1,+0,+0, +0,+0,-1, +0,+1,+0,
    -1,+0,+0, +0,-1,+0, +0,+0,+1,
    +1,+0,+0, +0,-1,+0, +0,+0,-1, 
  };

  projection=glm::perspective(glm::pi<float>()/2.f,1.f,near,far);
  for(unsigned side=0;side<6u;++side){
    view[side]=glm::mat4(1.f);
    for(unsigned axis=0;axis<3;++axis)
      for(unsigned k=0;k<3;++k)
        view[side][axis][k]=axes[(side*3+axis)*3+k];
  }
}
