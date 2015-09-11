#pragma once

#include<geCore/Command.h>
#include"geGL/geGL.h"
#include"../../app/fastAdjacency.h"

class AdjacencyToVBO: public ge::core::Command{
  protected:
    Adjacency*            _adjacency = nullptr;
    ge::gl::BufferObject**_vbo       = nullptr;
  public:
    AdjacencyToVBO(ge::gl::BufferObject**vbo,Adjacency*adjacency);
    void operator()();
};

