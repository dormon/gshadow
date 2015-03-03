#include"mymath.hpp"

void MyMath::Add3(float*a,float*b){
	a[0]+=b[0];
	a[1]+=b[1];
	a[2]+=b[2];
}

void MyMath::Sub3(float*a,float*b){
	a[0]-=b[0];
	a[1]-=b[1];
	a[2]-=b[2];
}

void MyMath::Mul3(float*a,float*b){
	a[0]*=b[0];
	a[1]*=b[1];
	a[2]*=b[2];
}

float MyMath::Dot3(float*a,float*b){
	return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}

void MyMath::KMul3(float*a,float K){
	a[0]*=K;
	a[1]*=K;
	a[2]*=K;
}

float MyMath::Len3(float*a){
	return sqrt(MyMath::Dot3(a,a));
}

void MyMath::Nor3(float*a){
	float l=MyMath::Len3(a);
	if(l==0)return;
	MyMath::KMul3(a,1/l);
}

void MyMath::Rot3(float*a){
	a[0]=1/a[0];
	a[1]=1/a[1];
	a[2]=1/a[2];
}

void MyMath::Ortho3(float*z,float*x,float*y){
	MyMath::Cross3(z,x,y);
	MyMath::Nor3(z);
}

void MyMath::Cross3(float*z,float*x,float*y){
	z[0]=x[1]*y[2]-x[2]*y[1];
	z[1]=y[0]*x[2]-x[0]*y[2];
	z[2]=x[0]*y[1]-y[0]*x[1];
}

void MyMath::Mov3(float*a,float*b){
	a[0]=b[0];
	a[1]=b[1];
	a[2]=b[2];
}

void MyMath::Rnd3(float*a){
	do{
		//TODO
		//a[0]=Random(-1,1);
		//a[1]=Random(-1,1);
		//a[2]=Random(-1,1);
	}while(MyMath::Len3(a)==0);
	MyMath::Nor3(a);
}

void MyMath::LN3(float*b,float*a){
	do{
		MyMath::Rnd3(b);
	}while(MyMath::Dot3(b,a)==MyMath::Len3(a)*MyMath::Len3(b));
}

float MyMath::Cos3(float*u,float*v){
	float amul=MyMath::Dot3(u,v);
	if(amul==0)return 0;
	return amul/MyMath::Len3(u)/MyMath::Len3(v);
}

void MyMath::Make3(float*O,float*A,float*B){
	MyMath::Mov3(O,B);
	MyMath::Sub3(O,A);
}

void MyMath::Reflect3(float*O,float*V,float*N){
	MyMath::Mov3(O,N);
	MyMath::KMul3(O,-2*MyMath::Dot3(V,N));
	MyMath::Add3(O,V);
}

int MyMath::Eq3(float*A,float*B){
	//TODO
	return 1;//float_Eq(A[0],B[0])&&float_Eq(A[1],B[1])&&float_Eq(A[2],B[2]);
}

int MyMath::Greater3(float*A,float*B){
  if(A[0]>B[0])return 1;
  if(A[0]<B[0])return-1;
  if(A[1]>B[1])return 1;
  if(A[1]<B[1])return-1;
  if(A[2]>B[2])return 1;
  if(A[2]<B[2])return-1;
  return 0;
}

