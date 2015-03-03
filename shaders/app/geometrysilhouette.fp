#version 430

/////////////////////////////////////////////////////////////////////
// Options:
// if you define something, it will be applied.
// if you don't define something, opposite will be applied.
// Some defines has value that has to be set properly.
// Version number has to be chosen by user, preprocessor
// cannot do it (nothing useful can precede #version N P).
/////////////////////////////////////////////////////////////////////

//use extension: GL_AMD_shader_stencil_value_export
//define/not define 
//#define USE_STENCIL_VALUE_EXPORT
//use layouts - define/don't define
//#define USE_LAYOUTS
//visualize front and back faces with different color
//#define VISUALIZE

// DO NOT CHANGE ANYTHING BELOW THIS COMMENT!!!
//UNLESS, OF COURSE, YOU KNOW WHAT YOU ARE DOING.





/////////////////////////////////////////////////////////////////////
// TODO extensions and checks
/////////////////////////////////////////////////////////////////////

#ifdef USE_STENCIL_VALUE_EXPORT
  #extension GL_ARB_shader_stencil_export:enable
#endif





/////////////////////////////////////////////////////////////////////
// Inputs
/////////////////////////////////////////////////////////////////////

#ifdef USE_STENCIL_VALUE_EXPORT
flat in int gMultiplicity;
#endif//USE_STENCIL_VALUE_EXPORT






/////////////////////////////////////////////////////////////////////
// Outputs from geometry shader
/////////////////////////////////////////////////////////////////////

#ifdef VISUALIZE
  #ifdef USE_LAYOUTS
layout(location=0)out vec4 fColor;
  #else//USE_LAYOUTS
out vec4 fColor;
  #endif//USE_LAYOUTS
#endif//VISUALIZE





/////////////////////////////////////////////////////////////////////
// Main function
/////////////////////////////////////////////////////////////////////

void main(){
#ifdef USE_STENCIL_VALUE_EXPORT
  gl_FragStencilValueAMD=gMultiplicity; 
#endif//USE_STENCIL_VALUE_EXPORT

#ifdef VISUALIZE
  if(gl_FrontFacing)fColor=vec4(0,1,0,1);
  else fColor=vec4(1,0,0,1);
#endif//VISUALIZE
}
