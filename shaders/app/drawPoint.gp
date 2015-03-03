#version 440 core

layout(points)in;
layout(triangle_strip,max_vertices=4)out;

uniform vec3 a=vec3(0,0,0);

uniform mat4 v   = mat4(1);
uniform mat4 p   = mat4(1);

uniform float size=.01f;

void main(){
  mat4 vp=p*v;
  vec4 center=vp*vec4(a,1);
  for(uint i=0;i<4;++i){
    gl_Position=center+p*vec4(-1.+2.*(i%2),-1.+2.*(i/2),0,0)*size;EmitVertex();
    //gl_Position=vec4(vec2(-1.+2.*(i%2),-1.+2.*(i/2))*(size*0+0.1),-1,1);EmitVertex();
  }
  EndPrimitive();
}
