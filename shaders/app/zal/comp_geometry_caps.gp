#version 430

layout(triangles)in;
layout(triangle_strip,max_vertices=6)out;

uniform mat4 mvp;
uniform vec4 LightPosition;

void main(){
	vec4 P[6];
	P[0]=gl_PositionIn[0];
	P[1]=gl_PositionIn[1];
	P[2]=gl_PositionIn[2];

	P[3]=vec4(gl_PositionIn[0].xyz*LightPosition.w-LightPosition.xyz,0);
	P[4]=vec4(gl_PositionIn[1].xyz*LightPosition.w-LightPosition.xyz,0);
	P[5]=vec4(gl_PositionIn[2].xyz*LightPosition.w-LightPosition.xyz,0);
	vec3 N=normalize(cross((P[1]-P[0]).xyz,(P[2]-P[0]).xyz));
	float Distance=dot(N,LightPosition.xyz)-dot(N,P[0].xyz);
	if(Distance>0){
		vec4 c=P[0];
		P[0]=P[1];
		P[1]=c;
		c=P[3];
		P[3]=P[4];
		P[4]=c;
	}
	gl_Position=mvp*P[0];EmitVertex();
	gl_Position=mvp*P[1];EmitVertex();
	gl_Position=mvp*P[2];EmitVertex();
	EndPrimitive();
	gl_Position=mvp*P[5];EmitVertex();
	gl_Position=mvp*P[4];EmitVertex();
	gl_Position=mvp*P[3];EmitVertex();
	EndPrimitive();
}

