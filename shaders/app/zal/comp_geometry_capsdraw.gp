#version 430

layout(triangles)in;
layout(triangle_strip,max_vertices=6)out;

uniform mat4 mvp;
uniform vec4 LightPosition;

int GreaterVec(vec3 a,vec3 b){
	return int(dot(ivec3(sign(a-b)),ivec3(4,2,1)));
}

int ComputeMult(vec3 A,vec3 B,vec3 C,vec4 L){
  vec3 n=cross(C-A,L.xyz-A*L.w);
	return int(sign(dot(n,B-A)));
}

void Swap(inout vec4 A,inout vec4 B){vec4 C=A;A=B;B=C;}

void main(){
	vec4 A=gl_in[0].gl_Position;
	vec4 B=gl_in[1].gl_Position;
	vec4 C=gl_in[2].gl_Position;
	#define T0 P[0].xyz
	#define T1 P[1].xyz
	#define T2 IBuffer[gid+i].xyz
	if(GreaterVec(A.xyz,B.xyz)>0)Swap(A,B);
	if(GreaterVec(B.xyz,C.xyz)>0)Swap(B,C);
	if(GreaterVec(A.xyz,B.xyz)>0)Swap(A,B);
	int Multiplicity=ComputeMult(A.xyz,B.xyz,C.xyz,LightPosition);
	if(Multiplicity==0)return;
	if(Multiplicity>0)Swap(A,B);
	/*
	vec3 N=cross(B.xyz-A.xyz,C.xyz-A.xyz);
	float d=dot(vec4(N,-dot(N,A.xyz)),LightPosition);
	if(d>0){
		vec4 Carry=A;
		A=B;
		B=Carry;
	}*/
	gl_Position=mvp*A;EmitVertex();
	gl_Position=mvp*B;EmitVertex();
	gl_Position=mvp*C;EmitVertex();
	EndPrimitive();
	gl_Position=mvp*vec4(C.xyz*LightPosition.w-LightPosition.xyz,0);EmitVertex();
	gl_Position=mvp*vec4(B.xyz*LightPosition.w-LightPosition.xyz,0);EmitVertex();
	gl_Position=mvp*vec4(A.xyz*LightPosition.w-LightPosition.xyz,0);EmitVertex();
	EndPrimitive();
}
