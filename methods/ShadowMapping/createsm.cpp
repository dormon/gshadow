#include"createsm.h"

#include<glm/gtc/matrix_transform.hpp>

ge::gl::TextureObject*createSM(unsigned resolution){
  ge::gl::TextureObject*shadowMap=NULL;
  float ones[]={1,1,1,1};
  shadowMap=new ge::gl::TextureObject(GL_TEXTURE_2D,GL_DEPTH_COMPONENT24,1,resolution,resolution);
  shadowMap->texParameteri (GL_TEXTURE_MIN_FILTER  ,GL_NEAREST             );
  shadowMap->texParameteri (GL_TEXTURE_MAG_FILTER  ,GL_NEAREST             );
  shadowMap->texParameteri (GL_TEXTURE_WRAP_S      ,GL_CLAMP_TO_BORDER     );
  shadowMap->texParameteri (GL_TEXTURE_WRAP_T      ,GL_CLAMP_TO_BORDER     );
  shadowMap->texParameteri (GL_TEXTURE_COMPARE_FUNC,GL_LEQUAL              );
  shadowMap->texParameteri (GL_TEXTURE_COMPARE_MODE,GL_COMPARE_R_TO_TEXTURE);
  shadowMap->texParameterfv(GL_TEXTURE_BORDER_COLOR,ones                   );
  return shadowMap;
}

glm::mat4 biasMatrix(){
  return glm::scale    (glm::mat4(1.f),glm::vec3(.5f))*
    glm::translate(glm::mat4(1.f),glm::vec3(1.f));
}
