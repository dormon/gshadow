#version 440 core

layout(vertices=3)out;

uniform uint tessFactor=32u;

bool isEdgeVisible(vec2 a,vec2 b){
  float mint=min(min((-1-a.x)/(b.x-a.x),(-1-a.y)/(b.y-a.y)),0);
  float maxt=max(max((+1-a.x)/(b.x-a.x),(+1-a.y)/(b.y-a.y)),1);
  return mint<maxt;
}

bool pointsVisible(vec2 a,vec2 b,vec2 c){
  if(isEdgeVisible(a,b))return true;
  if(isEdgeVisible(a,c))return true;
  if(isEdgeVisible(b,c))return true;
  return false;
}

void main(){
  gl_out[gl_InvocationID].gl_Position=gl_in[gl_InvocationID].gl_Position;
  if(gl_InvocationID==0){
    vec4 clipA=nv_getFinal(gl_in[0].gl_Position);
    vec4 clipB=nv_getFinal(gl_in[1].gl_Position);
    vec4 clipC=nv_getFinal(gl_in[2].gl_Position);


    vec3 A=clipA.xyz/clipA.w;
    vec3 B=clipB.xyz/clipB.w;
    vec3 C=clipC.xyz/clipC.w;

    if(!pointsVisible(A.xy,B.xy,C.xy)){
      gl_TessLevelOuter[0]=0;
      gl_TessLevelOuter[1]=0;
      gl_TessLevelOuter[2]=0;
      gl_TessLevelInner[0]=0;
      return;
    }

    float a=distance(B,C);
    float b=distance(A,C);
    float c=distance(A,B);
    
    /*
    float pa=distance(gl_in[0].gl_Position.xy/gl_in[0].gl_Position.w,gl_in[1].gl_Position.xy/gl_in[1].gl_Position.w);
    float pb=distance(gl_in[0].gl_Position.xy/gl_in[0].gl_Position.w,gl_in[2].gl_Position.xy/gl_in[2].gl_Position.w);
    float pc=distance(gl_in[1].gl_Position.xy/gl_in[1].gl_Position.w,gl_in[2].gl_Position.xy/gl_in[2].gl_Position.w);

    float tessA=min(max(pa/a,1),float(tessFactor));
    float tessB=min(max(pb/b,1),float(tessFactor));
    float tessC=min(max(pc/c,1),float(tessFactor));
    */
    
    float piece=2./float(tessFactor);
    float tessA=min(max(ceil(a/piece),1),float(tessFactor));
    float tessB=min(max(ceil(b/piece),1),float(tessFactor));
    float tessC=min(max(ceil(c/piece),1),float(tessFactor));
    
    gl_TessLevelOuter[0]=tessA;
    gl_TessLevelOuter[1]=tessB;
    gl_TessLevelOuter[2]=tessC;
    gl_TessLevelInner[0]=max(tessA,max(tessB,tessC));
  }

}
