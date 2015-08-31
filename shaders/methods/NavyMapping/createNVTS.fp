#version 440 core

layout(location=0)out vec4 fColor;

in float z;

void main(){
#ifdef USE_PARABOLOID
  gl_FragDepth=z;
#endif//USE_PARABOLOID
  fColor=vec4(1);
}
