#version 430

#define MY_PI 3.1415925

layout(location=0)out vec4 fColor;

layout(binding=1)uniform usampler2D Color;
layout(binding=2)uniform  sampler2D Position;
layout(binding=3)uniform  sampler2D Normal;
layout(binding=8)uniform  sampler2D shadowMask;

uniform vec3 CameraPosition;//position of the camera

ivec2 Coord=ivec2(gl_FragCoord.xy);

uniform vec4 LightPosition;
uniform vec3 La;//light ambient color
uniform vec3 Ld;//light diffuse color
uniform vec3 Ls;//light specular color

uniform bool SSAO;
uniform bool useShadows=false;

uvec4 color          = texelFetch(Color,Coord,0);
vec3  Ka             = vec3((color.xyz>>0)&0xff)/0xff;
vec3  Kd             = Ka;
vec3  Ks             = vec3((color.xyz>>8)&0xff)/0xff;
float Shininess      = 50;//shininess factor
float specularFactor = float(color.w)/255.;

	
uniform float aoFilterRadius = 20.0f; 
uniform float aoDistanceTreshold = 100.0; 
const int sample_count = 16;
const vec2 poisson16[] = vec2[](    // These are the Poisson Disk Samples
	vec2( -0.94201624,  -0.39906216 ),
	vec2(  0.94558609,  -0.76890725 ),
	vec2( -0.094184101, -0.92938870 ),
	vec2(  0.34495938,   0.29387760 ),
	vec2( -0.91588581,   0.45771432 ),
	vec2( -0.81544232,  -0.87912464 ),
	vec2( -0.38277543,   0.27676845 ),
	vec2(  0.97484398,   0.75648379 ),
	vec2(  0.44323325,  -0.97511554 ),
	vec2(  0.53742981,  -0.47373420 ),
	vec2( -0.26496911,  -0.41893023 ),
	vec2(  0.79197514,   0.19090188 ),
	vec2( -0.24188840,   0.99706507 ),
	vec2( -0.81409955,   0.91437590 ),
	vec2(  0.19984126,   0.78641367 ),
	vec2(  0.14383161,  -0.14100790 )
);

float ambientOcclusion(vec3 viewPos, vec3 viewNormal) {
	float ambientOcclusion = 0.0;
	// perform AO
	for (int i = 0; i < sample_count; ++i)
	{
		// sample at an offset specified by the current Poisson-Disk sample and scale it by a radius (has to be in Texture-Space)
		vec2 sampleTexCoord = gl_FragCoord.xy + (poisson16[i] * (aoFilterRadius));
		vec3 samplePos = texelFetch(Position, ivec2(sampleTexCoord), 0).xyz;
		vec3 sampleDir = normalize(samplePos - viewPos);
		// angle between SURFACE-NORMAL and SAMPLE-DIRECTION (vector from SURFACE-POSITION to SAMPLE-POSITION)
		float NdotS = max(dot(viewNormal, sampleDir), 0);
		//if(NdotS < 0.5) return vec3(1.0, 1.0, 1.0);
		// distance between SURFACE-POSITION and SAMPLE-POSITION
		float VPdistSP = length(viewPos - samplePos);
		// a = distance function
		float a = 1.0 - smoothstep(aoDistanceTreshold, aoDistanceTreshold * 2, VPdistSP);
		// b = dot-Product
		float b = NdotS;
		ambientOcclusion += (a * b);
	}
	//return vec3(ambientOcclusion/sample_count); 
	return 1.0 - (ambientOcclusion / sample_count);
}

vec3 ComputeDiffuseSpecular(){
	vec3 P = texelFetch(Position,Coord,0).xyz;
	vec3 N = texelFetch(Normal,Coord,0).xyz;
	vec3 Sum=vec3(0);
	vec3 V=normalize(CameraPosition-P);//view vector
	vec3 L=normalize(LightPosition.xyz-P*LightPosition.w);//light vector
	vec3 R=reflect(-L,N);//reflected light vector
	float diffuseFactor=max(dot(L,N),0);
	Sum+=diffuseFactor*Kd*Ld;//diffuse component
	if(diffuseFactor>0)
		Sum+=specularFactor*Ks*Ls*pow(max(dot(R,V),0),Shininess);//specular component
	return Sum;
}

void main(){
	vec3 Sum=vec3(0);

	vec3  gPosition    = texelFetch(Position,Coord,0).xyz;
	vec3  gNormal      = texelFetch(Normal,Coord,0).xyz;
	
	float ac=SSAO?ambientOcclusion(gPosition,gNormal):1;//ambient component

	Sum+=Ka*La*ac;
  float shadowCoef=1;
  if(useShadows)shadowCoef=texelFetch(shadowMask,Coord,0).x;
  Sum+=shadowCoef*ComputeDiffuseSpecular()*ac;

	fColor=vec4(Sum,1);//output color
}


