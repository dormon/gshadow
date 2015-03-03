#version 430

#define MY_PI 3.1415925

layout(location=0)out vec4 fColor;

layout(binding=1)uniform sampler2D Color;
layout(binding=2)uniform sampler2D Position;
layout(binding=3)uniform sampler2D Normal;
layout(binding=4)uniform sampler2D Stencil;
layout(binding=5)uniform samplerCube shadow;

uniform vec3 CameraPosition;//position of the camera

ivec2 Coord=ivec2(gl_FragCoord.xy);

uniform vec4 LightPosition;
uniform vec3 La;//light ambient color
uniform vec3 Ld;//light diffuse color
uniform vec3 Ls;//light specular color

uniform bool FrameBufferFlag;
uniform bool ImageAtomicAddFlag;

vec3  Ka           = texelFetch(Color,Coord,0).rgb;

vec3  Kd        = Ka;//material diffuse color
vec3  Ks        = vec3(1);//material specular color
float Shininess = 50;//shininess factor

ivec3 Shuffle(ivec3 A,ivec3 Max){
	return 
	ivec3(
		(A.x*13+1337)%Max.x,
		(A.y*17+33137)%Max.y,
		(A.z*23+51337)%Max.z
	);
}

vec3  gPosition    = texelFetch(Position,Coord,0).xyz;
vec3  gNormal      = texelFetch(Normal,Coord,0).xyz;

vec3 ComputeDiffuseSpecular(){
	vec3 Sum=vec3(0);
	vec3 N=normalize(gNormal);//normal
	vec3 V=normalize(CameraPosition-gPosition);//view vector
	vec3 L=normalize(LightPosition.xyz-gPosition*LightPosition.w);//light vector
	vec3 R=reflect(-L,N);//reflected light vector
	float DiffuseFactor=max(dot(L,N),0);
	Sum+=Kd*Ld*DiffuseFactor;//diffuse component
	if(DiffuseFactor>0)
		Sum+=Ks*Ls*pow(max(dot(R,V),0),Shininess);//specular component
	float dsl=length(LightPosition.xyz-gPosition*LightPosition.w);
	float dvs=length(CameraPosition-gPosition);
	float dvl=length(LightPosition.xyz-CameraPosition*LightPosition.w);
	return Sum;
}

vec3 VisualizeFrameBuffer(float StencilValue){
	vec3 Sum=vec3(0);
	float b=0;
	if(abs(StencilValue)>100){
		b=abs(StencilValue)/1000;
		Sum=vec3(0,0,b);
	}else
		Sum=vec3(max(StencilValue,0)/10,max(-StencilValue,0)/10,0);
	return Sum;
}

float linearize(float d){
  float near=1;
  float far=1000;
  //d=(f+n)/(f-n)+2*n*f/(f-n)/z;
  //d-(f+n)/(f-n) = 2*n*f/(f-n)/z;
  //z= 2*n*f/((d-(f+n)/(f-n) )*(f-n))
  //z= 2*n*f/( d*(f-n) - (f+n) )
  float z= 2*near*far/(d*(far-near)-(far+near));
  return (2.0 * near) / (far + near - d * (far - near));
  return abs(z);
  if(d==1)return 1;
  else return 0;
}

void main(){
	vec3 Sum=vec3(0);


	Sum+=Ka*La;//ambient component

	float s;
  float depth=length(gPosition-LightPosition.xyz);
	//s=texture(shadow,vec4(normalize(gPosition-LightPosition.xyz),depth)).x;

	//if(s>0){
		Sum+=s*ComputeDiffuseSpecular();
	//}
	fColor=vec4(Sum,1);//+edge;//output color
	//fColor=vec4(texture(shadow,gl_FragCoord.xy).xyz/10,1);
  float shadowMapDepth=texture(shadow,vec3(gPosition-LightPosition.xyz)).r;
  fColor=vec4(linearize(shadowMapDepth));
  //fColor=vec4(shadowMapDepth);
  //if(shadowMapDepth==1)fColor=vec4(1);
  //else fColor=vec4(0,1,0,1);

}

