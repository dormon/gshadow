#version 440 core

#ifndef UNWARPX_BINDING_POSITIONMAP
#define UNWARPX_BINDING_POSITIONMAP 0
#endif//UNWARPX_BINDING_POSITIONMAP

#ifndef UNWARPX_BINDING_UNWARPXCOUNTMAP
#define UNWARPX_BINDING_UNWARPXCOUNTMAP 1
#endif//UNWARPX_BINDING_UNWARPXCOUNTMAP

#ifndef UNWARPX_BINDING_SMOOTHEDGRIDX
#define UNWARPX_BINDING_SMOOTHEDGRIDX 2
#endif//UNWARPX_BINDING_SMOOTHEDGRIDX

#ifndef SHADOWMAP_SIZE 
#define SHADOWMAP_SIZE 1024
#endif//SHADOWMAP_SIZE

#ifndef GRID_SIZE_X
#define GRID_SIZE_X 4
#endif//GRID_SIZE_X

layout(location=0)out vec4 fColor;
layout(      binding=UNWARPX_BINDING_POSITIONMAP    )uniform sampler2D positionMap;
layout(r32ui,binding=UNWARPX_BINDING_UNWARPXCOUNTMAP)uniform uimage2D  unwarpXCountMap;
layout(r32f ,binding=UNWARPX_BINDING_SMOOTHEDGRIDX  )uniform  image2D  smoothedGridX;


in vec2 gCoord;
ivec2 Coord=ivec2(gl_FragCoord.xy);

uniform mat4 BPV;

ivec2 unwarp(in vec2 coord){
  ivec2 result;
  result.y=int(coord.y);
  float gridPosition[GRID_SIZE_X+1];
  for(uint i=0;i<=GRID_SIZE_X;++i)
    gridPosition[i]=(imageLoad(smoothedGridX,ivec2(result.y,i)).r*.5+.5)*SHADOWMAP_SIZE;
  if(coord.x<gridPosition[0])coord.x=gridPosition[0];
  if(coord.x>gridPosition[GRID_SIZE_X])coord.x=gridPosition[GRID_SIZE_X];
  uint gridId=0;
  for(gridId;gridId<GRID_SIZE_X;++gridId){
    if(coord.x>=gridPosition[gridId])break;
  }
  float a=gridPosition[gridId];
  float b=gridPosition[gridId+1];
  float ta=float(gridId)/GRID_SIZE_X*SHADOWMAP_SIZE;
  float tb=float(gridId+1)/GRID_SIZE_X*SHADOWMAP_SIZE;
  float t=(coord.x-a)/(b-a);
  result.x=int(ta+(tb-ta)*t);
  return result.yx;//swap axes
}

void main(){
  if(all(isnan(texelFetch(positionMap,Coord,0))))return;
	vec4 shadowMapCoord = BPV*vec4(texelFetch(positionMap,Coord,0).xyz,1);
  imageAtomicAdd(unwarpXCountMap,unwarp(shadowMapCoord.xy/shadowMapCoord.w*SHADOWMAP_SIZE),1);
  fColor=vec4(1);
}
