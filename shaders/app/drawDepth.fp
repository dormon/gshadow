#version 440 core

layout(location=0)out vec4 fColor;
layout(binding=0)uniform sampler2D tex;

in vec2 gCoord;

uniform float near=0.1f;
uniform float far=100.f;

void main(){
  float val=texture(tex,gCoord).x;
  fColor=vec4((1-val)*50);
}
