#version 430

#ifndef WRITECM_BINDING_POSITIONMAP
#define WRITECM_BINDING_POSITIONMAP 0
#endif//WRITECM_BINDING_POSITIONMAP

#ifndef WRITECM_BINDING_COUNTMAP
#define WRITECM_BINDING_COUNTMAP 1
#endif//WRITECM_BINDING_COUNTMAP


layout(location=0)out vec4 fColor;
layout(binding=WRITECM_BINDING_POSITIONMAP)uniform sampler2D positionMap;
layout(r32ui,binding=WRITECM_BINDING_COUNTMAP)uniform uimage2D countMap;

in vec2 gCoord;
ivec2 Coord=ivec2(gl_FragCoord.xy);

uniform mat4 BPV;
uniform uint shadowMapSize;

void main(){
  if(all(isnan(texelFetch(positionMap,Coord,0))))return;
	vec4 shadowMapCoord = BPV*vec4(texelFetch(positionMap,Coord,0).xyz,1);
  imageAtomicAdd(countMap,ivec2(shadowMapCoord.xy/shadowMapCoord.w*shadowMapSize),1);
  //imageAtomicAdd(countMap,ivec2(Coord.xy),100);
  fColor=vec4(1);
}
