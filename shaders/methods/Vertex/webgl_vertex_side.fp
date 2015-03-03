#version 100

#ifndef OUTPUT_COLOR
#define OUTPUT_COLOR vec4(1.0,0.0,1.0,1.0)
#endif//OUTPUT_COLOR

#if __VERSION__ >= 130
out vec4 fColor;
#endif

void main(){
#if __VERSION__ < 130
  gl_FragColor=OUTPUT_COLOR;
#else
  fColor=OUTPUT_COLOR;
#endif
}
