#version 430

layout(location=0)out vec4 fColor;

in float z;

void main(){
	fColor=vec4(0,0,abs(z)/1000,1);
}
