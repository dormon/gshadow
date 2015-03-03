#version 440 core
layout(points)in;
layout(triangle_strip,max_vertices=4)out;

out vec2 gCoord;

void main(){
	for(int i=0;i<4;++i){
		gl_Position=vec4(-1+2*(i%2),-1+2*(i/2),-1,1);
    gCoord=vec2(-1+2*(i%2),-1+2*(i/2));
		EmitVertex();
	}
	EndPrimitive();
}
