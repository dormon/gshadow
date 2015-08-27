#pragma once

#include<geGL/geGL.h>

class IntegrateOffset{
  protected:
    std::string           _dir             ;
    ge::gl::TextureObject*_integratedOffset = NULL;//output
    ge::gl::TextureObject*_offset           = NULL;//input
    unsigned              _resolution      ;
    unsigned              _wgsx            ;
    ge::gl::ProgramObject*_program          = NULL;
  public:
    IntegrateOffset(
        std::string           dir             ,
        ge::gl::TextureObject*integratedOffset,
        ge::gl::TextureObject*offset          ,
        unsigned              resolution      ,
        unsigned              wgsx            );
    ~IntegrateOffset();
    void setIntegratedOffet(ge::gl::TextureObject*integratedOffset);
    void setOffset         (ge::gl::TextureObject*offset          );
    void setResolution     (unsigned resolution                   );
    void operator()();
};
