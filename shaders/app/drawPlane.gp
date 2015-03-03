#version 440 core

layout(points)in;
layout(triangle_strip,max_vertices=12)out;

uniform vec4 a=vec4(1,0,0,0);
uniform float size=0;

uniform mat4 v   = mat4(1);
uniform mat4 p   = mat4(1);

void main(){
  vec3 center=(-a.w/dot(a.xyz,a.xyz))*a.xyz;
  vec3 v0;
  if(a.y==0&&a.z==0)v0=a.xyz+vec3(0,a.x,0);
  else v0=a.xyz+vec3(a.x,0,0);
  vec3 x=normalize(cross(a.xyz,v0));
  vec3 y=normalize(cross(a.xyz,x));


  mat4 vp=p*v;
  if(size==0){
    gl_Position=vp*vec4(center,1);EmitVertex();
    gl_Position=vp*vec4(x,size);EmitVertex();
    gl_Position=vp*vec4(y,size);EmitVertex();
    EndPrimitive();
    gl_Position=vp*vec4(center,1);EmitVertex();
    gl_Position=vp*vec4(y,size);EmitVertex();
    gl_Position=vp*vec4(-x,size);EmitVertex();
    EndPrimitive();
    gl_Position=vp*vec4(center,1);EmitVertex();
    gl_Position=vp*vec4(-x,size);EmitVertex();
    gl_Position=vp*vec4(-y,size);EmitVertex();
    EndPrimitive();
    gl_Position=vp*vec4(center,1);EmitVertex();
    gl_Position=vp*vec4(-y,size);EmitVertex();
    gl_Position=vp*vec4(x,size);EmitVertex();
    EndPrimitive();
  }else{
    x*=size;
    x+=center;
    y*=size;
    y+=center;
    gl_Position=vp*vec4(center,1);EmitVertex();
    gl_Position=vp*vec4(x,1);EmitVertex();
    gl_Position=vp*vec4(y,1);EmitVertex();
    EndPrimitive();
    gl_Position=vp*vec4(center,1);EmitVertex();
    gl_Position=vp*vec4(y,1);EmitVertex();
    gl_Position=vp*vec4(-x,1);EmitVertex();
    EndPrimitive();
    gl_Position=vp*vec4(center,1);EmitVertex();
    gl_Position=vp*vec4(-x,1);EmitVertex();
    gl_Position=vp*vec4(-y,1);EmitVertex();
    EndPrimitive();
    gl_Position=vp*vec4(center,1);EmitVertex();
    gl_Position=vp*vec4(-y,1);EmitVertex();
    gl_Position=vp*vec4(x,1);EmitVertex();
    EndPrimitive();
  }

}
