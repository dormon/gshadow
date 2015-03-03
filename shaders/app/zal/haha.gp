#version 330
#extension GL_EXT_geometry_shader4:enable
layout(points)in;
layout(triangle_strip,max_vertices=4)out;
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
uniform vec4 LightPosition;

in vec4 vPosition0[];
in vec4 vPosition1[];
in vec4 vPosition2[];
in vec4 vPosition3[];
in vec4 vPosition4[];
in vec4 vPosition5[];
in vec4 vPosition6[];
in vec4 vPosition7[];
in vec4 vPosition8[];
in vec4 vPosition9[];
in vec4 vPosition10[];
in vec4 vPosition11[];
in vec4 vPosition12[];
in vec4 vPosition13[];

flat out float gMultiplicity;

void main(){
	vec4 P[4];
	P[0]=gl_PositionIn[0];
	P[1]=vPosition0[0];
	P[2]=vec4(P[0].xyz*LightPosition.w-LightPosition.xyz,0);
	P[3]=vec4(P[1].xyz*LightPosition.w-LightPosition.xyz,0);

	vec3 N=normalize(cross((P[1]-P[0]).xyz,(LightPosition-P[0]).xyz));
	float d=dot(N,P[0].xyz);
	int Num=int(vNum[0].x);
	vec4 O[13];
	O[0]=vPosition1[0];
	O[1]=vPosition2[0];
	O[2]=vPosition3[0];
	O[3]=vPosition4[0];
	O[4]=vPosition5[0];
	O[5]=vPosition6[0];
	O[6]=vPosition7[0];
	O[7]=vPosition8[0];
	O[8]=vPosition9[0];
	O[9]=vPosition10[0];
	O[10]=vPosition11[0];
	O[11]=vPosition12[0];
	O[12]=vPosition13[0];
	int Multiplicity=0;
	for(int i=0;i<13;++i)
		if(dot(N,O[i].xyz)<d)Multiplicity++;
		else Multiplicity--;

	gMultiplicity=Multiplicity;gl_Position=p*v*m*P[0];EmitVertex();
	gMultiplicity=Multiplicity;gl_Position=p*v*m*P[1];EmitVertex();
	gMultiplicity=Multiplicity;gl_Position=p*v*m*P[2];EmitVertex();
	gMultiplicity=Multiplicity;gl_Position=p*v*m*P[3];EmitVertex();
	EndPrimitive();
}
