#include"copyTex.h"

ge::gl::ProgramObject*_copyTexProgram=nullptr;

void copyTexInit(std::string dir){
  _copyTexProgram = new ge::gl::ProgramObject(
      dir+"/app/copyTex.comp");
}

void copyTexDeinit(){
  delete _copyTexProgram;
}

void copyTex(ge::gl::TextureObject*output,ge::gl::TextureObject*input,unsigned x,unsigned y){
  _copyTexProgram->use();
  input ->bindImage(0);
  output->bindImage(1);
  unsigned workSizex=ge::core::getDispatchSize(x,8);
  unsigned workSizey=ge::core::getDispatchSize(y,8);
  glDispatchCompute(workSizex,workSizey,1);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}
