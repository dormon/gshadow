#version 440 core

uniform vec4 color=vec4(1,1,1,1);

layout(location=0)out vec4 fColor;

in vec4 normal;

void main(){
  if(color.w>1)fColor=vec4(normal.xyz*.5+.5,normal.w);
  else fColor=color;
  //if(gl_FrontFacing)fColor=vec4(0,1,0,1);
  //else fColor=vec4(1,0,0,1);
}
