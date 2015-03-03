#version 330
#extension GL_ARB_shading_language_420pack:enable

layout(binding=1)uniform sampler2D Color;
layout(binding=2)uniform sampler2D Position;
layout(binding=3)uniform sampler2D Normal;

uniform vec3 CameraPosition;//position of the camera
uniform vec2 WindowSize;//size of window

uniform vec3 LightPosition;
uniform vec3 LightColor;
uniform float LightRadius;

layout(location=0)out vec4 fColor;//output color

vec2 Coord=gl_FragCoord.xy/WindowSize;

vec3 Ka=texture(Color,Coord).xyz;
vec3 La=vec3(.1);//light ambient color

vec3 Kd=texture(Color,Coord).xyz;
vec3 Ld;//light diffuse color

vec3 Ks=vec3(1);
vec3 Ls;//light specular color
float Shininess=50;//shininess factor

float a=1;
float b=1;
float c=40;

vec3 vPosition=texture(Position,Coord).xyz;//position
void main(){
	vec3 Sum=vec3(0);//sumation of lights' contribution
	//Sum+=Ka*La;//ambient component
	vec3 Nor=texture(Normal,Coord).xyz;
	if(Nor.x==0&&Nor.y==0&&Nor.z==0)return;
	vec3 N=normalize(Nor);
	vec3 V=normalize(CameraPosition-vPosition);//view vector
	vec3 Ld=Ls=LightColor;
	float d=length(vPosition-LightPosition);
	if(d<LightRadius){
		float mag=pow(cos(d/(LightRadius)*3.141592/2),.3);
		float att=mag*clamp(1/(a+b*d+c*d*d),0,1);
		//att=sin(att*10);
		vec3 L=normalize(LightPosition-vPosition);//light vector
		vec3 R=reflect(-L,N);//reflected light vector
		Sum+=att*Kd*Ld*max(dot(L,N),0);//diffuse component
		Sum+=att*Ks*Ls*pow(max(dot(R,V),0),Shininess);//specular component
	}
	fColor=vec4(Sum,1);
}

