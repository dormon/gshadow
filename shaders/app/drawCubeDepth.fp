#version 440 core

layout(location=0)out vec4 fColor;
layout(binding=0)uniform samplerCube tex;

in vec2 gCoord;

uniform float near=0.1f;
uniform float far=100.f;

uniform int linMet=0;

uniform int face=0;

float linearize(float d){
  return 2.f*far*near/(far+near-(2.f*d-1.f)*(far-near));
}

void main(){
  vec3 co=vec3(1,0,0);
  vec2 gC=gCoord*2-1;
  if(face==0)co=vec3(-1,gC.y,gC.x);
  if(face==1)co=vec3(+1,gC.y,-gC.x);
  if(face==2)co=vec3(gC.y,-1,-gC.x);
  if(face==3)co=vec3(-gC.y,+1,-gC.x);
  if(face==4)co=vec3(-gC.x,gC.y,-1);
  if(face==5)co=vec3(gC.x,gC.y,+1);


  float val=texture(tex,co).x;
  //fColor=vec4(log(linearize(val))*10);
  //fColor=vec4((1-val)*50);

  if(linMet==0)fColor=vec4((linearize(val)-near)/(far-near));
  if(linMet==1)fColor=vec4(val);
  if(linMet==2)fColor=vec4((1-val)*50);
  if(linMet==3)fColor=vec4(log(linearize(val))*10);

}
