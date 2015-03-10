#version 440 core

layout(location=0)out vec4 fColor;
layout(binding=0)uniform usampler2D tex;

in vec2 gCoord;

uniform uint minValue = 0u;
uniform uint maxValue = 1u;
uniform uint channel  = 0u;

void main(){
  uint val=texture(tex,gCoord)[channel];
  float v=float(val-minValue)/float(maxValue-minValue);
  fColor=getHeatColor(v);
}
