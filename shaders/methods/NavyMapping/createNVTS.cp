#version 440 core

layout(vertices=3)out;

layout(binding=2)uniform sampler2D desiredView;
layout(binding=3)uniform sampler2D smoothX;
layout(binding=4)uniform sampler2D smoothY;

uniform uint tessFactor=32u;

uniform bool cullTriangles=false;

bool isEdgeVisible(vec2 a,vec2 b){
  float mint=min(min((-1-a.x)/(b.x-a.x),(-1-a.y)/(b.y-a.y)),0);
  float maxt=max(max((+1-a.x)/(b.x-a.x),(+1-a.y)/(b.y-a.y)),1);
  return mint<maxt;
}

bool isPointVisible(vec2 a){
  return all(bvec4(greaterThan(a,vec2(-1.0)),lessThan(a,vec2(+1.0))));
}

bool pointsVisible(vec2 a,vec2 b,vec2 c){
  if((!isPointVisible(a))&&(!isPointVisible(b))&&(!isPointVisible(c)))return false;
  return true;
  /*
  if(isEdgeVisible(a,b))return true;
  if(isEdgeVisible(a,c))return true;
  if(isEdgeVisible(b,c))return true;
  return false;*/
}

void main(){
  gl_out[gl_InvocationID].gl_Position=gl_in[gl_InvocationID].gl_Position;

  if(gl_InvocationID==0){
    vec4 clipA=nv_getFinal(gl_in[0].gl_Position,desiredView,smoothX,smoothY);
    vec4 clipB=nv_getFinal(gl_in[1].gl_Position,desiredView,smoothX,smoothY);
    vec4 clipC=nv_getFinal(gl_in[2].gl_Position,desiredView,smoothX,smoothY);
    /*
    clipA = gl_in[0].gl_Position;
    clipB = gl_in[1].gl_Position;
    clipC = gl_in[2].gl_Position;
    */

    vec3 A=clipA.xyz/clipA.w;
    vec3 B=clipB.xyz/clipB.w;
    vec3 C=clipC.xyz/clipC.w;

    if(cullTriangles){
      if(!pointsVisible(A.xy,B.xy,C.xy)){
        gl_TessLevelOuter[0]=0;
        gl_TessLevelOuter[1]=0;
        gl_TessLevelOuter[2]=0;
        gl_TessLevelInner[0]=0;
        return;
      }
    }

    float a=distance(B.xy,C.xy);
    float b=distance(A.xy,C.xy);
    float c=distance(A.xy,B.xy);
    float piece=2./float(tessFactor);
    float tessA=min(max(ceil(a/piece),1.0),float(tessFactor));
    float tessB=min(max(ceil(b/piece),1.0),float(tessFactor));
    float tessC=min(max(ceil(c/piece),1.0),float(tessFactor));
    gl_TessLevelOuter[0]=tessA;
    gl_TessLevelOuter[1]=tessB;
    gl_TessLevelOuter[2]=tessC;
    gl_TessLevelInner[0]=max(tessA,max(tessB,tessC));
  }

}
