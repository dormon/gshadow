#version 330
#extension GL_ARB_shading_language_420pack:enable

layout(binding=4)uniform sampler2D tex;

in vec2 vCoord;
in vec3 vPos;
in vec3 vN;

vec3 encode(vec3 n){
	float p=sqrt(n.z*8+8);
	return vec3(n.xy/p+.5,0);
}

layout(location=0)out vec4 fColor;
layout(location=1)out vec3 fPos;
layout(location=2)out vec3 fN;
void main(){
	fColor=texture(tex,vCoord);
	fPos=vPos;
	fN=normalize(vN);
	fN=encode(normalize(vN));

}
