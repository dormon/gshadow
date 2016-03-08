#include"drawPrimitive.h"

DrawPrimitive::DrawPrimitive(std::string shaderDir){
  this->_drawTriangleProgram = new ge::gl::ProgramObject(
      shaderDir+"drawTriangle.vp",
      shaderDir+"drawTriangle.gp",
      shaderDir+"drawTriangle.fp");
  this->_drawPointProgram = new ge::gl::ProgramObject(
      shaderDir+"drawPoint.vp",
      shaderDir+"drawPoint.gp",
      shaderDir+"drawPoint.fp");
  this->_drawLineProgram = new ge::gl::ProgramObject(
      shaderDir+"drawLine.vp",
      shaderDir+"drawLine.gp",
      shaderDir+"drawLine.fp");
  this->_drawPlaneProgram = new ge::gl::ProgramObject(
      shaderDir+"drawPlane.vp",
      shaderDir+"drawPlane.gp",
      shaderDir+"drawPlane.fp");
  this->_drawTexture = new ge::gl::ProgramObject(
      shaderDir+"drawTexture.vp",
      shaderDir+"drawTexture.gp",
      shaderDir+"drawTexture.fp");
  this->_drawHeatF = new ge::gl::ProgramObject(
      shaderDir+"drawTexture.vp",
      shaderDir+"drawTexture.gp",
      shaderDir+"drawHeat.fp",
      ge::gl::ShaderObject::include(shaderDir+"heatFunction.vp"));
  this->_drawHeatU = new ge::gl::ProgramObject(
      shaderDir+"drawTexture.vp",
      shaderDir+"drawTexture.gp",
      shaderDir+"drawHeatU.fp",
      ge::gl::ShaderObject::include(shaderDir+"heatFunction.vp"));
  this->_draw1DF = new ge::gl::ProgramObject(
      shaderDir+"drawTexture.vp",
      shaderDir+"drawTexture.gp",
      shaderDir+"draw1DF.fp",
      ge::gl::ShaderObject::include(shaderDir+"heatFunction.vp"));
  this->_drawDepth = new ge::gl::ProgramObject(
      shaderDir+"drawTexture.vp",
      shaderDir+"drawTexture.gp",
      shaderDir+"drawDepth.fp");

  this->_drawCubeDepth = new ge::gl::ProgramObject(
      shaderDir+"drawTexture.vp",
      shaderDir+"drawTexture.gp",
      shaderDir+"drawCubeDepth.fp");


  this->_emptyVAO   = new ge::gl::VertexArrayObject();
  this->_mode       = TRIANGLES;
  this->_view       = glm::mat4(1.f);
  this->_projection = glm::mat4(1.f);
  this->_color      = glm::vec4(1.f);
}

void DrawPrimitive::setWindowSize(unsigned*size){
  this->_windowSize[0]=size[0];
  this->_windowSize[1]=size[1];
}

void DrawPrimitive::setWindowSize(glm::uvec2 size){
  this->_windowSize[0]=size[0];
  this->_windowSize[1]=size[1];
}


void DrawPrimitive::beginTriangles(){
  this->_mode=TRIANGLES;
  this->_setStateSet();
}

void DrawPrimitive::beginLine(){
  this->_mode=LINES;
  this->_setStateSet();
}

void DrawPrimitive::beginPoint(){
  this->_mode=POINTS;
  this->_setStateSet();
}

void DrawPrimitive::beginPlanes(){
  this->_mode=PLANE;
  this->_setStateSet();
}

void DrawPrimitive::end(){
  this->_emptyVAO->unbind();
}

ge::gl::ProgramObject*DrawPrimitive::_getActiveProgram(){
  switch(this->_mode){
    case TRIANGLES:return this->_drawTriangleProgram;
    case LINES    :return this->_drawLineProgram    ;
    case POINTS   :return this->_drawPointProgram   ;
    case PLANE    :return this->_drawPlaneProgram   ;
    default       :return this->_drawTriangleProgram;
  }
}

void DrawPrimitive::_setStateSet(){
  this->_getActiveProgram()->use();
  this->setView      (this->_view      );
  this->setProjection(this->_projection);
  this->setColor     (this->_color     );
  this->_emptyVAO->bind();
}

void DrawPrimitive::setView(glm::mat4 view){
  this->_view=view;
  ge::gl::ProgramObject*program=this->_getActiveProgram();
  program->set("v",1,GL_FALSE,(const float*)glm::value_ptr(this->_view));
}

void DrawPrimitive::setProjection(glm::mat4 projection){
  this->_projection=projection;
  ge::gl::ProgramObject*program=this->_getActiveProgram();
  program->set("p",1,GL_FALSE,glm::value_ptr(this->_projection));
}

void DrawPrimitive::setColor(float r,float g,float b,float a){
  this->setColor(glm::vec4(r,g,b,a));
}

void DrawPrimitive::setColor(glm::vec4 color){
  this->_color=color;
  ge::gl::ProgramObject*program=this->_getActiveProgram();
  program->set("color",1,glm::value_ptr(this->_color));
}

void DrawPrimitive::triangle(glm::vec3 a,glm::vec3 b,glm::vec3 c){
  this->_drawTriangleProgram->set("a",1,glm::value_ptr(a));
  this->_drawTriangleProgram->set("b",1,glm::value_ptr(b));
  this->_drawTriangleProgram->set("c",1,glm::value_ptr(c));
  glDrawArrays(GL_POINTS,0,1);
}

void DrawPrimitive::line(glm::vec3 a,glm::vec3 b){
  this->_drawLineProgram->set("a",1,glm::value_ptr(a));
  this->_drawLineProgram->set("b",1,glm::value_ptr(b));
  glDrawArrays(GL_POINTS,0,1);
}

void DrawPrimitive::point(glm::vec3 a,float size){
  this->_drawPointProgram->set("a",1,glm::value_ptr(a));
  this->_drawPointProgram->set("size",size);
  glDrawArrays(GL_POINTS,0,1);
}

void DrawPrimitive::plane(glm::vec4 a,float size){
  this->_drawPlaneProgram->set("a",1,glm::value_ptr(a));
  this->_drawPlaneProgram->set("size",size);
  glDrawArrays(GL_POINTS,0,1);
}

void DrawPrimitive::_setViewPort(float x,float y,float sx,float sy){
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glViewport(
      x *this->_windowSize[0],
      y *this->_windowSize[1],
      sx*this->_windowSize[0],
      sy*this->_windowSize[1]);
  this->_emptyVAO->bind();
}

void DrawPrimitive::_resetViewPort(){
  glDrawArrays(GL_POINTS,0,1);
  this->_emptyVAO->unbind();
  glViewport(0,0,this->_windowSize[0],this->_windowSize[1]);
}

void DrawPrimitive::drawTexture(GLuint id,float x,float y,float sx,float sy){
  this->_setViewPort(x,y,sx,sy);

  this->_drawTexture->use();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,id);

  this->_resetViewPort();
}

void DrawPrimitive::drawHeatMap(GLuint id,float x,float y,float sx,float sy,
    float min,float max,int trans,unsigned channel){
  this->_setViewPort(x,y,sx,sy);

  this->_drawHeatF->use();
  this->_drawHeatF->set("minValue",min);
  this->_drawHeatF->set("maxValue",max);
  this->_drawHeatF->set("channel",channel);
  this->_drawHeatF->set("transId",trans);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,id);

  this->_resetViewPort();
}

void DrawPrimitive::drawHeatMap(GLuint id,float x,float y,float sx,float sy,
    unsigned min,unsigned max,int trans,unsigned channel){
  this->_setViewPort(x,y,sx,sy);

  this->_drawHeatU->use();
  this->_drawHeatU->set("minValue",min);
  this->_drawHeatU->set("maxValue",max);
  this->_drawHeatU->set("channel",channel);
  this->_drawHeatU->set("transId",trans);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,id);

  this->_resetViewPort();
}

void DrawPrimitive::draw1D(GLuint id,float x,float y,float sx,float sy,
    float min,float max,unsigned channel){
  this->_setViewPort(x,y,sx,sy);

  this->_draw1DF->use();
  this->_draw1DF->set("size",.8/(sy*this->_windowSize[1]));
  this->_draw1DF->set("minValue",min);
  this->_draw1DF->set("maxValue",max);
  this->_draw1DF->set("channel",channel);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_1D,id);

  this->_resetViewPort();
}

void DrawPrimitive::drawDepth  (GLuint id,float x,float y,float sx,float sy,
        float near,float far,LinMet met){
  this->_setViewPort(x,y,sx,sy);
  this->_drawDepth->use();
  this->_drawDepth->set("near",near);
  this->_drawDepth->set("far",far);
  this->_drawDepth->set("linMet",(int)met);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,id);
  GLint last;
  glGetTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,&last);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,GL_NONE);
  this->_resetViewPort();
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,last);

}
void DrawPrimitive::drawCubeDepth  (GLuint id,float x,float y,float sx,float sy,
    float near,float far,int face,LinMet met){
  this->_setViewPort(x,y,sx,sy);

  this->_drawCubeDepth->use();
  this->_drawCubeDepth->set("near",near);
  this->_drawCubeDepth->set("far",far);
  this->_drawCubeDepth->set("face",face);
  this->_drawCubeDepth->set("linMet",(int)met);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP,id);
  GLint last;
  glGetTexParameteriv(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_COMPARE_MODE,&last);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_COMPARE_MODE,GL_NONE);
  this->_resetViewPort();
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_COMPARE_MODE,last);

}

