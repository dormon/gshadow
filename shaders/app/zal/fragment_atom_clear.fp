#version 430 core

layout(location=0)out vec4 fragColor;

layout(r32i,location=0)writeonly uniform iimage2D Image;
ivec2 Coord=ivec2(gl_FragCoord.xy);

void main(){
	imageStore(Image,Coord,ivec4(0));
	fragColor=vec4(1);
}
