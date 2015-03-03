#version 430

layout(location=0)out vec4 fColor;
layout(binding=9)uniform sampler2D depth;

in vec2 gCoord;

ivec2 coord=ivec2(gl_FragCoord.xy);

float linearize(float d,float n,float f){
  //return 2*n/(f+n+d*(f-n));
  //return 2*f*n/((d+1)*n+(1-d)*f);
  return -2*n/(d-1);
}

void main(){
  fColor=vec4(linearize(texture(depth,gCoord*.5+.5).r,.1,1000));
}
