#version 430

#define MY_PI 3.1415925

layout(location=0)out vec4 fColor;

layout(binding=1)uniform usampler2D       Color   ;
layout(binding=2)uniform  sampler2D       Position;
layout(binding=3)uniform  sampler2D       Normal  ;
layout(binding=4)uniform  sampler2D       Stencil ;
layout(binding=5)uniform  sampler2DShadow shadow  ;

uniform vec3 CameraPosition;//position of the camera
uniform mat4 BPV;//bias projection view

ivec2 Coord=ivec2(gl_FragCoord.xy);

uniform vec4 LightPosition;
uniform vec3 La;//light ambient color
uniform vec3 Ld;//light diffuse color
uniform vec3 Ls;//light specular color

uniform bool FrameBufferFlag;
uniform bool ImageAtomicAddFlag;

uvec4 color     = texelFetch(Color,Coord,0);
vec3  Ka        = vec3((color.xyz>>0)&0xff)/0xff;
vec3  Kd        = Ka;
vec3  Ks        = vec3((color.xyz>>8)&0xff)/0xff;
float Shininess = 50;//shininess factor
float specularFactor = float(color.w)/255.;

vec3 ComputeDiffuseSpecular(){
	vec3  gPosition    = texelFetch(Position,Coord,0).xyz;
	vec3  gNormal      = texelFetch(Normal,Coord,0).xyz;
	vec3 Sum=vec3(0);
	vec3 N=normalize(gNormal);//normal
	vec3 V=normalize(CameraPosition-gPosition);//view vector
	vec3 L=normalize(LightPosition.xyz-gPosition*LightPosition.w);//light vector
	vec3 R=reflect(-L,N);//reflected light vector
	float DiffuseFactor=max(dot(L,N),0);
	Sum+=Kd*Ld*DiffuseFactor;//diffuse component
	if(DiffuseFactor>0)
		Sum+=Ks*Ls*pow(max(dot(R,V),0),Shininess);//specular component
	float dsl=length(LightPosition.xyz-gPosition*LightPosition.w);
	float dvs=length(CameraPosition-gPosition);
	float dvl=length(LightPosition.xyz-CameraPosition*LightPosition.w);

	return Sum;
}


void main(){
	vec3 Sum=Ka*La;
	vec4 vShadowPos = BPV*vec4(texelFetch(Position,Coord,0).xyz,1);
	float s=textureProj(shadow,vShadowPos).x;
  Sum+=s*ComputeDiffuseSpecular();
	fColor=vec4(Sum,1);//+edge;//output color
}

