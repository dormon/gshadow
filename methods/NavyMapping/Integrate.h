#pragma once

#include<geGL/geGL.h>

class Integrate{
  protected:
    std::string _dir;
    ge::gl::ProgramObject*_program       = NULL;
    ge::gl::TextureObject*_integral      = NULL;
    ge::gl::TextureObject*_integralCount = NULL;
    ge::gl::TextureObject*_countMap      = NULL;
    unsigned _resolution;
    unsigned _wgsx;

  public:
    Integrate(
        std::string _dir,
        ge::gl::TextureObject*_integral,
        ge::gl::TextureObject*_integralCount,
        ge::gl::TextureObject*_countMap,
        unsigned _resolution,
        unsigned _wgsx);
    ~Integrate();
    void setResolution(unsigned resolution);
    void setIntegral     (ge::gl::TextureObject*integral     );
    void setIntegralCount(ge::gl::TextureObject*integralCount);
    void setCountMap     (ge::gl::TextureObject*countMap   );
    void operator()();
};
