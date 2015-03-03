#version 330
#extension GL_ARB_shading_language_420pack:enable

// texture of lights
// 0. line - position and radius
// 1. line - color
layout(binding=0)uniform sampler2D Lights;//texture of lights
uniform int NumLights;//number of lights
uniform int Segment;//id of segment

layout(binding=1)uniform sampler2D Color;
layout(binding=2)uniform sampler2D Position;
layout(binding=3)uniform sampler2D Normal;

uniform vec3 CameraPosition;//position of the camera
uniform vec2 WindowSize;//size of window

layout(location=0)out vec4 fColor;//output color

vec2 Coord=gl_FragCoord.xy/WindowSize;

vec3 Ka=texture(Color,Coord).xyz;
vec3 La=vec3(0,1,1);//light ambient color

uniform float ia;
vec3 Kd=Ka;//texture(Color,Coord).xyz;
vec3 Ld;//light diffuse color

vec3 Ks=vec3(1);
vec3 Ls;//light specular color
float Shininess=50;//shininess factor

uniform float a;
uniform float b;
uniform float c;
uniform float mu;
/*
float a=1;
float b=1;
float c=40;
*/
vec3 vPosition=texture(Position,Coord).xyz;//position
uniform int nn;
void main(){
	vec3 Sum=vec3(0);//sumation of lights' contribution
	Sum+=Ka*La*ia;//ambient component
	vec3 Nor=texture(Normal,Coord).xyz;
	if(Nor.x==0&&Nor.y==0&&Nor.z==0)return;
	vec3 N=normalize(Nor);
	vec3 V=normalize(CameraPosition-vPosition);//view vector
	for(int i=0;i<NumLights;++i){//loop over lights
		vec4 LightPositionRadius=
			texelFetch(Lights,ivec2(i,0+Segment*2),0);//position and radius
		vec3 Ld=Ls=texelFetch(Lights,ivec2(i,1+Segment*2),0).rgb;//light color
		float d=length(vPosition-LightPositionRadius.xyz);
		if(d<LightPositionRadius.w){
			float mag=pow(cos(d/LightPositionRadius.w*3.141592/2),mu);
			float att=clamp(mag/(a+b*d+c*d*d),0,1);
			vec3 L=normalize(LightPositionRadius.xyz-vPosition);//light vector
			vec3 R=reflect(-L,N);//reflected light vector
			Sum+=att*Kd*Ld*max(dot(L,N),0);//diffuse component
			Sum+=att*Ks*Ls*pow(max(dot(R,V),0),Shininess);//specular component
		}
	}
	fColor=vec4(Sum,1);
	//fColor=vec4(1);
}
