#ifndef _METHODSPARAM_HPP_
#define _METHODSPARAM_HPP_

#include<iostream>

typedef struct{
  bool Deterministic;
  bool ReferenceEdge;
  bool Universal;
  bool UseVS2GSArray;
  bool UseVertexArray;
  bool UseStencilValueExport;
  bool CullSides;
  bool FrontCapToInfinity;
  unsigned Version;
}SGeometryParam;

bool operator!=(const SGeometryParam&a, const SGeometryParam&b);

typedef struct{
  bool ReferenceEdge;
  bool CullSides;
  bool UseStencilValueExport;
}STessellationParam;

bool operator!=(const STessellationParam&a, const STessellationParam&b);

typedef struct{
  unsigned WorkGroupSize;
  bool CullSides;
}SComputeSOEParam;

bool operator!=(const SComputeSOEParam&a, const SComputeSOEParam&b);

typedef struct{
  unsigned WorkGroupSize;
  bool CullSides;
}SComputeParam;

bool operator!=(const SComputeParam&a, const SComputeParam&b);

struct SShadowMapParam{
  unsigned Resolution;
  float Fovy;
  float FocusPoint[3];
  SShadowMapParam&operator=(const SShadowMapParam&a);
  bool operator!=(const SShadowMapParam&b)const;
};


struct SSintornParam{
  unsigned WavefrontSize;
  unsigned ShadowFrustaPerWorkGroup;
};

bool operator!=(const SSintornParam&a,const SSintornParam&b);

#endif//_METHODSPARAM_HPP_
