#version 440 core

layout(points)in;
layout(line_strip,max_vertices=16)out;

uniform vec3 minPoint;
uniform vec3 maxPoint;

uniform mat4 mvp;

vec3 getPoint(uint id){
  vec3 t=vec3((id>>0)&1,(id>>1)&1,(id>>2)&1);
  return minPoint*(1-t)+maxPoint*t;
}

void main(){
  gl_Position=mvp*vec4(getPoint(0),1);EmitVertex();
  gl_Position=mvp*vec4(getPoint(1),1);EmitVertex();
  gl_Position=mvp*vec4(getPoint(3),1);EmitVertex();
  gl_Position=mvp*vec4(getPoint(2),1);EmitVertex();
  gl_Position=mvp*vec4(getPoint(0),1);EmitVertex();
  gl_Position=mvp*vec4(getPoint(4),1);EmitVertex();
  gl_Position=mvp*vec4(getPoint(5),1);EmitVertex();
  gl_Position=mvp*vec4(getPoint(7),1);EmitVertex();
  gl_Position=mvp*vec4(getPoint(6),1);EmitVertex();
  gl_Position=mvp*vec4(getPoint(4),1);EmitVertex();
  EndPrimitive();
  gl_Position=mvp*vec4(getPoint(1),1);EmitVertex();
  gl_Position=mvp*vec4(getPoint(5),1);EmitVertex();
  EndPrimitive();
  gl_Position=mvp*vec4(getPoint(2),1);EmitVertex();
  gl_Position=mvp*vec4(getPoint(6),1);EmitVertex();
  EndPrimitive();
  gl_Position=mvp*vec4(getPoint(3),1);EmitVertex();
  gl_Position=mvp*vec4(getPoint(7),1);EmitVertex();
  EndPrimitive();
}
