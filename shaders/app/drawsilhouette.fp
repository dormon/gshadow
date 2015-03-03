#version 430

in vec4 gColor;

layout(location=0)out vec4 fColor;

void main(){
	fColor=gColor;
}
