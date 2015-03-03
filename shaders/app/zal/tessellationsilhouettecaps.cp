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

void Swap(inout vec4 A,inout vec4 B){vec4 C=A;A=B;B=C;}

patch out vec4 P[6];

void main(){
	P[0]=vec4(gl_in[0].gl_Position.xyz,1);
	P[1]=vec4(gl_in[1].gl_Position.xyz,1);
	P[2]=vec4(gl_in[2].gl_Position.xyz,1);

	if(GreaterVec(P[0].xyz,P[1].xyz)>0)Swap(P[0],P[1]);
	if(GreaterVec(P[1].xyz,P[2].xyz)>0)Swap(P[1],P[2]);
	if(GreaterVec(P[0].xyz,P[1].xyz)>0)Swap(P[0],P[1]);

	int Multiplicity=ComputeMult(P[0].xyz,P[1].xyz,P[2].xyz,LightPosition);
	if(Multiplicity>0)Swap(P[0],P[1]);

	P[3]=vec4(P[2].xyz*LightPosition.w-LightPosition.xyz,0);
	P[4]=vec4(P[1].xyz*LightPosition.w-LightPosition.xyz,0);
	P[5]=vec4(P[0].xyz*LightPosition.w-LightPosition.xyz,0);

	gl_TessLevelOuter[0]=int(Multiplicity!=0);
	gl_TessLevelOuter[2]=1;
	gl_TessLevelOuter[1]=gl_TessLevelOuter[3]=gl_TessLevelInner[0]=3;
	gl_TessLevelInner[1]=1;
}
