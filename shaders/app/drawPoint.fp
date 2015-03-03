#version 440 core

uniform vec4 color=vec4(1,1,1,1);

layout(location=0)out vec4 fColor;

void main(){
  fColor=color;
}
