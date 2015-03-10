#version 440 core

layout(location=0)out vec4 fColor;
layout(binding=0)uniform sampler2D tex;

in vec2 gCoord;

uniform float minValue=0.f;
uniform float maxValue=1.f;
uniform uint channel=0;

void main(){
  float val=texture(tex,gCoord)[channel];
  val=(val-minValue)/(maxValue-minValue);
  fColor=getHeatColor(val);
}
