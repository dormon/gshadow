#version 430

#define GENERATE_SIDES
//#define GENERATE_CAPS
//#define BACK_FACING

#ifdef GENERATE_SIDES
	#ifdef GENERATE_CAPS
		#define MAX_VERTICES 18
	#else
		#define MAX_VERTICES 12
	#endif
#else
	#ifdef GENERATE_CAPS
		#define MAX_VERTICES 6
	#else
		#error Sides or caps have to be generated!
	#endif
#endif

#define NUM_VERTICES 3
#define NUM_NORMALS 4
#define NUM_ATTRIB (NUM_VERTICES+NUM_NORMALS)

layout(points)in;
layout(triangle_strip,max_vertices=MAX_VERTICES)out;

uniform vec4 LightPosition;
uniform mat4 mvp;

#define MODEL_VIEW_PROJECTION mvp

in struct{
	vec4 Attrib[NUM_ATTRIB];
}vAttrib[];

void main(){
#ifdef BACK_FACING
	if(dot(vAttrib[0].Attrib[3],LightPosition)>0)return;//triangle is front-facing the light
#else
	if(dot(vAttrib[0].Attrib[3],LightPosition)<0)return;//triangle is back-facing the light
#endif
	vec4 P[6];
	P[0]=vAttrib[0].Attrib[0];
	P[1]=vAttrib[0].Attrib[1];
	P[2]=vAttrib[0].Attrib[2];
	P[3]=vec4(vAttrib[0].Attrib[0].xyz*LightPosition.w-LightPosition.xyz,0);
	P[4]=vec4(vAttrib[0].Attrib[1].xyz*LightPosition.w-LightPosition.xyz,0);
	P[5]=vec4(vAttrib[0].Attrib[2].xyz*LightPosition.w-LightPosition.xyz,0);

#ifdef GENERATE_CAPS
	for(int i=0;i<3;++i){//loop over front-cap vertices
		gl_Position=MODEL_VIEW_PROJECTION*P[i];
		EmitVertex();
	}
	EndPrimitive();
	for(int i=5;i>=3;--i){//loop over back-cap vertices
		gl_Position=MODEL_VIEW_PROJECTION*P[i];
		EmitVertex();
	}
	EndPrimitive();
#endif

#ifdef GENERATE_SIDES
	ivec4 ID[3]={ivec4(0,4,3,1),ivec4(2,3,5,0),ivec4(4,2,1,5)};
	for(int e=0;e<3;++e)//loop over edges
	#ifdef BACK_FACING
		if(dot(vAttrib[0].Attrib[4+e],LightPosition)>0){
	#else
		if(dot(vAttrib[0].Attrib[4+e],LightPosition)<0){
	#endif
			for(int k=0;k<4;++k){//loop over vertices
				gl_Position=MODEL_VIEW_PROJECTION*P[ID[e][k]];
				EmitVertex();
			}
			EndPrimitive();
		}
#endif
}

