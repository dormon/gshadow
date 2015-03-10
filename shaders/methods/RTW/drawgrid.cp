#version 440 core

layout(vertices=1)out;

void main(){
  gl_TessLevelOuter[0]=16;
  gl_TessLevelOuter[1]=16;
  gl_TessLevelOuter[2]=16;
  gl_TessLevelOuter[3]=16;
  gl_TessLevelInner[0]=16;
  gl_TessLevelInner[1]=16;
}
