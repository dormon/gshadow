#version 330

#extension GL_EXT_geometry_shader4:enable

layout(triangles)in;
layout(triangle_strip,max_vertices=6)out;

in vec3 vPosition[];
in vec3 vNormal[];

out vec3 gPosition;
out vec3 gNormal;
uniform mat4 m,v,p;//model, view, projecion
uniform mat4 mvp;
//flat out vec4 gPlane;

//uniform vec3 CameraPosition;

void main(){

/*
	vec3 N;
	N=normalize(cross(gl_PositionIn[1].xyz-gl_PositionIn[0].xyz,gl_PositionIn[2].xyz-gl_PositionIn[0].xyz));
	N=normalize((v*m*vec4(-N,0)).xyz);
	*/
	/*vec3 N=normalize(cross(vPosition[1]-vPosition[0],vPosition[2]-vPosition[0]));
	float d=-dot(vPosition[0],N);
	if(dot(CameraPosition,N)+d<0)gPlane=vec4(-N,-d);
	else gPlane=vec4(N,d);*/
	//gPlane=vec4(N,CameraPosition.x);
	//gPlane=vec4(N,d)*float(d>=0)+vec4(-N,-d)*float(d<0);
	for(int i=2;i>=0;--i){
		gl_Position=mvp*gl_in[i].gl_Position;
		gPosition=vPosition[i];

		gNormal=vNormal[i];
		//gNormal=N;
		EmitVertex();
	}
	EndPrimitive();
	for(int i=0;i<3;++i){
		gl_Position=mvp*gl_in[i].gl_Position;
		gPosition=vPosition[i];
		gNormal=vNormal[i];
		//gNormal=N;

		EmitVertex();
	}
	EndPrimitive();
}


