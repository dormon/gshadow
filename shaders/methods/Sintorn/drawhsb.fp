#version 430

#define UINT_BIT_SIZE 32


#ifndef MAX_LEVELS
  #define MAX_LEVELS 4
#endif//MAX_LEVELS

//#define TESTSIZE (4096/2)

#define RESULT_LENGTH_IN_UINT         (WAVEFRONT_SIZE/UINT_BIT_SIZE)


layout(location=0)out vec4 fColor;
layout(std430,binding=0)buffer HDB{uvec2 HSBData[];};
layout(binding=16)uniform usampler2D HSBTexture;
layout(r32ui,binding=0)readonly uniform uimage2D FinalStencilMask;
layout(r32ui,binding=10)readonly uniform uimage2D HST;
layout(rg32ui,binding=11)uniform uimage2D HSBImage;


uniform uvec2 TileDivisibility[MAX_LEVELS];
uniform uvec2 TileSizeInPixels[MAX_LEVELS];
uniform uint  LevelOffset     [MAX_LEVELS];
uniform uint  NumLevels;
uniform uint  Level=0;
uniform uint  DrawNear=0;

uvec2 Coord=uvec2(gl_FragCoord.xy);

uint GetData(uint Index,uint l){
  uint DataIndex  = (Index/WAVEFRONT_SIZE);//+LevelOffset[Level]+int(Level>0);
  if(l>0)DataIndex+=LevelOffset[l-1]+1;
  uint BitIndex   = Index%WAVEFRONT_SIZE;
  //uvec2 Data      = texelFetch(HSBTexture,ivec2(DataIndex%TESTSIZE,DataIndex/TESTSIZE),0).rg;
  uvec2 Data      = imageLoad(HSBImage,ivec2(DataIndex%TESTSIZE,DataIndex/TESTSIZE)).rg;
  uint ResultUint = Data[BitIndex/UINT_BIT_SIZE];
  return (ResultUint>>(BitIndex%UINT_BIT_SIZE))&1u;
}

uint ComputeSampleIndex1D(uvec2 C,uint L){
  uvec2 ActCoord=C;
  uint SampleIndex1D=0;

  uvec2 CW=ActCoord/TileSizeInPixels[0];
  uint TileID1Da=uint(dot(CW.xy,uvec2(1,TileDivisibility[0].x)));
  SampleIndex1D+=TileID1Da; 
  ActCoord&=(TileSizeInPixels[0]-1);

  for(uint level=1;level<=L;++level){//loop over levels
    SampleIndex1D*=WAVEFRONT_SIZE;
    //SampleIndex1D+=WAVEFRONT_SIZE;
    uvec2 CW=ActCoord/TileSizeInPixels[level];
    uint TileID1D=uint(dot(CW.xy,uvec2(1,TileDivisibility[level].x)));
    SampleIndex1D+=TileID1D;
    ActCoord&=(TileSizeInPixels[level]-1);
  }
  return SampleIndex1D;

}

void main(){
  if(Level>=NumLevels){
    fColor=vec4(1,0,0,1);
    uint S=imageLoad(FinalStencilMask,ivec2(Coord)).x;
    fColor=vec4(1-S,1,0,1);
    return;
  }
  //*
    uvec2 cc=(Coord)%uvec2(8);
    uint invocationId=cc.y*8+cc.x;
    uint stencilValue=imageLoad(HST,ivec2((Coord.x/8)*RESULT_LENGTH_IN_UINT+(invocationId/UINT_BIT_SIZE),(Coord.y/8))).x;
    uint shadow=(stencilValue>>(invocationId%UINT_BIT_SIZE))&1u;
    fColor=vec4(1-shadow,1,0,1);
    return;
// */

  /*
    uvec2 cc=(Coord/uvec2(8))%uvec2(8);
    uint invocationId=cc.y*8+cc.x;
    uint stencilValue=imageLoad(HST,ivec2((Coord.x/64)*RESULT_LENGTH_IN_UINT+(invocationId/UINT_BIT_SIZE),(Coord.y/64))).x;
    uint shadow=(stencilValue>>(invocationId%UINT_BIT_SIZE))&1u;
    fColor=vec4(1-shadow,1,0,1);
    return;
// */

  /*
    uvec2 cc=Coord/uvec2(64);
    uint invocationId=cc.y*8+cc.x;
    uint stencilValue=imageLoad(HST,ivec2(0*RESULT_LENGTH_IN_UINT+(invocationId/UINT_BIT_SIZE),0)).x;
    //stencilValue=imageLoad(HST,ivec2(0,0)).x;
    uint shadow=(stencilValue>>(invocationId%UINT_BIT_SIZE))&1u;
    //shadow=uint(stencilValue>0);
    fColor=vec4(1-shadow,1,0,1);
    return;
// */
  //*
    uint S=imageLoad(FinalStencilMask,ivec2(Coord)).x;
    fColor=vec4(1-S,1,0,1);
    return;
//*/

  uint Data=0;
  for(uint l=Level;l<=Level;++l)
    Data+=GetData(ComputeSampleIndex1D(Coord,l),l);
  //if(LevelOffset[Level]==64+64*64&&Level==2){fColor=vec4(1,0,1,1);return;}
  //if(Data>1)fColor=vec4(1,1,1,1);
  //else fColor=vec4(abs(1-Data));
  if(Data>0)fColor=vec4(0,0,0,1);
  else fColor=vec4(1,1,1,1);
  //fColor=vec4(sin(ComputeSampleIndex1D(Coord,Level)*0.001));
  //uint S=ComputeSampleIndex1D(Coord,Level);
  //fColor=vec4((S&0x3F)/64.,S/(64.*64.),0,1);
  //if(SampleIndex1D<64)fColor=vec4(0,1,0,1);
  //if(SampleIndex1D>64*100&&SampleIndex1D<64*101)fColor=vec4(0,1,0,1);
  //if(Level==0||Level==1)fColor=vec4(1,1,0,1);
  //fColor=vec4(float(SampleIndex1D)/float(64),0,0,1);

}
