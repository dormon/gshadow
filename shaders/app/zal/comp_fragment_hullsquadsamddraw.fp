#version 430
layout(early_fragment_tests)in;

layout(location=0)out vec4 fragColor;

flat in float eMultiplicity;

void main(){
	gl_FragStencilValueAMD=int(eMultiplicity);
	fragColor=vec4(1);
}
