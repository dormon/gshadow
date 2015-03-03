#version 440 core

#ifndef DRAWUNWARPX_BINDING_UNWARPXCOUNTMAP
#define DRAWUNWARPX_BINDING_UNWARPXCOUNTMAP 0
#endif//DRAWUNWARPX_BINDING_UNWARPXCOUNTMAP

#ifndef SHADOWMAP_SIZE
#define SHADOWMAP_SIZE 1024
#endif//SHADOWMAP_SIZE

layout(location=0)out vec4 fColor;
layout(r32ui,binding=DRAWUNWARPX_BINDING_UNWARPXCOUNTMAP)uniform uimage2D unwarpXCountMap;

in vec2 gCoord;

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

vec4 getColor(float t){
  t=clamp(t,0,1);
  vec4 colors[]={
    vec4(0,0,0,1),
    vec4(0,0,1,1),
    vec4(0,1,0,1),
    vec4(1,0,0,1),
    vec4(1,1,0,1),
    vec4(1,1,1,1)
  };
  t*=colors.length();
  uint i=uint(floor(t));
  return mix(colors[i],colors[i],fract(t));
}

void main(){
  uint sampleCount=imageLoad(unwarpXCountMap,ivec2((gCoord.yx*.5+.5)*SHADOWMAP_SIZE)).x;
  fColor=getColor(sampleCount);
  fColor=getColor(sampleCount/50.);
  /*if(sampleCount>1)fColor=vec4(1);
  else fColor=vec4(0);*/
  //fColor=vec4(1,0,0,1);
  //fColor=vec4(1,0,0,1);
  //fColor=vec4(gCoord*.5+.5,0,1);
}

