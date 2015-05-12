#version 330

layout(location=0)out uvec4 fColor;//output color
layout(location=1)out vec3  fPosition;//output position
layout(location=2)out vec4  fNormal;//output normal

layout(std430,binding=0)buffer BColor{struct SColor{vec4 diffuse;vec4 specular;}colors[];};

in vec3 gPosition;//vertex position in world space
in vec3 gNormal;//vertex normal in world space

vec3 Ka=vec3(.5);//material ambient color

//flat in vec4 gPlane;
flat in uint gDI;

void main(){
  vec3  diffuseColor   = vec3(.5,float(gDI%10)/float(9),0);
  vec3  specularColor  = vec3(1);
  vec3  normal         = gNormal;
  vec3  lightmap       = vec3(1);
  float specularFactor = 1;

  uvec4 color  = uvec4(0);
  color.xyz   += uvec3(diffuseColor *lightmap*0xff);
  color.xyz   += uvec3(specularColor*lightmap*0xff)<<8;
  color.w      = uint (specularFactor*0xff);

	fColor    = color;
	fPosition = gPosition;
	fNormal   = vec4(normal,-dot(gPosition,normal));
}
