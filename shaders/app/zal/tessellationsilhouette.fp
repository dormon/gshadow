#version 430

layout(location=0)out vec4 fColor;

void main(){
	if(gl_FrontFacing)
		fColor=vec4(0,1,0,1);
	else
		fColor=vec4(1,0,0,1);

}
