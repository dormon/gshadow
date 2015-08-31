#version 440 core
layout(points)in;
layout(triangle_strip,max_vertices=4)out;

out vec2 gCoord;

uniform int transId=0;

vec2 transform(vec2 co){
  if(transId==0)return vec2(  co.x,  co.y);
  if(transId==1)return vec2(1-co.x,  co.y);
  if(transId==2)return vec2(  co.x,1-co.y);
  if(transId==3)return vec2(1-co.x,1-co.y);
  if(transId==4)return vec2(  co.y,  co.x);
  if(transId==5)return vec2(1-co.y,  co.x);
  if(transId==6)return vec2(  co.y,1-co.x);
  if(transId==7)return vec2(1-co.y,1-co.x);
  return co;
}

void main(){
	for(int i=0;i<4;++i){
		gl_Position=vec4(-1.f+2.f*float(i%2),-1.f+2.f*float(i/2),-1.f,1.f);
    gCoord=vec2(i%2,i/2);
		EmitVertex();
	}
	EndPrimitive();
}
