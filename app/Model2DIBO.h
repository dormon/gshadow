#pragma once

#include<geCore/Command.h>
#include<geGL/geGL.h>
#include"simpleLoader.h"

class Model2DIBO: public ge::core::Command{
  protected:
    ge::gl::BufferObject**_dibo ;
    ModelPN*              _model;
  public:
    Model2DIBO(ge::gl::BufferObject**dibo,ModelPN*model);
    void operator()();
};

