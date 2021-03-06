#version 430
layout(points)in;
layout(triangle_strip,max_vertices=4)out;

out gl_PerVertex { vec4 gl_Position; };



void main(){
	for(int i=0;i<4;++i){
		gl_Position=vec4(-1+2*(i%2),-1+2*(i/2),-1,1);
		EmitVertex();
	}
	EndPrimitive();
}
