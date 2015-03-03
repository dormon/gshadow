#version 330
#extension GL_EXT_geometry_shader4:enable
layout(points)in;
layout(triangle_strip,max_vertices=4)out;
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
uniform vec4 LightPosition;

in vec4 vPosition01[];
in vec4 vNum[];
in vec4 vPosition03[];
in vec4 vPosition04[];
in vec4 vPosition05[];
in vec4 vPosition06[];
in vec4 vPosition07[];
in vec4 vPosition08[];
in vec4 vPosition09[];
in vec4 vPosition10[];
in vec4 vPosition11[];
in vec4 vPosition12[];
in vec4 vPosition13[];
in vec4 vPosition14[];
in vec4 vPosition15[];

flat out float gMultiplicity;
uniform int caps;

void main(){
	vec4 P[4];
	P[0]=gl_PositionIn[0];
	P[1]=vPosition01[0];
	P[2]=vec4(P[0].xyz*LightPosition.w-LightPosition.xyz,0);
	P[3]=vec4(P[1].xyz*LightPosition.w-LightPosition.xyz,0);

	vec3 N=normalize(cross((P[1]-P[0]).xyz,(LightPosition-P[0]).xyz));
	float d=dot(N,P[0].xyz);
	int Num=int(vNum[0].x);
	vec4 O[13];
	O[0]=vPosition03[0];
	O[1]=vPosition04[0];
	O[2]=vPosition05[0];
	O[3]=vPosition06[0];
	O[4]=vPosition07[0];
	O[5]=vPosition08[0];
	O[6]=vPosition09[0];
	O[7]=vPosition10[0];
	O[8]=vPosition11[0];
	O[9]=vPosition12[0];
	O[10]=vPosition13[0];
	O[11]=vPosition14[0];
	O[12]=vPosition15[0];
	int Multiplicity=0;
	for(int i=0;i<Num;++i){
		if(dot(N,O[i].xyz)<d){//front facing
			Multiplicity++;	
		}else{
			Multiplicity--;
		}
	}
	if(Multiplicity!=0){
		for(int i=0;i<4;++i){
			gMultiplicity=Multiplicity;
			gl_Position=p*v*m*P[i];
			EmitVertex();
		}
		EndPrimitive();
	}
/*
	if(Multiplicity<0){
		Multiplicity=-Multiplicity;
		for(int i=0;i<Multiplicity;++i){
			gl_Position=p*v*m*P[0];EmitVertex();
			gl_Position=p*v*m*P[1];EmitVertex();
			gl_Position=p*v*m*P[2];EmitVertex();
			gl_Position=p*v*m*P[3];EmitVertex();
			EndPrimitive();
		}
	}else if(Multiplicity>0){
		for(int i=0;i<Multiplicity;++i){
			gl_Position=p*v*m*P[1];EmitVertex();
			gl_Position=p*v*m*P[0];EmitVertex();
			gl_Position=p*v*m*P[3];EmitVertex();
			gl_Position=p*v*m*P[2];EmitVertex();
			EndPrimitive();
		}
	}*/
}
