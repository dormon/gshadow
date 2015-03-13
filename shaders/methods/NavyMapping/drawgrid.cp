#version 440 core

layout(vertices=1)out;

void main(){
  float tessFactor=32;
  gl_TessLevelOuter[0]=tessFactor;
  gl_TessLevelOuter[1]=tessFactor;
  gl_TessLevelOuter[2]=tessFactor;
  gl_TessLevelOuter[3]=tessFactor;
  gl_TessLevelInner[0]=tessFactor;
  gl_TessLevelInner[1]=tessFactor;
}
