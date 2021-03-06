#version 440 core

layout(location=0)out vec4 fColor;
layout(binding=0)uniform sampler2D tex;

in vec2 gCoord;

uniform float near=0.1f;
uniform float far=100.f;

uniform int linMet=0;

float linearize(float d){
  return 2.f*far*near/(far+near-(2.f*d-1.f)*(far-near));
}

void main(){
  float val=texture(tex,gCoord).x;

  if(linMet==0)fColor=vec4((linearize(val)-near)/(far-near));
  if(linMet==1)fColor=vec4(val);
  if(linMet==2)fColor=vec4((1-val)*50);
  if(linMet==3)fColor=vec4(log(linearize(val))*10);

  //fColor=vec4((linearize(val)-near)/(far-near));
}
