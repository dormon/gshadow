#include"methodsparam.hpp"

bool operator!=(const SGeometryParam&a, const SGeometryParam&b){
  return
    (a.Deterministic!=b.Deterministic)||
    (a.ReferenceEdge!=b.ReferenceEdge)||
    (a.Universal!=b.Universal)||
    (a.UseVS2GSArray!=b.UseVS2GSArray)||
    (a.UseVertexArray!=b.UseVertexArray)||
    (a.UseStencilValueExport!=b.UseStencilValueExport)||
    (a.CullSides!=b.CullSides)||
    (a.FrontCapToInfinity!=b.FrontCapToInfinity)||
    (a.Version!=b.Version);
}

bool operator!=(const STessellationParam&a, const STessellationParam&b){
  return
    (a.ReferenceEdge!=b.ReferenceEdge)||
    (a.CullSides!=b.ReferenceEdge) ||
    (a.UseStencilValueExport!=b.UseStencilValueExport);
}

bool operator!=(const SComputeSOEParam&a, const SComputeSOEParam&b){
  return
    (a.WorkGroupSize!=b.WorkGroupSize)||
    (a.CullSides!=b.CullSides);
}

bool operator!=(const SComputeParam&a, const SComputeParam&b){
  return
    (a.WorkGroupSize!=b.WorkGroupSize)||
    (a.CullSides!=b.CullSides);
}
/*
bool operator!=(const SShadowMapParam&a, const SShadowMapParam&b){
  return
    (a.Resolution!=b.Resolution)||
    (a.Fovy!=b.Fovy)||
    (a.FocusPoint[0]!=b.FocusPoint[0])||
    (a.FocusPoint[1]!=b.FocusPoint[1])||
    (a.FocusPoint[2]!=b.FocusPoint[2]);
}
*/

bool operator!=(const SShadowMapParam&a,const SShadowMapParam&b){
  //std::cerr<<"POROVNANI\n";
  return
    (a.Resolution!=b.Resolution)||
    (a.Fovy!=b.Fovy)||
    (a.FocusPoint[0]!=b.FocusPoint[0])||
    (a.FocusPoint[1]!=b.FocusPoint[1])||
    (a.FocusPoint[2]!=b.FocusPoint[2]);
}

bool SShadowMapParam::operator!=(const SShadowMapParam&b)const{
  //std::cerr<<"POROVNANITHIS\n";
  return
    (this->Resolution!=b.Resolution)||
    (this->Fovy!=b.Fovy)||
    (this->FocusPoint[0]!=b.FocusPoint[0])||
    (this->FocusPoint[1]!=b.FocusPoint[1])||
    (this->FocusPoint[2]!=b.FocusPoint[2]);
}

SShadowMapParam&SShadowMapParam::operator=(const SShadowMapParam&a){
  //std::cerr<<"PRIRAZENI\n";
  this->Resolution=a.Resolution;
  this->Fovy=a.Fovy;
  this->FocusPoint[0]=a.FocusPoint[0];
  this->FocusPoint[1]=a.FocusPoint[1];
  this->FocusPoint[2]=a.FocusPoint[2];
  //std::cerr<<"JOJOJO\n";
  return*this;
}

bool operator!=(const SSintornParam&a,const SSintornParam&b){
  return
    (a.WavefrontSize!=b.WavefrontSize)||
    (a.ShadowFrustaPerWorkGroup!=b.ShadowFrustaPerWorkGroup);
}

