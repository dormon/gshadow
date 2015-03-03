#version 430

#ifndef DRAWCM_BINDING_COUNTMAP
#define DRAWCM_BINDING_COUNTMAP 0
#endif//DRAWCM_BINDING_COUNTMAP

layout(location=0)out vec4 fColor;
layout(r32ui,binding=DRAWCM_BINDING_COUNTMAP)uniform uimage2D countMap;

in vec2 gCoord;

uniform uint shadowMapSize;

vec4 getColor(uint s){
  float t=s/100;
  uint steps=10;
  vec4 acol;
  vec4 bcol;
  if(s<steps*1){
    t=float(s)/steps;
    acol=vec4(0,0,0,1);
    bcol=vec4(0,0,1,1);
  }else if(s<steps*2){
    t=float(s-steps*1)/steps;
    acol=vec4(0,0,1,1);
    bcol=vec4(0,1,0,1);
  }else if(s<steps*3){
    t=float(s-steps*2)/steps;
    acol=vec4(0,1,0,1);
    bcol=vec4(1,0,0,1);
  }else if(s<steps*4){
    t=float(s-steps*3)/steps;
    acol=vec4(1,0,0,1);
    bcol=vec4(1,1,0,1);
  }else{
    t=float(s-steps*4)/steps;
    acol=vec4(1,1,0,1);
    bcol=vec4(1,1,1,1);
  }
  return mix(acol,bcol,t);
}

void main(){
  uint sampleCount=imageLoad(countMap,ivec2((gCoord*.5+.5)*shadowMapSize)).x;
  fColor=getColor(sampleCount);
  //fColor=vec4(1,0,0,1);
  //fColor=vec4(gCoord*.5+.5,0,1);
}
