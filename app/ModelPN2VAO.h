#pragma once

#include<geCore/Command.h>
#include<geGL/geGL.h>
#include"simpleLoader.h"

class ModelPN2VAO: public ge::core::Command{
  protected:
    ge::gl::VertexArrayObject**_vao    ;
    ge::gl::BufferObject**     _vbo    ;
    ModelPN*                   _model  ;
  public:
    ModelPN2VAO(ge::gl::VertexArrayObject**_vao,ge::gl::BufferObject**_vbo,ModelPN*model);
    void operator()();
};
