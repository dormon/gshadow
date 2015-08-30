#version 440 core
layout(location=0)out float fColor;

layout(binding=0)uniform sampler2D       position;
layout(binding=1)uniform sampler2DShadow shadowMap[2];
layout(binding=3)uniform sampler2D desiredView[2];
layout(binding=5)uniform sampler2D smoothX[2];
layout(binding=7)uniform sampler2D smoothY[2];

uniform mat4 v[2];
uniform float near;
uniform float far;
uniform vec4 lightPosition;

ivec2 screenCoord=ivec2(gl_FragCoord.xy);

#ifndef POLY_OFFSET
#define POLY_OFFSET 100
#endif//POLY_OFFSET

void main(){
  vec3 worldSpace=texelFetch(position,screenCoord,0).xyz;
  int face=int((worldSpace.x-lightPosition.x)>0);
  vec4 fin=dualParaboloidProjection(v[face],worldSpace,near,far+POLY_OFFSET);
  fin=nv_getFinal(fin,desiredView[face],smoothX[face],smoothY[face]);
  fin.xy*=.5;
  fin.xy+=.5;
  fColor=textureProj(shadowMap[face],fin).x;
}

