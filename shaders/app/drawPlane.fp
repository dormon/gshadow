#version 440 core

uniform vec4 color=vec4(1,1,1,1);

layout(location=0)out vec4 fColor;

void main(){
  if(gl_FrontFacing)fColor=vec4(0,1,0,color.w);
  else fColor=vec4(1,0,0,color.w);
}
