#version 430

layout(vertices=1)out;

uniform vec4 LightPosition;

int GreaterVec(vec3 a,vec3 b){
	return int(dot(sign(a-b),vec3(4,2,1)));
}

int ComputeMult(vec3 A,vec3 B,vec3 C,vec4 L){
  vec3 n=cross(C-A,L.xyz-A*L.w);
	return int(sign(dot(n,B-A)));
}

patch out vec4 P[4];
patch out float TTT;

void main(){
	int Num=int(gl_in[2].gl_Position.x);

	P[0]=vec4(gl_in[0].gl_Position.xyz,1);
	P[1]=vec4(gl_in[1].gl_Position.xyz,1);
	P[2]=vec4(P[0].xyz*LightPosition.w-LightPosition.xyz,0);
	P[3]=vec4(P[1].xyz*LightPosition.w-LightPosition.xyz,0);
	int Multiplicity=0;
	for(int i=0;i<Num;++i){
#define T0 P[0].xyz
#define T1 P[1].xyz
#define T2 gl_in[i+3].gl_Position.xyz
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
	int AbsMultiplicity=abs(Multiplicity);
	gl_TessLevelOuter[0]=int(AbsMultiplicity>0);
	gl_TessLevelOuter[2]=1;
	TTT=gl_TessLevelOuter[1]=gl_TessLevelOuter[3]=gl_TessLevelInner[0]=2*AbsMultiplicity-1;
	gl_TessLevelInner[1]=1;
	if(Multiplicity>0){
		vec4 c=P[0];
		P[0]=P[1];
		P[1]=c;
		c=P[2];
		P[2]=P[3];
		P[3]=c;
	}
}
