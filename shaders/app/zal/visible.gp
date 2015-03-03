#version 330
bool NotVisible(vec4 a,vec4 b){
	vec4 l=mvp*LightPosition;
	l.xyz/=l.w;
	if(l.x<-1||l.x>+1||l.y<-1||l.y>+1||l.z<-1||l.z>+1)return false;

	vec4 p;
	p=mvp*a;
	p.xyz/=p.w;
	if(p.x>=-1&&p.x<=1&&p.y>=-1&&p.y<=1&&p.z>=-1&&p.z<=1)return false;

	p=mvp*b;
	p.xyz/=p.w;
	if(p.x>=-1&&p.x<=1&&p.y>=-1&&p.y<=1&&p.z>=-1&&p.z<=1)return false;

	return true;
}

bool NotV(vec4 a,vec4 b){
	vec4 aa=mvp*a;
	vec3 A=aa.xyz/=aa.w;
	vec4 bb=mvp*b;
	vec3 B=bb.xyz/=bb.w;
	vec4 ll=mvp*LightPosition;
	vec3 L=ll.xyz/=ll.w;
	vec3 U=A-L;
	vec3 V=B-A;
	float x=dot(U,L);
	float y=dot(V,L);
	float z=dot(U,V);
	float m=dot(U,U);
	float n=dot(V,V);
	float o=dot(L,L);
	float c=x*z-m*y;
	float j=y*z-n*x;
	if(j==0&&c==0)return false;
	float t;
	if(j==0)t=clamp(sign(c),0,1);
	else t=clamp(c/j,0,1);
	vec3 AA=A+t*(B-A);
	vec3 BB=2*AA-L;
	float l=-dot((BB-AA),AA)/dot(BB-AA,BB-AA);
	if(l<0)return true;
	vec3 X=AA+l*(BB-AA);
	if(length(X)<sqrt(3))return false;
	//if(X.x>=-1&&X.x<=1&&X.y>=-1&&X.y<=1&&X.z>=-1&&X.z<=1)return false;
	return true;
}

