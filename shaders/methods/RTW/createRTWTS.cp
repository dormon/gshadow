#version 440 core

layout(vertices=3)out;

uniform uint tessFactor=32u;

void main(){
  gl_out[gl_InvocationID].gl_Position=gl_in[gl_InvocationID].gl_Position;
  if(gl_InvocationID==0){
    vec3 A=gl_in[0].gl_Position.xyz/gl_in[0].gl_Position.w;
    vec3 B=gl_in[1].gl_Position.xyz/gl_in[1].gl_Position.w;
    vec3 C=gl_in[2].gl_Position.xyz/gl_in[2].gl_Position.w;
    float a=distance(B,C);
    float b=distance(A,C);
    float c=distance(A,B);
    float piece=2./float(tessFactor);
    float tessA=min(max(ceil(a/piece),1),64);
    float tessB=min(max(ceil(b/piece),1),64);
    float tessC=min(max(ceil(c/piece),1),64);
    gl_TessLevelOuter[0]=tessA;
    gl_TessLevelOuter[1]=tessB;
    gl_TessLevelOuter[2]=tessC;
    gl_TessLevelInner[0]=max(tessA,max(tessB,tessC));
  }

}
