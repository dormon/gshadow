#pragma once

#include<geGL/geGL.h>
#include<glm/glm.hpp>
#include<vector>

class CreateDesiredView{
  private:
    ge::gl::ProgramObject* _fastdv0Program = NULL;
    ge::gl::ProgramObject* _fastdvProgram  = NULL;
    std::vector<ge::gl::TextureObject*>_dvsTex;
    std::vector<glm::uvec2>_dvsWorkSize;
  protected:
    std::string _dir;
    unsigned _width;
    unsigned _height;
    ge::gl::TextureObject*_viewSamples;
    unsigned _wgsx;
    unsigned _wgsy;
  public:
    CreateDesiredView(
        std::string shaderDir,
        unsigned width ,
        unsigned height,
        ge::gl::TextureObject*viewSamples,
        ge::gl::TextureObject*desiredView,
        unsigned wgsx,
        unsigned wgsy);
    void setViewSamples(ge::gl::TextureObject*viewSamples);
    void setDesiredView(ge::gl::TextureObject*desiredView);
    ~CreateDesiredView();
    void operator()();
};
