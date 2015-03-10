#version 440 core

layout(location=0)out vec4 fColor;
layout(binding=0)uniform sampler1D tex;

in vec2 gCoord;

uniform float size=0.01f;
uniform float minValue=0.f;
uniform float maxValue=1.f;
uniform uint  channel=0;

void main(){
  float val=texture(tex,gCoord.x)[channel];
  val=(val-minValue)/(maxValue-minValue);
  if(abs(gCoord.y-val)<size)fColor=vec4(1.f);
  else fColor=vec4(0.f);
}
