#version 430

#define MY_PI 3.1415925

layout(location=0)out float fColor;

layout(binding=0)uniform sampler2D         position;
layout(binding=1)uniform samplerCubeShadow shadowMap;

uniform mat4 BPV[6];

uniform float near;
uniform float far;

ivec2 Coord=ivec2(gl_FragCoord.xy);

uniform vec4 lightPosition;
vec3 viewSamplePosition = texelFetch(position,Coord,0).xyz;

float linearize(float d){
  return 2.f*far*near/(far+near-(2.f*d-1.f)*(far-near));
}

float computeDepth(vec3 dir){
  vec3 adir=abs(dir);
  float baseDis=-max(max(adir.x,adir.y),adir.z);
  float A=-(far+near)/(far-near);
  float B=-2*far*near/(far-near);
  //d=(A*z+B*1)/(-1*z)
  //d=-A-B/z
  return (-A-B/baseDis)*.5+.5;
}

void main(){
  vec3 dir=viewSamplePosition-lightPosition.xyz;
  fColor=texture(shadowMap,vec4(dir,computeDepth(dir))).x;
}

