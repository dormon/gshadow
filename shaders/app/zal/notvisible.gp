#version 330
#extension GL_EXT_geometry_shader4:enable
layout(points)in;
layout(triangle_strip,max_vertices=82)out;
//uniform mat4 m;
//uniform mat4 v;
//uniform mat4 p;
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
flat out float gMultiplicity;
int CmpVec(vec3 a,vec3 b){
	if(a.x<b.x)return-1;
	else if(a.x>b.x)return 1;
	else{
		if(a.y<b.y)return-1;
		else if(a.y>b.y)return 1;
		else{
			if(a.z<b.z)return-1;
			else if(a.z>b.z)return 1;
			else return 0;
		}
	}
}

bool SQ(float u,float v,float w,float s){
}

bool NV(vec4 a,vec4 b,vec4 l){
	a=mvp*a;a.xyz/=a.w;
	b=mvp*b;b.xyz/=b.w;
	l=mvp*l;l.xyz/=l.w;

	vec3 w=a.xyz;
	vec3 v=a.xyz-l.xyz;
	vec3 u=b.xyz-a.xyz;

}

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
void main(){
	vec4 P[4];
	P[0]=gl_PositionIn[0];
	P[1]=vPosition0[0];
	if(NotVisible(P[0],P[1]))return;
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
		vec3 T[3];
		vec3 D[3];
		vec3 NS[3];
		vec4 PL[3];
		float d[3];

		T[0]=P[0].xyz;
		T[1]=P[1].xyz;
		T[2]=O[i].xyz;

		D[0]=LightPosition.xyz-T[0]*LightPosition.w;
		NS[0]=normalize(cross(T[1]-T[0],D[0]));
		PL[0]=vec4(NS[0],-dot(NS[0],T[0]));
		d[0]=dot(PL[0],vec4(T[2],1));

		if(abs(d[0])<0.00001){

			D[1]=LightPosition.xyz-T[1]*LightPosition.w;
			D[2]=LightPosition.xyz-T[2]*LightPosition.w;

			if(CmpVec(T[2],T[0])==1){
				NS[1]=normalize(cross(T[2]-T[0],D[0]));
				PL[1]=vec4(NS[1],-dot(NS[1],T[0]));
				d[1]=dot(PL[1],vec4(T[1],1));
			}
			else{
				NS[1]=normalize(cross(T[0]-T[2],D[2]));
				PL[1]=vec4(NS[1],-dot(NS[1],T[2]));
				d[1]=-dot(PL[1],vec4(T[1],1));
			}

			if(CmpVec(T[2],T[1])==1){
				NS[2]=normalize(cross(T[2]-T[1],D[1]));
				PL[2]=vec4(NS[2],-dot(NS[2],T[1]));
				d[2]=dot(PL[2],vec4(T[0],1));
			}else{
				NS[2]=normalize(cross(T[1]-T[2],D[2]));
				PL[2]=vec4(NS[2],-dot(NS[2],T[2]));
				d[2]=-dot(PL[2],vec4(T[0],1));
			}

			if(d[0]==0||d[1]==0||d[2]==0)continue;
			if(!((d[0]<0&&d[1]>0&&d[2]<0)||(d[0]>0&&d[1]<0&&d[2]>0)))continue;
		}
		if(O[i].w==1){
			float a=1;
			vec3 NN=normalize(cross(P[1].xyz-P[0].xyz,O[i].xyz-P[0].xyz));
			if(dot(NN,LightPosition.xyz)-dot(NN,P[0].xyz)>0)a=1;
			else a=-1;
			gMultiplicity=a;gl_Position=mvp*vec4(P[2].xyz,0);EmitVertex();
			gMultiplicity=a;gl_Position=mvp*vec4(P[3].xyz,0);EmitVertex();
			gMultiplicity=a;gl_Position=mvp*vec4(O[i].xyz*LightPosition.w-LightPosition.xyz,0);EmitVertex();
			EndPrimitive();
			gMultiplicity=-a;gl_Position=mvp*P[0];EmitVertex();
			gMultiplicity=-a;gl_Position=mvp*P[1];EmitVertex();
			gMultiplicity=-a;gl_Position=mvp*O[i];EmitVertex();
			EndPrimitive();
		}
		if(d[0]<0)Multiplicity++;
		else if(d[0]>0)Multiplicity--;
	}

	if(Multiplicity!=0){
		gMultiplicity=Multiplicity;gl_Position=mvp*P[0];EmitVertex();
		gMultiplicity=Multiplicity;gl_Position=mvp*P[1];EmitVertex();
		gMultiplicity=Multiplicity;gl_Position=mvp*P[2];EmitVertex();
		gMultiplicity=Multiplicity;gl_Position=mvp*P[3];EmitVertex();
		EndPrimitive();
	}
}
