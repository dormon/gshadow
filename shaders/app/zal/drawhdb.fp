#version 430

layout(location=0)out vec4 fColor;

//layout(std430,binding=0)buffer HDB{vec2 HDBData[];};
layout(binding=0)uniform sampler2D HDB;

ivec2 Coord=ivec2(gl_FragCoord.xy);

uniform int mode=0;

void main(){
//*
	if(mode==0){
	ivec2 C=ivec2(Coord.xy);
	int Coord1D=(C.y/8)*8*512+(C.x/8)*64+(C.x%8)+(C.y%8)*8
+64+64*64;
	ivec2 TC=ivec2(Coord1D%1024,Coord1D/1024);
	fColor=vec4(texelFetch(HDB,TC,0).xxx,1);
	}
//	*/

//*
	if(mode==1){
	ivec2 C=ivec2(Coord.xy/8);
	int Coord1D=C.x+C.y*64+64;
	ivec2 TC=ivec2(Coord1D%1024,Coord1D/1024);
	fColor=vec4(texelFetch(HDB,TC,0).xxx,1);
	}
//	*/
//*
	if(mode==2){
	ivec2 C=ivec2(Coord.xy/64);
	int Coord1D=C.x+C.y*8;
	ivec2 TC=ivec2(Coord1D%1024,Coord1D/1024);
	fColor=vec4(texelFetch(HDB,TC,0).xxx,1);
	}
//	*/

}
