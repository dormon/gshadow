#version 430

layout(binding=8)uniform sampler2DRect Texture;
layout(location=0)out vec4 fColor;

in vec2 gCoord;


void main(){
	float n=0.1;
	float f=10000;
	//fColor=vec4(2*n/(f+n-texture(Texture,gCoord).r*(f-n)));
	fColor=vec4(texture(Texture,gCoord));

}
