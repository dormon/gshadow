#version 430

layout(binding=0)uniform sampler2D OverDrawTexture;
//layout(binding=1)uniform sampler1D ColorGradientTexture;

ivec2 Coord=ivec2(gl_FragCoord.xy);

uniform float MaxOverDraw=255;

layout(location=0)out vec4 fColor;

void main(){
	float OverDraw=texture(OverDrawTexture,Coord).r;
	float t=clamp(OverDraw/MaxOverDraw,0,1);
	fColor=vec4(t);//texture(ColorGradientTexture,t);
}
