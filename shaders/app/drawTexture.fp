#version 440 core

layout(location=0)out vec4 fColor;
layout(binding=0)uniform sampler2D tex;

in vec2 gCoord;

void main(){
  fColor=texture(tex,gCoord);
}
