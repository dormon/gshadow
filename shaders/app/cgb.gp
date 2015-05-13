#version 330

#extension GL_EXT_geometry_shader4:enable

layout(triangles)in;
layout(triangle_strip,max_vertices=3)out;

in vec3 vPosition[];
in vec3 vNormal[];

out vec3 gPosition;
out vec3 gNormal;
uniform mat4 m,v,p;//model, view, projecion
uniform mat4 mvp;
//flat out vec4 gPlane;

//uniform vec3 CameraPosition;

flat in uint vDI[];
flat out uint gDI;

void main(){
	for(int i=0;i<3;++i){
    gDI=vDI[i];

		gl_Position=mvp*gl_in[i].gl_Position;
		gPosition=vPosition[i];
		gNormal=vNormal[i];

		EmitVertex();
	}
	EndPrimitive();
  
}


