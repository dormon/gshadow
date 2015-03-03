#version 430
#extension GL_ARB_shader_stencil_export: enable

#define MY_PI 3.1415925

layout(location=0)out vec4 fColor;

layout(binding=1)uniform sampler2D Color;
layout(binding=2)uniform sampler2D Position;
layout(binding=3)uniform sampler2D Normal;
layout(binding=4)uniform sampler2D Stencil;
layout(binding=5)uniform sampler2DShadow shadow;

uniform vec3 CameraPosition;//position of the camera
uniform mat4 BPV;//bias projection view

ivec2 Coord=ivec2(gl_FragCoord.xy);

uniform vec4 LightPosition;
uniform vec3 La;//light ambient color
uniform vec3 Ld;//light diffuse color
uniform vec3 Ls;//light specular color

uniform bool FrameBufferFlag;
uniform bool ImageAtomicAddFlag;

vec3  Ka           = texelFetch(Color,Coord,0).rgb;

vec3  Kd        = Ka;//material diffuse color
vec3  Ks        = vec3(1);//material specular color
float Shininess = 50;//shininess factor

ivec3 Shuffle(ivec3 A,ivec3 Max){
	return 
	ivec3(
		(A.x*13+1337)%Max.x,
		(A.y*17+33137)%Max.y,
		(A.z*23+51337)%Max.z
	);
}

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

vec3 VisualizeFrameBuffer(float StencilValue){
	vec3 Sum=vec3(0);
	float b=0;
	if(abs(StencilValue)>100){
		b=abs(StencilValue)/1000;
		Sum=vec3(0,0,b);
	}else
		Sum=vec3(max(StencilValue,0)/10,max(-StencilValue,0)/10,0);
	return Sum;
}


void main(){
	vec3 Sum=vec3(0);


	Sum+=Ka*La;//ambient component

	vec4 vShadowPos = BPV*vec4(texelFetch(Position,Coord,0).xyz,1);

	float ss[9];
	ss[0]=textureProjOffset(shadow,vShadowPos,ivec2(-1,-1)).x;
	ss[1]=textureProjOffset(shadow,vShadowPos,ivec2(+0,-1)).x;
	ss[2]=textureProjOffset(shadow,vShadowPos,ivec2(+1,-1)).x;
	ss[3]=textureProjOffset(shadow,vShadowPos,ivec2(-1,0)).x;
	ss[4]=textureProjOffset(shadow,vShadowPos,ivec2(+0,0)).x;
	ss[5]=textureProjOffset(shadow,vShadowPos,ivec2(+1,0)).x;
	ss[6]=textureProjOffset(shadow,vShadowPos,ivec2(-1,+1)).x;
	ss[7]=textureProjOffset(shadow,vShadowPos,ivec2(+0,+1)).x;
	ss[8]=textureProjOffset(shadow,vShadowPos,ivec2(+1,+1)).x;
	//vec4 edge=vec4(0,1,0,0);
	if(ss[0]!=ss[1]||
	ss[2]!=ss[3]||
	ss[4]!=ss[5]||
	ss[6]!=ss[7]||
	ss[0]!=ss[2]||
	ss[4]!=ss[6]||
	ss[0]!=ss[4]||
	ss[0]!=ss[8]){
		//edge=vec4(1,0,0,1);
		//return;
		//s=1;
		//gl_FragDepth=1;
		gl_FragStencilRefARB=128;
		//discard;
	}else{
		Sum+=/*ss[0]*/ComputeDiffuseSpecular();
		//gl_FragDepth=1;
		gl_FragStencilRefARB=0;
	}

	fColor=vec4(Sum,1);//output color
	//fColor=vec4(texture(shadow,gl_FragCoord.xy).xyz/10,1);
}

