#version 430

#ifndef MAX_LEVELS
  #define MAX_LEVELS 4
#endif//MAX_LEVELS

#ifndef DRAWHDB_BINDING_HDBIMAGE
  #define DRAWHDB_BINDING_HDBIMAGE 1
#endif//DRAWHDB_BINDING_HDBIMAGE

#ifndef DRAWHDB_BINDING_HDT
  #define DRAWHDB_BINDING_HDT 2
#endif//DRAWHDB_BINDING_HDT

#ifndef TESTSIZE
  #error you have to define TESTSIZE - resolution of texture
#endif//TESTSIZE

layout(location=0)out vec4 fColor;
layout(rg32f ,binding=DRAWHDB_BINDING_HDBIMAGE)uniform image2D HDBImage;
layout(rg32f ,binding=DRAWHDB_BINDING_HDT     )uniform image2D HDT[MAX_LEVELS];

uniform uvec2 TileDivisibility[MAX_LEVELS];
uniform uvec2 TileSizeInPixels[MAX_LEVELS];
uniform uint  LevelOffset     [MAX_LEVELS];
uniform uint  NumLevels;
uniform uint  Level=0;
uniform uint  DrawNear=0;

uvec2 Coord=uvec2(gl_FragCoord.xy);


float Linearize(float d,float n,float f){
  //return 2*n/(f+n+d*(f-n));
  //return 2*f*n/((d+1)*n+(1-d)*f);
  return -2*n/(d-1)/100;
}


vec2 GetData(uint Index){
  return imageLoad(   HDBImage,ivec2(Index%TESTSIZE,Index/TESTSIZE)).xy;
  //return texelFetch(HDBTexture,ivec2(Index%TESTSIZE,Index/TESTSIZE),0).xy;
}


void main(){
  if(Level>=NumLevels){fColor=vec4(1,0,0,1);return;}


  uvec2 ActCoord=Coord;
  uint SampleIndex1D=0;

  uvec2 CW=ActCoord/TileSizeInPixels[0];
  uint TileID1D=uint(dot(CW.xy,uvec2(1,TileDivisibility[0].x)));
  SampleIndex1D+=TileID1D; 
  ActCoord&=(TileSizeInPixels[0]-1);

  for(uint level=1;level<=Level;++level){//loop over levels
    SampleIndex1D*=WAVEFRONT_SIZE;
    //SampleIndex1D+=WAVEFRONT_SIZE;
    uvec2 CW=ActCoord/TileSizeInPixels[level];
    uint TileID1D=uint(dot(CW.xy,uvec2(1,TileDivisibility[level].x)));
    SampleIndex1D+=TileID1D;
    ActCoord&=(TileSizeInPixels[level]-1);
  }

  vec2 Data=GetData(LevelOffset[Level]+SampleIndex1D);
  Data=imageLoad(HDT[Level],ivec2(Coord/TileSizeInPixels[Level])).xy;
  //if(LevelOffset[Level]==64+64*64&&Level==2){fColor=vec4(1,0,1,1);return;}
  fColor=vec4(Linearize(Data[DrawNear],.1,1000));
  //fColor=vec4((SampleIndex1D%64)/64.,SampleIndex1D/(64*64.)*0,0,1);
  //if(SampleIndex1D<64)fColor=vec4(0,1,0,1);
  //if(SampleIndex1D>64*100&&SampleIndex1D<64*101)fColor=vec4(0,1,0,1);
  //if(Level==0||Level==1)fColor=vec4(1,1,0,1);
  //fColor=vec4(float(SampleIndex1D)/float(64),0,0,1);

}
