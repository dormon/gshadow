#version 330
#extension GL_ARB_shading_language_420pack: enable
layout(location=0)out vec4 fragColor;
layout(binding=0)uniform sampler2D TexColor;
uniform vec3 AmbientColor;
void main(){
	fragColor=texelFetch(TexColor,ivec2(gl_FragCoord),0)*vec4(AmbientColor,1);
}
