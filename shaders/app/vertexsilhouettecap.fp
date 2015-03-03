#version 100

void main(){
	if(gl_FrontFacing)
		gl_FragColor=vec4(0,1,0,1);
	else
		gl_FragColor=vec4(0,0,1,1);

}
