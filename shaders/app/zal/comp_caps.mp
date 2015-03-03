#version 430 core

layout(local_size_x=32)in;

layout(std430,binding=0)buffer SInput{vec4 IBuffer[];};
layout(std430,binding=1)buffer SOutput{vec4 OBuffer[];};

uniform vec4 LightPosition;
uniform uint NumTriangle;

void main(){
	uint gid=gl_GlobalInvocationID.x;
	if(gid<NumTriangle){//compute silhouette
		vec4 A=IBuffer[gid*3+0];
		vec4 B=IBuffer[gid*3+1];
		vec4 C=IBuffer[gid*3+2];
		vec3 N=cross(B.xyz-A.xyz,C.xyz-A.xyz);
		float d=dot(vec4(N,-dot(N,A.xyz)),LightPosition);
		if(d>0){
			vec4 Carry=A;
			A=B;
			B=Carry;
		}
		OBuffer[gid*6+0]=A;
		OBuffer[gid*6+1]=B;
		OBuffer[gid*6+2]=C;
		OBuffer[gid*6+3]=vec4(C.xyz*LightPosition.w-LightPosition.xyz,0);
		OBuffer[gid*6+4]=vec4(B.xyz*LightPosition.w-LightPosition.xyz,0);
		OBuffer[gid*6+5]=vec4(A.xyz*LightPosition.w-LightPosition.xyz,0);
	}
}
