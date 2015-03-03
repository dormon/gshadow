#version 430

#ifndef GRID_SIZE_X
#define GRID_SIZE_X 4
#endif//GRID_SIZE_X

#ifndef GRID_SIZE_Y
#define GRID_SIZE_Y 4
#endif//GRID_SIZE_Y

#ifndef BINDING_GRIDX
#define BINDING_GRIDX 6
#endif//BINDING_GRIDX

#ifndef BINDING_GRIDY
#define BINDING_GRIDY 7
#endif//BINDING_GRIDY

#ifndef SHADOWMAP_SIZE 
#define SHADOWMAP_SIZE 1024
#endif//SHADOWMAP_SIZE

layout(r32f,binding=BINDING_GRIDX)uniform image2D gridX;
layout(r32f,binding=BINDING_GRIDY)uniform image2D gridY;

vec2 unwarpx(in vec2 coord){
  vec2 result;
  result.y=coord.y;
  float gridPosition[GRID_SIZE_X+1];
  for(uint i=0;i<=GRID_SIZE_X;++i)
    gridPosition[i]=imageLoad(gridX,ivec2((result.y*.5+.5)*(SHADOWMAP_SIZE-1),i)).r;
  if(coord.x<gridPosition[0          ])coord.x=gridPosition[0          ];
  if(coord.x>gridPosition[GRID_SIZE_X])coord.x=gridPosition[GRID_SIZE_X];
  uint gridId=0;
  for(;gridId<GRID_SIZE_X;++gridId){
    if(coord.x>=gridPosition[gridId])break;
  }
  float a=gridPosition[gridId+0];
  float b=gridPosition[gridId+1];
  float ta=float(gridId+0)/GRID_SIZE_X*2.-1;
  float tb=float(gridId+1)/GRID_SIZE_X*2.-1;
  float t=(coord.x-a)/(b-a);

  if(b-a<=0){
    if(coord.x< 0)return vec2(-1,coord.y);
    if(coord.x>=0)return vec2(+1,coord.y);
  }

  result.x=ta+(tb-ta)*t;
  return result;
}

vec2 unwarpy(in vec2 coord){
  vec2 result;
  result.x=coord.x;
  float gridPosition[GRID_SIZE_Y+1];
  for(uint i=0;i<=GRID_SIZE_Y;++i)
    gridPosition[i]=imageLoad(gridY,ivec2((result.x*.5+.5)*(SHADOWMAP_SIZE-1),i)).r;
  if(coord.y<gridPosition[0          ])coord.y=gridPosition[0          ];
  if(coord.y>gridPosition[GRID_SIZE_Y])coord.y=gridPosition[GRID_SIZE_Y];
  uint gridId=0;
  for(;gridId<GRID_SIZE_Y;++gridId){
    if(coord.y>=gridPosition[gridId])break;
  }
  float a=gridPosition[gridId+0];
  float b=gridPosition[gridId+1];
  float ta=float(gridId+0)/GRID_SIZE_Y;
  float tb=float(gridId+1)/GRID_SIZE_Y;
  float t=(coord.y-a)/(b-a);

  if(b-a<=0){
    if(coord.y< 0)return vec2(coord.x,-1);
    if(coord.y>=0)return vec2(coord.x,+1);
  }

  result.y=ta+(tb-ta)*t;
  return result;
}



#define MY_PI 3.1415925

layout(location=0)out vec4 fColor;

layout(binding=1)uniform usampler2D       Color   ;
layout(binding=2)uniform  sampler2D       Position;
layout(binding=3)uniform  sampler2D       Normal  ;
layout(binding=4)uniform  sampler2D       Stencil ;
layout(binding=5)uniform  sampler2DShadow shadow  ;

uniform vec3 CameraPosition;//position of the camera
uniform mat4 BPV;//bias projection view
uniform mat4 v;
uniform mat4 p;

ivec2 Coord=ivec2(gl_FragCoord.xy);

uniform vec4 LightPosition;
uniform vec3 La;//light ambient color
uniform vec3 Ld;//light diffuse color
uniform vec3 Ls;//light specular color

uniform bool FrameBufferFlag;
uniform bool ImageAtomicAddFlag;

uvec4 color     = texelFetch(Color,Coord,0);
vec3  Ka        = vec3((color.xyz>>0)&0xff)/0xff;
vec3  Kd        = Ka;
vec3  Ks        = vec3((color.xyz>>8)&0xff)/0xff;
float Shininess = 50;//shininess factor
float specularFactor = color.w/255.;

vec3 ComputeDiffuseSpecular(){
	vec3  gPosition    = texelFetch(Position,Coord,0).xyz;
	vec3  gNormal      = texelFetch(Normal,Coord,0).xyz;
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

uniform bool wrap=false;

void main(){
	vec3 Sum=Ka*La;
	vec4 vShadowPos = BPV*vec4(texelFetch(Position,Coord,0).xyz,1);
  if(BPV[0].x==123123.321)return;
  vec4 view = v*vec4(texelFetch(Position,Coord,0).xyz,1);
  vShadowPos = p*view;

  float s=0;
  if(view.z>0||any(lessThan(vShadowPos.xyz,-vShadowPos.www))||any(greaterThan(vShadowPos.xyz,vShadowPos.www)))
    s=1;
  else{
    if(wrap)
      vShadowPos=vec4(/*unwarpy*/(unwarpx(vShadowPos.xy/vShadowPos.w))*vShadowPos.w,vShadowPos.zw);
	  s=textureProj(shadow,vec4((vShadowPos.xyz+vShadowPos.www)/2,vShadowPos.w)).x;
  }
  Sum+=s*ComputeDiffuseSpecular();
	fColor=vec4(Sum,1);//+edge;//output color
}

