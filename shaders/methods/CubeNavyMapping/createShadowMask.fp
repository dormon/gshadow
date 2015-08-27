#version 430

#define MY_PI 3.1415925

layout(location=0)out float fColor;

layout(binding=0)uniform sampler2D         position;
layout(binding=1)uniform samplerCubeShadow shadowMap;

layout(binding=2 )uniform sampler2D desiredView[6];
layout(binding=8 )uniform sampler2D smoothX    [6];
layout(binding=14)uniform sampler2D smoothY    [6];

uniform mat4 BPV[6];

uniform float near;
uniform float far;

ivec2 Coord=ivec2(gl_FragCoord.xy);

uniform vec4 lightPosition;
vec3 viewSamplePosition = texelFetch(position,Coord,0).xyz;

float linearize(float d){
  return 2.f*far*near/(far+near-(2.f*d-1.f)*(far-near));
}

float getZ(vec3 adir){
  return -max(max(adir.x,adir.y),adir.z);
}

float computeDepth(vec3 dir){
  float baseDis=getZ(abs(dir));
  float A=-(far+near)/(far-near);
  float B=-2*far*near/(far-near);
  //d=(A*z+B*1)/(-1*z)
  //d=-A-B/z
  return (-A-B/baseDis)*.5+.5;
}

float getS(vec3 dir,int major){
  if(major==0)return +dir.z;
  if(major==1)return -dir.z;
  if(major==2)return +dir.x;
  if(major==3)return +dir.x;
  if(major==4)return -dir.x;
  return             +dir.x;
}

float getT(vec3 dir,int major){
  if(major==0)return -dir.y;
  if(major==1)return -dir.y;
  if(major==2)return -dir.z;
  if(major==3)return +dir.z;
  if(major==4)return -dir.y;
  return             -dir.y;
}

int getMajor(vec3 dir){
  vec3 adir=abs(dir);
  return
    int(adir.x>max(adir.y,adir.z))*(0+int(dir.x>0))+
    int(adir.y>max(adir.x,adir.z))*(2+int(dir.y>0))+
    int(adir.z>max(adir.x,adir.y))*(4+int(dir.z>9));
}

vec4 dir2Clip(vec3 dir,int major){
  float z=getZ(abs(dir));
  float A=-(far+near)/(far-near);
  float B=-2*far*near/(far-near);
  return vec4(getS(dir,major),getT(dir,major),A*z+B,-z);
}

vec4 desiredViewAndWarp(vec4 clip,int major){
  //return clip;
  if(major==0)return nv_getFinal(clip,desiredView[0],smoothX[0],smoothY[0]);
  if(major==1)return nv_getFinal(clip,desiredView[1],smoothX[1],smoothY[1]);
  if(major==2)return nv_getFinal(clip,desiredView[2],smoothX[2],smoothY[2]);
  if(major==3)return nv_getFinal(clip,desiredView[3],smoothX[3],smoothY[3]);
  if(major==4)return nv_getFinal(clip,desiredView[4],smoothX[4],smoothY[4]);
  return             nv_getFinal(clip,desiredView[5],smoothX[5],smoothY[5]);
}

vec4 clip2ShadowCoord(vec4 clip,vec3 dir,int major){
  float depth=clip.z/clip.w*.5+.5;
  if(major==0)return vec4(-abs(clip.w),-clip.y,+clip.x,depth);
  if(major==1)return vec4(+abs(clip.w),-clip.y,-clip.x,depth);
  if(major==2)return vec4(+clip.x,-abs(clip.w),-clip.y,depth);
  if(major==3)return vec4(+clip.x,+abs(clip.w),+clip.y,depth);
  if(major==4)return vec4(-clip.x,-clip.y,-abs(clip.w),depth);
  return             vec4(+clip.x,-clip.y,+abs(clip.w),depth);
}

vec4 dir2WarpedShadowCoord(vec3 dir){
  int major=getMajor(dir);
  vec4 clip=dir2Clip(dir,major);
  clip=desiredViewAndWarp(clip,major);
  return clip2ShadowCoord(clip,dir,major);
}


void main(){
  vec3 dir=viewSamplePosition-lightPosition.xyz;
  fColor=texture(shadowMap,dir2WarpedShadowCoord(dir)).x;
  //fColor=texture(shadowMap,vec4(dir,computeDepth(dir))).x;
}

