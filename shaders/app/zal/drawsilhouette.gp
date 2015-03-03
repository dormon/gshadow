#version 330

layout(points)in;
layout(line_strip,max_vertices=2)out;

uniform mat4 mvp;
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
in vec4 vPosition14[];

out vec4 gColor;

int GreaterVec(vec3 a,vec3 b){
	return int(dot(ivec3(sign(a-b)),ivec3(4,2,1)));
}

int ComputeMult(vec3 A,vec3 B,vec3 C,vec4 L){
  vec3 n=cross(C-A,L.xyz-A*L.w);
	return int(sign(dot(n,B-A)));
}

vec4 MultToColor(int Mult){
	return vec4(((Mult>>0)&1),((Mult>>1)&1),((Mult>>2)&3),1);	
}

void main(){
	vec4 P[4];
	P[0]=gl_PositionIn[0];
	P[1]=vPosition0[0];
	P[2]=vec4(P[0].xyz*LightPosition.w-LightPosition.xyz,0);
	P[3]=vec4(P[1].xyz*LightPosition.w-LightPosition.xyz,0);
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
	int Num=int(vPosition14[0].x);
	for(int i=0;i<Num;++i){
#define T0 P[0].xyz
#define T1 P[1].xyz
#define T2 O[i].xyz
		if(GreaterVec(T0,T2)>0){//T[2] T[0] T[1]?
			Multiplicity+=ComputeMult(T2,T0,T1,LightPosition);
		}else{
			if(GreaterVec(T1,T2)>0){//T[0] T[2] T[1]?
				Multiplicity-=ComputeMult(T0,T2,T1,LightPosition);
			}else{//T[0] T[1] T[2]?
				Multiplicity+=ComputeMult(T0,T1,T2,LightPosition);
			}
		}
	}
	if(Multiplicity>0){
		//gColor=vec4(0,0,0,1);gl_Position=mvp*P[0];EmitVertex();
		gColor=MultToColor(Multiplicity);gl_Position=mvp*P[0];EmitVertex();
		gColor=MultToColor(Multiplicity);gl_Position=mvp*P[1];EmitVertex();
	}
	if(Multiplicity<0){
		//gColor=vec4(0,0,0,1);gl_Position=mvp*P[1];EmitVertex();
		gColor=MultToColor(-Multiplicity);gl_Position=mvp*P[1];EmitVertex();
		gColor=MultToColor(-Multiplicity);gl_Position=mvp*P[0];EmitVertex();
	}
	/*
	if(Multiplicity==0){
		gColor=vec4(1,1,1,1);gl_Position=mvp*P[0];EmitVertex();
		gColor=vec4(1,1,1,1);gl_Position=mvp*P[1];EmitVertex();
	}*/
	EndPrimitive();
}
