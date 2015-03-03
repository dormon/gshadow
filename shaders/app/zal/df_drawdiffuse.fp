#version 330
#extension GL_ARB_shading_language_420pack: enable

layout(location=0)out vec4 fragColor;

layout(binding=0)uniform sampler2D TexColor;
layout(binding=1)uniform sampler2D TexPos;
layout(binding=2)uniform sampler2D TexNormal;

layout(binding=3)uniform sampler2DShadow TexShadow;
layout(pixel_center_integer)in vec4 gl_FragCoord;

in vec3 vLPos;
uniform vec3 LightColor;
uniform float LightRadius;
uniform mat4 lmat;

vec3 Kd;
vec3 Ld=LightColor;

vec3 Ks=vec3(1,1,1);
vec3 Ls=LightColor;
float Shininess=50;

vec3 decode(vec2 enc){
	vec2 fenc=enc*4-2;
	float f=dot(fenc,fenc);
	float g=sqrt(1-f/4);
	vec3 n;
	n.xy=fenc*g;
	n.z=1-f/2;
	return n;
}

void main(){
	Kd=texelFetch(TexColor,ivec2(gl_FragCoord),0).rgb;
	vec3 pos=texelFetch(TexPos,ivec2(gl_FragCoord),0).xyz;
	vec3 N=texelFetch(TexNormal,ivec2(gl_FragCoord),0).xyz;
	
	N=decode(N.xy);

	vec3 L=normalize(vLPos-pos);
	float d=pow(clamp(1-length(vLPos-pos)/LightRadius,0,1),2);

	vec3 R=reflect(-L,N);
	vec3 V=normalize(-pos);

	float s=0;
	if(d>0){
		vec4 shadowPoss=lmat*vec4(pos,1);
		s =textureProjOffset(TexShadow,shadowPoss,ivec2(0,0));
		s+=textureProjOffset(TexShadow,shadowPoss,ivec2(0,1));
		s+=textureProjOffset(TexShadow,shadowPoss,ivec2(1,0));
		s+=textureProjOffset(TexShadow,shadowPoss,ivec2(1,1));
		s/=4;
		s*=d;
	}
	vec3 D=Kd*Ld*max(dot(N,L),0);
	vec3 S=Ks*Ls*clamp(pow(max(dot(R,V),0),Shininess),0,1);
	
	fragColor=vec4((D+S)*s,1);
}
