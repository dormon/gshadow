#version 440 core

layout(location=0)out vec4 fColor;
layout(binding=0)uniform sampler2D tex;

in vec2 gCoord;

uniform float near=0.1f;
uniform float far=100.f;

float linearize(float d){
  return 2.f*far*near/(far+near-(2.f*d-1.f)*(far-near));
}

void main(){
  float val=texture(tex,gCoord).x;
  fColor=vec4(log(linearize(val))*10);
}
