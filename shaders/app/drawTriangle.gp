#version 440 core

layout(points)in;
layout(triangle_strip,max_vertices=3)out;

uniform vec3 a=vec3(0,0,0);
uniform vec3 b=vec3(0,1,0);
uniform vec3 c=vec3(1,1,0);

uniform mat4 v   = mat4(1);
uniform mat4 p   = mat4(1);

out vec4 normal;

void main(){
  mat4 vp=p*v;
  vec4 n=(vp*vec4(normalize(cross(b-a,c-a)),0));
  normal=n;gl_Position=vp*vec4(a,1);EmitVertex();
  normal=n;gl_Position=vp*vec4(b,1);EmitVertex();
  normal=n;gl_Position=vp*vec4(c,1);EmitVertex();
  EndPrimitive();
}
