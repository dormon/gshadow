#version 430 core

layout(local_size_x=64)in;

layout(std430,binding=0)buffer SInput{vec4 IBuffer[];};
layout(std430,binding=1)buffer SOutput{vec4 OBuffer[];};
layout(std430,binding=2)buffer SCounter{uint Counter[];};


uniform uint NumEdge;
uniform uint NumTriangle;
uniform vec4 LightPosition;
uniform mat4 mvp;

int GreaterVec(vec3 a,vec3 b){
	return int(dot(ivec3(sign(a-b)),ivec3(4,2,1)));
}

int ComputeMult(vec3 A,vec3 B,vec3 C,vec4 L){
  vec3 n=cross(C-A,L.xyz-A*L.w);
	return int(sign(dot(n,B-A)));
}
	//X=A+t*(B-A);
	//X.i>-X.w && X.i<X.w
	//+X.i+X.w > 0
	//-X.i+X.w > 0
	//+A.i + t*(B.i-A.i) + A.w + t*(B.w-A.w) > 0
	//-A.i - t*(B.i-A.i) + A.w + t*(B.w-A.w) > 0
	//(+A.i+A.w) + t*((+B.i+B.w)-(+A.i-A.w)) > 0
	//(-A.i+A.w) + t*((-B.i+B.w)-(-A.i-A.w)) > 0
	//M + t*(N-M) > 0
	//O + t*(P-O) > 0
	//M + t*NM > 0
	//O + t*PO > 0
	//M + t*NM = 0 => t=-M/NM
	//O + t*PO = 0 => t=-O/PO
	//if(NM==0&&M>=0)t in <0,1>
	//if(NM==0&&M< 0)t in ø
	//if(NM >0)      t in <-M/NM,+oo> and <0,1>
	//if(NM <0)      t in <-oo,-N/NM> and <0,1>
      /*
X(t,l).x*R.x=X(t,l).y*R.y
X(t,l).x*R.x=X(t,l).z*R.z
(A+t*B+l*C).x*R.x = (A+t*B+l*C).y*R.y
(A+t*B+l*C).x*R.x = (A+t*B+l*C).z*R.z
(A.x*R.x-A.y*R.y) + t*(B.x*R.x-B.y*R.y) + l*(C.x*R.x-C.y*R.y) = 0
(A.x*R.x-A.z*R.z) + t*(B.x*R.x-B.z*R.z) + l*(C.x*R.x-C.z*R.z) = 0
m + t*n + l*o = 0
p + t*q + l*r = 0

t=(-m-l*o)/n
t=(-p-l*r)/q
(-m-l*o)/n=(-p-l*r)/q
(m+l*o)*q=(p+l*r)*n
l=(m*q-p*n)/(r*n-o*q)

l=(-m-t*n)/o
l=(-p-t*q)/r
(-m-t*n)/o=(-p-t*q)/r
(m+t*n)*r=(p+t*q)*o
t=(m*r-p*o)/(q*o-n*r)
	*/

bool IsEdgeVisible(in vec4 A,in vec4 B){
	vec3 M=+A.xyz+A.www;
	vec3 N=+B.xyz+B.www;
	vec3 O=-A.xyz+A.www;
	vec3 P=-B.xyz+B.www;
	vec3 NM=N-M;
	vec3 PO=P-O;
	float Left=0;
	float Right=1;
	for(int i=0;i<3;++i){
		if(NM[i]==0){
			if(M[i]<0)return false;
		}else{
			if(NM[i]>0)Left=max(Left,-M[i]/NM[i]);
			else Right=min(Right,-M[i]/NM[i]);
		}
 	                                           
		if(PO[i]==0){
			if(O[i]<0)return false;
		}else{
			if(PO[i]>0)Left=max(Left,-O[i]/PO[i]);
			else Right=min(Right,-O[i]/PO[i]);
		}
	}
	return Left<=Right;
}

bool IsFullVisible(in vec4 A,in vec4 B,in vec4 C,int Diag){
  vec3 a=A.xyz;
  vec3 b=B.xyz;
  vec3 c=C.xyz;
  if(Diag>=0){
    a[Diag]=-a[Diag];
    b[Diag]=-b[Diag];
    c[Diag]=-c[Diag];
  }
	float m=(a.x-a.y);
	float n=(b.x-b.y);
	float o=(c.x-c.y);
	float p=(a.x-a.z);
	float q=(b.x-b.z);
	float r=(c.x-c.z);
  float d=(q*o-n*r);
  float t=(m*r-p*o)/d;
  float l=-(m*q-p*n)/d;
	vec4 X=A+t*B+l*C;
  return (t>0)&&(t<1)&&(l>0)&&(l<1)&&
      all(greaterThan(X.xyz,-X.www))&&all(lessThan(X.xyz,X.www));
}

bool IsVisible(in vec4 a,in vec4 b,in vec4 c,in vec4 d,vec4 l){
	vec4 A=mvp*a;
	vec4 B=mvp*b;
	vec4 C=mvp*c;
	vec4 D=mvp*d;
	vec3 n=(mvp*vec4(cross(b.xyz-a.xyz,l.xyz-a.xyz*l.w),0)).xyz;
	ivec3 Corner=ivec3(1+sign(n))>>1;
  if(Corner.z==1)Corner=ivec3(1)-Corner;
	int Diag=Corner.x+(Corner.y<<1)-1;
	if(IsFullVisible(A,B-A,C-A,Diag))return true;
	if(IsEdgeVisible(A,B))return true;
	if(IsEdgeVisible(A,C))return true;
	if(IsEdgeVisible(B,D))return true;
	if(IsEdgeVisible(C,D))return true;
	return false;
}

bool IsVisible(in vec4 P[4],in int Diag){
	if(IsFullVisible(P[0],P[1]-P[0],P[2]-P[0],Diag))return true;
	if(IsEdgeVisible(P[0],P[1]))return true;
	if(IsEdgeVisible(P[0],P[2]))return true;
	if(IsEdgeVisible(P[1],P[3]))return true;
	if(IsEdgeVisible(P[2],P[3]))return true;
	return false;
}

uniform int mode=0;
uniform int mode2=0;

void main(){
	uint gid=gl_GlobalInvocationID.x;
	if(gid<NumEdge){//compute silhouette
		vec4 P[4];
		//gid*=16;
    gid<<=4;
		
		P[0]=IBuffer[gid+0];
		P[1]=IBuffer[gid+1];
		P[2]=vec4(P[0].xyz*LightPosition.w-LightPosition.xyz,0);
		P[3]=vec4(P[1].xyz*LightPosition.w-LightPosition.xyz,0);

    vec4 ClipP[4];
    for(int i=0;i<4;++i)
      ClipP[i]=mvp*P[i];

    vec3 Normal=(mvp*vec4(cross(
        P[1].xyz-P[0].xyz,
        LightPosition.xyz-P[0].xyz*LightPosition.w),0)).xyz;
	  ivec3 Corner=ivec3(1+sign(Normal))>>1;
    if(Corner.z==1);Corner=ivec3(1)-Corner;
	  int Diag=Corner.x+(Corner.y<<1)-1;

		if(!IsVisible(ClipP,Diag))return;

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
		if(Multiplicity>0){
			uint WH=atomicAdd(Counter[(Multiplicity-1)<<2],4)+NumEdge*6*(Multiplicity-1);
			WH+=int(Multiplicity>1)*6*NumTriangle;
			OBuffer[WH++]=P[0];
			OBuffer[WH++]=P[1];
			OBuffer[WH++]=P[2];
			OBuffer[WH++]=P[3];
		}
		if(Multiplicity<0){
			Multiplicity=-Multiplicity;
			uint WH=atomicAdd(Counter[(Multiplicity-1)<<2],4)+NumEdge*6*(Multiplicity-1);
			WH+=int(Multiplicity>1)*6*NumTriangle;
			OBuffer[WH++]=P[1];
			OBuffer[WH++]=P[0];
			OBuffer[WH++]=P[3];
			OBuffer[WH++]=P[2];
		}
	}
}
