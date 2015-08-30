#version 440 core

layout(location=0)out vec4 fColor;

in float z;

void main(){
  gl_FragDepth=z;
  fColor=vec4(1);
}
