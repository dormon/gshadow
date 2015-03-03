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


  this->_emptyVAO   = new ge::gl::VertexArrayObject();
  this->_mode       = TRIANGLES;
  this->_view       = glm::mat4(1.f);
  this->_projection = glm::mat4(1.f);
  this->_color      = glm::vec4(1.f);
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
  program->set("v",1,GL_FALSE,glm::value_ptr(this->_view));
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
