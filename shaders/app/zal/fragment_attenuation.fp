#version 330
#extension GL_ARB_shading_language_420pack:enable

// texture of lights
// 0. line - position and radius
// 1. line - color
layout(binding=0)uniform sampler2D Lights;//texture of lights
uniform int NumLights;//number of lights
//uniform float Position;
layout(location=0)out vec4 fColor;//output color

uniform vec3 CameraPosition;//position of the camera

in vec3 vPosition;//vertex position in world space
in vec3 vNormal;//vertex normal in world space
in vec3 vCameraPosition;//camera position in world space

vec3 Ka=vec3(.5);//material ambient color
vec3 La=vec3(.1);//light ambient color

vec3 Kd=vec3(.5);//material diffuse color
vec3 Ld;//light diffuse color

vec3 Ks=vec3(1);//material specular color
vec3 Ls;//light specular color
float Shininess=50;//shininess factor

float a=1;
float b=1;
float c=40;

void main(){
	vec3 Sum=vec3(0);//sumation of lights' contribution
	//Sum+=Ka*La;//ambient component
	vec3 N=normalize(vNormal);//normal
	vec3 V=normalize(CameraPosition-vPosition);//view vector
	for(int i=0;i<NumLights;++i){//loop over lights
		vec4 LightPositionRadius=
			texelFetch(Lights,ivec2(i,0),0);//position and radius
		vec3 Ld=Ls=texelFetch(Lights,ivec2(i,1),0).rgb;//light color
		float d=length(vPosition-LightPositionRadius.xyz);
		if(d<LightPositionRadius.w){
			float mag=pow(cos(d/LightPositionRadius.w*3.141592/2),.3);
			float att=clamp(mag/(a+b*d+c*d*d),0,1);
			vec3 L=normalize(LightPositionRadius.xyz-vPosition);//light vector
			vec3 R=reflect(-L,N);//reflected light vector
			Sum+=att*Kd*Ld*max(dot(L,N),0);//diffuse component
			Sum+=att*Ks*Ls*pow(max(dot(R,V),0),Shininess);//specular component
		}
	}
	fColor=vec4(Sum,1);//output color
}
