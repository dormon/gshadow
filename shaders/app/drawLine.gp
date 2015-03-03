#version 440 core

layout(points)in;
layout(line_strip,max_vertices=2)out;

uniform vec3 a=vec3(0,0,0);
uniform vec3 b=vec3(0,0,0);


uniform mat4 v   = mat4(1);
uniform mat4 p   = mat4(1);

uniform float size=.01f;

void main(){
  mat4 vp=p*v;
  gl_Position=vp*vec4(a,1);EmitVertex();
  gl_Position=vp*vec4(b,1);EmitVertex();
  EndPrimitive();
}
