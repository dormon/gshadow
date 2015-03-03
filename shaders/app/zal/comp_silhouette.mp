#version 430 core

layout(local_size_x=32)in;

layout(std430,binding=0)buffer SInput{vec4 IBuffer[];};
layout(std430,binding=1)buffer SOutput{vec4 OBuffer[];};
layout(std430,binding=2)buffer SCounter{uint Counter;};

uniform uint NumEdge;
uniform vec4 LightPosition;

int GreaterVec(vec3 a,vec3 b){
	return int(dot(ivec3(sign(a-b)),ivec3(4,2,1)));
}

int ComputeMult(vec3 A,vec3 B,vec3 C,vec4 L){
  vec3 n=cross(C-A,L.xyz-A*L.w);
	return int(sign(dot(n,B-A)));
}

void main(){
	uint gid=gl_GlobalInvocationID.x;
	if(gid<NumEdge){//compute silhouette
		vec4 P[4];
		gid*=16;
		P[0]=IBuffer[gid+0];
		P[1]=IBuffer[gid+1];
		P[2]=vec4(P[0].xyz*LightPosition.w-LightPosition.xyz,0);
		P[3]=vec4(P[1].xyz*LightPosition.w-LightPosition.xyz,0);

		int Multiplicity=0;
		int Num=int(IBuffer[gid+15].x)+2;
		for(int i=2;i<Num;++i){
			#define T0 P[0].xyz
			#define T1 P[1].xyz
			#define T2 IBuffer[gid+i].xyz
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

		int NumCap=int(IBuffer[gid+15].y);
		//NumCap=0; //zpass
		int NumTriangles=abs(Multiplicity)+NumCap;
		if(NumTriangles==0)return;
		uint WH=atomicAdd(Counter,6*NumTriangles);

		if(Multiplicity>0){
			for(int i=0;i<Multiplicity;++i){
				OBuffer[WH++]=P[0];
				OBuffer[WH++]=P[1];
				OBuffer[WH++]=P[2];
				OBuffer[WH++]=P[2];
				OBuffer[WH++]=P[1];
				OBuffer[WH++]=P[3];
			}
		}
		if(Multiplicity<0){
			Multiplicity=-Multiplicity;
			for(int i=0;i<Multiplicity;++i){
				OBuffer[WH++]=P[1];
				OBuffer[WH++]=P[0];
				OBuffer[WH++]=P[3];
				OBuffer[WH++]=P[3];
				OBuffer[WH++]=P[0];
				OBuffer[WH++]=P[2];
			}
		}

		NumCap+=2;
		for(int i=2;i<NumCap;++i){
			vec3 NN=cross(P[1].xyz-P[0].xyz,IBuffer[gid+i].xyz-P[0].xyz);
			if(dot(NN,P[2].xyz)<0){
				OBuffer[WH++]=P[2];
				OBuffer[WH++]=P[3];
				OBuffer[WH++]=vec4(IBuffer[gid+i].xyz*LightPosition.w-LightPosition.xyz,0);
				OBuffer[WH++]=P[1];
				OBuffer[WH++]=P[0];
				OBuffer[WH++]=IBuffer[gid+i];
			}else{
				OBuffer[WH++]=P[3];
				OBuffer[WH++]=P[2];
				OBuffer[WH++]=vec4(IBuffer[gid+i].xyz*LightPosition.w-LightPosition.xyz,0);
				OBuffer[WH++]=P[0];
				OBuffer[WH++]=P[1];
				OBuffer[WH++]=IBuffer[gid+i];
			}
		}
	}
}
