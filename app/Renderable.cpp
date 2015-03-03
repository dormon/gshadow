#include"Renderable.h"

void RenderState::set(){
  if(this->_fbo)    this->_fbo    ->bind();
  if(this->_vao)    this->_vao    ->bind();
  if(this->_program)this->_program->use ();
}

void RenderState::reset(){
  if(this->_fbo)    this->_fbo    ->unbind();
  if(this->_vao)    this->_vao    ->unbind();
}
