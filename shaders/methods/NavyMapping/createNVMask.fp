#version 440 core
layout(location=0)out float fColor;

layout(binding=0)uniform sampler2D       position;
layout(binding=1)uniform sampler2DShadow shadowMap;

uniform mat4 mvp;

ivec2 screenCoord=ivec2(gl_FragCoord.xy);

void main(){
  vec4 clip = mvp*vec4(texelFetch(position,screenCoord,0).xyz,1);
  vec4 warped=nv_getFinal(clip);

  warped.xyz=(warped.xyz+warped.www)/2.;
  fColor=textureProj(shadowMap,warped).x;
}
