#ifndef _DRAWPRIMITIVE_H_
#define _DRAWPRIMITIVE_H_

#define GLM_FORCE_RADIANS
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>

#include<geGL/geGL.h>

class DrawPrimitive
{
  private:
    unsigned  _windowSize[2];
    glm::mat4 _view;
    glm::mat4 _projection;
    glm::vec4 _color;
    ge::gl::ProgramObject     *_drawTriangleProgram;
    ge::gl::ProgramObject     *_drawLineProgram;
    ge::gl::ProgramObject     *_drawPointProgram;
    ge::gl::ProgramObject     *_drawPlaneProgram;
    ge::gl::ProgramObject     *_drawTexture;
    ge::gl::ProgramObject     *_drawDepth;
    ge::gl::ProgramObject     *_drawCubeDepth;
    ge::gl::ProgramObject     *_drawHeatF;
    ge::gl::ProgramObject     *_drawHeatU;
    ge::gl::ProgramObject     *_draw1DF;
    ge::gl::VertexArrayObject *_emptyVAO;
    enum{
      TRIANGLES,
      LINES,
      POINTS,
      PLANE
    }_mode;
    ge::gl::ProgramObject*_getActiveProgram();
    void _setStateSet();
    void _setViewPort(float x,float y,float sx,float sy);
    void _resetViewPort();
  public:
    enum LinMet{
      STANDARD=0,
      DP=1,
      NEITHY=2,
      LOG=3
    };
    DrawPrimitive(std::string shaderDir);
    void setWindowSize(unsigned*size);
    void setWindowSize(glm::uvec2 size);
    void beginTriangles();
    void beginLine();
    void beginPoint();
    void beginPlanes();
    void end();
    void setView(glm::mat4 view);
    void setProjection(glm::mat4 projection);
    void setColor(float r,float g,float b,float a);
    void setColor(glm::vec4 color);
    void triangle(glm::vec3 a,glm::vec3 b,glm::vec3 c);
    void line(glm::vec3 a,glm::vec3 b);
    void point(glm::vec3 a,float size);
    void plane(glm::vec4 a,float size);
    void drawTexture(GLuint id,float x,float y,float sx,float sy);
    void drawDepth  (GLuint id,float x,float y,float sx,float sy,
        float near,float far,LinMet met=STANDARD);
    void drawCubeDepth  (GLuint id,float x,float y,float sx,float sy,
        float near,float far,int face,LinMet met=STANDARD);

    void drawHeatMap(GLuint id,float x,float y,float sx,float sy,
        unsigned min=0,unsigned max=255,int trans=0,unsigned channel=0);
    void drawHeatMap(GLuint id,float x,float y,float sx,float sy,
        float min=0.f,float max=1.f,int trans=0,unsigned channel=0);
    void draw1D(GLuint id,float x,float y,float sx,float sy,float min=0.f,float max=1.f,unsigned channel=0);
};

#endif//_DRAWPRIMITIVE_H_
