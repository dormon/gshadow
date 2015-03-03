#version 330

#extension GL_ARB_shading_language_420pack:enable
layout(location=0)out vec4 fColor;//output color

flat in float gMultiplicity;

void main(){
	//fColor=vec4(gMultiplicity);
	//fColor=vec4(-10);
	//fColor=vec4((1-2*float(gl_FrontFacing))*gMultiplicity);
	if(gl_FrontFacing)
		fColor=vec4(-gMultiplicity);
	else
		fColor=vec4(gMultiplicity);
}
