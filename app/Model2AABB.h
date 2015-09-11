#pragma once

#include<geCore/Command.h>
#include<geGL/geGL.h>
#include"simpleLoader.h"

class Model2AABB: public ge::core::Command{
  protected:
    ge::gl::BufferObject**_aabb ;
    ModelPN*              _model;
  public:
    Model2AABB(ge::gl::BufferObject**aabb,ModelPN*model);
    void operator()();
};

