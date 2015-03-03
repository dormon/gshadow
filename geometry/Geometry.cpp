#include"Geometry.h"

#include<limits>

#include<iostream>
#include<sstream>

using namespace geometry;

ConvexHullPlanes::ConvexHullPlanes(ConvexHullPoints hull){
  if(hull.points.size()<4)return;
  for(unsigned i=0;i<hull.points.size()-2;++i){
    glm::vec3 a=hull.points[i];
    for(unsigned j=i+1;j<hull.points.size()-1;++j){
      glm::vec3 b=hull.points[j];
      for(unsigned k=j+1;k<hull.points.size();++k){
        //std::cerr<<i<<" "<<j<<" "<<k<<std::endl;
        glm::vec3 c=hull.points[k];
        Plane plane=Plane(a,b,c);
        int hp=hull.hullPlane(plane,i,j,k);
        //std::cerr<<"hp: "<<hp<<std::endl;
        if(hp<0){/*std::cerr<<"neg"<<std::endl;*/this->insert(plane.neg());}
        if(hp>0){/*std::cerr<<"normal"<<std::endl;*/this->insert(plane      );}
      }
    }
  }
}
ConvexHullPlanes::ConvexHullPlanes(){

}

ConvexHullPlanes::ConvexHullPlanes(glm::mat4 p,glm::mat4 v){
  geometry::Plane frustumPlanes[6];
  unsigned numFrustumPlanes=0;
  geometry::camera2Plane(
      frustumPlanes,
      &numFrustumPlanes,
      p,
      v);
  for(unsigned i=0;i<numFrustumPlanes;++i)
    this->insert(frustumPlanes[i]);
}
ConvexHullPlanes::ConvexHullPlanes(glm::vec3 minPoint,glm::vec3 maxPoint){
  glm::vec3 scenePoints[8];
  for(unsigned i=0;i<8;++i){
    glm::vec3 t=glm::vec3(float((i>>0)&1),float((i>>1)&1),float((i>>2)&1));
    glm::vec3 omt=glm::vec3(1.f)-t;
    scenePoints[i]=omt*minPoint+t*maxPoint;
  }
  this->insert(geometry::Plane(scenePoints[0],scenePoints[1],scenePoints[2]));
  this->insert(geometry::Plane(scenePoints[0],scenePoints[2],scenePoints[4]));
  this->insert(geometry::Plane(scenePoints[0],scenePoints[4],scenePoints[1]));
  this->insert(geometry::Plane(scenePoints[7],scenePoints[5],scenePoints[6]));
  this->insert(geometry::Plane(scenePoints[7],scenePoints[3],scenePoints[5]));
  this->insert(geometry::Plane(scenePoints[7],scenePoints[6],scenePoints[3]));
}

bool ConvexHullPlanes::inside(glm::vec3 point){
  for(unsigned p=0;p<this->planes.size();++p)
    if(this->planes[p].distance(point)<0)return false;
  return true;
}
bool ConvexHullPlanes::inside(glm::vec3 point,unsigned aPlane,unsigned bPlane,unsigned cPlane){
  for(unsigned p=0;p<this->planes.size();++p){
    if(p==aPlane||p==bPlane||p==cPlane)continue;
    if(this->planes[p].distance(point)<0)return false;
  }
  return true;
}

void ConvexHullPlanes::insert(Plane plane){
  for(unsigned p=0;p<this->planes.size();++p)
    if(this->planes[p]==plane||this->planes[p].neg()==plane)return;
  this->planes.push_back(plane);
}

std::string ConvexHullPlanes::toStr(){
  std::stringstream ss;
  for(unsigned p=0;p<this->planes.size();++p){
    ss<<"p"<<p<<": ";
    for(int k=0;k<4;++k)
      ss<<this->planes[p].data[k]<<" ";
    ss<<std::endl;
  }
  ss<<std::endl;

  return ss.str();
}


ConvexHullPoints geometry::intersect(ConvexHullPlanes a,ConvexHullPlanes b){
  ConvexHullPlanes c;
  for(unsigned i=0;i<a.planes.size();++i)
    c.insert(a.planes[i]);
  for(unsigned i=0;i<b.planes.size();++i)
    c.insert(b.planes[i]);
  return ConvexHullPoints(c);
}



ConvexHullPoints::ConvexHullPoints(){

}
ConvexHullPoints::ConvexHullPoints(ConvexHullPlanes hull){
  if(hull.planes.size()<3)return;
  for(unsigned i=0;i<hull.planes.size();++i)
    this->planes.push_back(hull.planes[i]);
  for(unsigned i=0;i<hull.planes.size()-2;++i){
    Plane a=hull.planes[i];
    for(unsigned j=i+1;j<hull.planes.size()-1;++j){
      Plane b=hull.planes[j];
      for(unsigned k=j+1;k<hull.planes.size();++k){
        Plane c=hull.planes[k];
        glm::vec3 p=geometry::planes2Point(a,b,c);
        if(std::isnan(p.x)||std::isnan(p.y)||std::isnan(p.z))continue;
        if(hull.inside(p,i,j,k)){
          this->aPlane.push_back(i);
          this->bPlane.push_back(j);
          this->cPlane.push_back(k);
          this->points.push_back(p);
        }
      }
    }
  }
}
std::string ConvexHullPoints::toStr(){
  std::stringstream ss;
  for(unsigned i=0;i<this->points.size();++i){
    ss<<"p"<<i<<": "
      <<this->aPlane[i]<<" "
      <<this->bPlane[i]<<" "
      <<this->cPlane[i]<<"   "
      <<this->points[i].x<<" "
      <<this->points[i].y<<" "
      <<this->points[i].z<<std::endl;
  }
  ss<<std::endl;
  return ss.str();
}


int ConvexHullPoints::hullPlane(Plane plane,unsigned ai,unsigned bi,unsigned ci){
  bool inFrontOf=false;
  bool behind=false;
  for(unsigned p=0;p<this->points.size();++p){
    if(p==ai)continue;
    if(p==bi)continue;
    if(p==ci)continue;
    float d=plane.distance(this->points[p]);
    if(d<0)behind=true;
    else inFrontOf=true;
    if(behind&&inFrontOf)return 0;
  }
  if(behind)return -1;
  return 1;
}

ConvexHullPointPlane::ConvexHullPointPlane(ConvexHullPoints hull){

  for(unsigned i=0;i<hull.planes.size();++i)
    this->planes.push_back(hull.planes[i]);

  std::vector<bool>processed;
  for(unsigned i=0;i<hull.points.size();++i)
    processed.push_back(false);

  for(unsigned i=0;i<hull.points.size();++i){
    if(processed[i])continue;
    processed[i]=true;
    std::vector<unsigned>currentPlanes;
    currentPlanes.push_back(hull.aPlane[i]);
    currentPlanes.push_back(hull.bPlane[i]);
    currentPlanes.push_back(hull.cPlane[i]);
    for(unsigned j=i+1;j<hull.points.size();++j){
      if(processed[i])continue;
      if(hull.points[i]==hull.points[j]){
        processed[j]=true;
        bool insertPlaneA=true;
        bool insertPlaneB=true;
        bool insertPlaneC=true;
        for(unsigned k=0;k<currentPlanes.size();++k){
          if(hull.aPlane[j]==currentPlanes[k])insertPlaneA=false;
          if(hull.bPlane[j]==currentPlanes[k])insertPlaneB=false;
          if(hull.cPlane[j]==currentPlanes[k])insertPlaneC=false;
        }
        if(insertPlaneA)currentPlanes.push_back(hull.aPlane[j]);
        if(insertPlaneB)currentPlanes.push_back(hull.bPlane[j]);
        if(insertPlaneC)currentPlanes.push_back(hull.cPlane[j]);
      }
    }
    if(this->offset.size()==0)this->offset.push_back(0);
    else this->offset.push_back(this->offset[this->offset.size()-1]+this->count[this->count.size()-1]);//currentPlanes.size());
    this->count.push_back(currentPlanes.size());

    for(unsigned p=0;p<currentPlanes.size();++p)
      this->planeIndices.push_back(currentPlanes[p]);

    this->points.push_back(hull.points[i]);
  }
}

std::string ConvexHullPointPlane::toStr(){
  std::stringstream ss;
  for(unsigned k=0;k<this->count.size();++k){
    ss<<"p"<<k<<": ";
    for(unsigned i=0;i<this->count[k];++i){
      ss<<this->planeIndices[this->offset[k]+i]<<" ";
    }
    ss<<std::endl;
  }
  ss<<std::endl;
  return ss.str();
}

ConvexHullFaces::ConvexHullFaces(ConvexHullPointPlane hull){
  for(unsigned i=0;i<hull.points.size();++i)
    this->points.push_back(hull.points[i]);

  std::vector<unsigned>currentPlanes;
  for(unsigned i=0;i<hull.planeIndices.size();++i){
    bool insertPlane=true;
    for(unsigned j=0;j<currentPlanes.size();++j)
      if(hull.planeIndices[i]==currentPlanes[j]){
        insertPlane=false;
        break;
      }
    if(insertPlane)currentPlanes.push_back(hull.planeIndices[i]);
  }
  for(unsigned i=0;i<currentPlanes.size();++i)
    this->planes.push_back(hull.planes[currentPlanes[i]]);

  for(unsigned p=0;p<currentPlanes.size();++p){
    unsigned counter=0;
    for(unsigned k=0;k<hull.count.size();++k)
      for(unsigned i=0;i<hull.count[k];++i)
        if(currentPlanes[p]==hull.planeIndices[hull.offset[k]+i]){
          this->index.push_back(k);
          counter++;
          break;
        }

    if(this->offset.size()==0)this->offset.push_back(0);
    else this->offset.push_back(this->offset[this->offset.size()-1]+this->count[this->count.size()-1]);
    this->count.push_back(counter);
  }
}

std::string ConvexHullFaces::toStr(){
  std::stringstream ss;
  //ss<<"asd: "<<this->index.size()<<std::endl;
  //for(unsigned i=0;i<this->index.size();++i)ss<<this->index[i]<<" ";
  //ss<<std::endl;
  //for(unsigned i=0;i<this->offset.size();++i)ss<<this->offset[i]<<":"<<this->count[i]<<" ";
  //ss<<std::endl;

  for(unsigned k=0;k<this->count.size();++k){
    ss<<"f"<<k<<": ";
    for(unsigned i=0;i<this->count[k];++i){
      ss<<this->index[this->offset[k]+i]<<" ";
    }
    ss<<std::endl;
  }
  ss<<std::endl;
  return ss.str();
}
ConvexHullTriangles::ConvexHullTriangles(ConvexHullPoints hull){
  geometry::ConvexHullPointPlane min0=geometry::ConvexHullPointPlane(hull);
  geometry::ConvexHullFaces      min1=geometry::ConvexHullFaces(min0);
  this->_constructor(min1);
}

ConvexHullTriangles::ConvexHullTriangles(ConvexHullPlanes hull){
  geometry::ConvexHullPoints     min0=geometry::ConvexHullPoints(hull);
  geometry::ConvexHullPointPlane min1=geometry::ConvexHullPointPlane(min0);
  geometry::ConvexHullFaces      min2=geometry::ConvexHullFaces(min1);
  this->_constructor(min2);
}

void ConvexHullTriangles::_constructor(ConvexHullFaces hull){
  ConvexHullSortedFaces sorted(hull);
  for(unsigned k=0;k<sorted.count.size();++k){
    for(unsigned i=2;i<sorted.count[k];++i){
      this->triangles.push_back(sorted.index[sorted.offset[k]+0  ]);
      this->triangles.push_back(sorted.index[sorted.offset[k]+i-1]);
      this->triangles.push_back(sorted.index[sorted.offset[k]+i  ]);
    }
  }
  for(unsigned p=0;p<hull.points.size();++p)
    this->points.push_back(hull.points[p]);
}


ConvexHullTriangles::ConvexHullTriangles(ConvexHullFaces hull){
  this->_constructor(hull);
}

std::string ConvexHullTriangles::toStr(){
  std::stringstream ss;
  for(unsigned t=0;t<this->triangles.size()/3;++t){
    ss<<"t"<<t<<": "
      <<this->triangles[t*3+0]<<" "
      <<this->triangles[t*3+1]<<" "
      <<this->triangles[t*3+2]<<std::endl;
  }
  ss<<std::endl;
  return ss.str();
}

void ConvexHullTriangles::draw(DrawPrimitive*drawer){
  for(unsigned t=0;t<this->triangles.size()/3;++t)
    drawer->triangle(
        this->points[this->triangles[t*3+0]],
        this->points[this->triangles[t*3+1]],
        this->points[this->triangles[t*3+2]]);
}


ConvexHullSortedFaces::ConvexHullSortedFaces(ConvexHullFaces hull){
  for(unsigned k=0;k<hull.count.size();++k){
    std::vector<unsigned>cursortedFace;
    glm::vec3 normal=glm::vec3(hull.planes[k].data);
    cursortedFace.push_back(hull.index[hull.offset[k]+0]);
    std::vector<bool>used;
    for(unsigned i=0;i<hull.count[k];++i)
      used.push_back(i==0);

    for(unsigned i=1;i<hull.count[k];++i)
      for(unsigned j=0;j<hull.count[k];++j){
        if(used[j])continue;
        glm::vec3 a=hull.points[cursortedFace[cursortedFace.size()-1]];
        glm::vec3 b=hull.points[hull.index[hull.offset[k]+j]];
        glm::vec3 c=hull.points[hull.index[hull.offset[k]+j]]+normal*glm::length(b-a);
        Plane plane(a,b,c);
        bool inFrontOf=false;
        unsigned skip=0;
        for(unsigned l=0;l<hull.count[k];++l){
          if(l==j||cursortedFace[cursortedFace.size()-1]==hull.index[hull.offset[k]+l]){skip++;continue;}
          if(plane.distance(hull.points[hull.index[hull.offset[k]+l]])>0){
            inFrontOf=true;
            break;
          }
        }

        if(!inFrontOf){
          used[j]=true;
          cursortedFace.push_back(hull.index[hull.offset[k]+j]);
          break;
        }
      }
    if(!this->offset.size())this->offset.push_back(0);
    else this->offset.push_back(this->offset[this->offset.size()-1]+this->count[this->count.size()-1]);

    this->count.push_back(cursortedFace.size());
    for(unsigned i=0;i<cursortedFace.size();++i)
      this->index.push_back(cursortedFace[i]);
  }

  for(unsigned i=0;i<hull.points.size();++i)
    this->points.push_back(hull.points[i]);

  for(unsigned i=0;i<hull.planes.size();++i)
    this->planes.push_back(hull.planes[i]); 
}

std::string ConvexHullSortedFaces::toStr(){
  std::stringstream ss;
  for(unsigned k=0;k<this->count.size();++k){
    ss<<"f"<<k<<": ";
    for(unsigned i=0;i<this->count[k];++i){
      ss<<this->index[this->offset[k]+i]<<" ";
    }
    ss<<std::endl;
  }
  ss<<std::endl;
  return ss.str();
}

ConvexHullLineSegments::ConvexHullLineSegments(ConvexHullSortedFaces hull){
  for(unsigned f=0;f<hull.count.size();++f){
    for(unsigned i=0;i<hull.count[f];++i){
      unsigned edgeA=hull.index[hull.offset[f]+i];
      unsigned edgeB=hull.index[hull.offset[f]+((i+1)%hull.count[f])];
      if(edgeA>edgeB){unsigned c=edgeA;edgeA=edgeB;edgeB=c;}
      bool finded=false;
      for(unsigned l=0;l<this->line.size();l+=2){
        if(this->line[l+0]==edgeA&&this->line[l+1]==edgeB){
          finded=true;
          break;
        }
      }
      if(!finded){
        this->line.push_back(edgeA);
        this->line.push_back(edgeB);
      }
    }
  }
  for(unsigned p=0;p<hull.points.size();++p)
    this->points.push_back(hull.points[p]);

  for(unsigned i=0;i<hull.planes.size();++i)
    this->planes.push_back(hull.planes[i]);
}

std::string ConvexHullLineSegments::toStr(){
  std::stringstream ss;
  for(unsigned l=0;l<this->line.size();l+=2)
    ss<<"l"<<l/2<<": "<<this->line[l+0]<<" "<<this->line[l+1]<<std::endl;
  ss<<std::endl;
  return ss.str();
}



ConvexHullPointLineSegments::ConvexHullPointLineSegments(ConvexHullSortedFaces hull){
  for(unsigned k=0;k<hull.count.size();++k){
    for(unsigned i=0;i<hull.count[k];++i){
      unsigned id=hull.index[hull.offset[k]+i];
      bool uniq=true;
      for(unsigned j=0;j<this->uniqPoints.size();++j)
        if(this->uniqPoints[j]==id){
          uniq=false;
          break;
        }
      if(uniq)this->uniqPoints.push_back(id);
    }
  }
  for(unsigned u=0;u<this->uniqPoints.size();++u){
    std::vector<unsigned>currentEdges;
    for(unsigned k=0;k<hull.count.size();++k){
      bool contain=false;
      unsigned uniqIndex=0;
      for(unsigned i=0;i<hull.count[k];++i)
        if(this->uniqPoints[u]==hull.index[hull.offset[k]+i]){
          uniqIndex=i;
          contain=true;
          break;
        }
      if(contain){
        unsigned edgeAPoint=hull.index[hull.offset[k]+((uniqIndex+1)%hull.count[k])];
        unsigned edgeBPoint=hull.index[hull.offset[k]+((uniqIndex+hull.count[k]-1)%hull.count[k])];

        bool ainside=false;
        bool binside=false;
        for(unsigned i=0;i<currentEdges.size();++i){
          if(currentEdges[i]==edgeAPoint)ainside=true;
          if(currentEdges[i]==edgeBPoint)binside=true;
        }

        if(!ainside)currentEdges.push_back(edgeAPoint);
        if(!binside)currentEdges.push_back(edgeBPoint);
      }
    }

    if(!this->offset.size())this->offset.push_back(0);
    else this->offset.push_back(this->offset[this->offset.size()-1]+this->count[this->count.size()-1]);
    this->count.push_back(currentEdges.size());
    for(unsigned i=0;i<currentEdges.size();++i)
      this->index.push_back(currentEdges[i]);
  }
  for(unsigned i=0;i<hull.points.size();++i)
    this->points.push_back(hull.points[i]);

  for(unsigned i=0;i<hull.planes.size();++i)
    this->planes.push_back(hull.planes[i]);

  for(unsigned u=0;u<this->uniqPoints.size();++u)
    this->lookUp.push_back(0);
  for(unsigned u=0;u<this->uniqPoints.size();++u)
    this->lookUp[this->uniqPoints[u]]=u;

}

std::string ConvexHullPointLineSegments::toStr(){
  std::stringstream ss;
  for(unsigned u=0;u<this->uniqPoints.size();++u){
    ss<<"u"<<u<<": ";
    for(unsigned i=0;i<this->count[u];++i){
      ss<<this->index[this->offset[u]+i]<<" ";
    }
    ss<<std::endl;
  }
  return ss.str();
}

bool ConvexHullSilhouette::isSilhouette(ConvexHullPointLineSegments hull,unsigned ai,unsigned bi,glm::vec3 light){
  glm::vec3 a=light;
  glm::vec3 b=hull.points[ai];
  glm::vec3 c=hull.points[bi];
  Plane plane(a,b,c);
  bool inFrontOf=false;
  bool behind=false;
  for(unsigned t=0;t<hull.uniqPoints.size();++t){
    if(hull.uniqPoints[t]==ai)continue;
    if(hull.uniqPoints[t]==bi)continue;
    float dist=plane.distance(hull.points[hull.uniqPoints[t]]);
    if(dist<0)behind=true;
    if(dist>0)inFrontOf=true;
    if(behind&&inFrontOf)break;
  }
  if(!(behind&&inFrontOf))return true;
  return false;
}

ConvexHullSilhouette::ConvexHullSilhouette(ConvexHullPointLineSegments hull,glm::vec3 light){
  this->light=light;
  unsigned prevUniq;
  unsigned actUniq;
  std::vector<bool>used;
  for(unsigned u=0;u<hull.uniqPoints.size();++u)used.push_back(false);

  for(unsigned u=0;u<hull.uniqPoints.size();++u){
    for(unsigned i=0;i<hull.count[u];++i){
      bool isSil=this->isSilhouette(hull,hull.uniqPoints[u],hull.index[hull.offset[u]+i],light);
      if(isSil){
        this->lineLoop.push_back(hull.uniqPoints[u]);
        this->lineLoop.push_back(hull.index[hull.offset[u]+i]);
        prevUniq = hull.uniqPoints[u];
        actUniq  = hull.index[hull.offset[u]+i];
        used[prevUniq]=true;
        used[actUniq ]=true;
        //break
        u=hull.uniqPoints.size();
        break;
      }
    }
  }

  bool running=true;
  do{
    unsigned start=hull.lookUp[actUniq];
    bool candidateExists=false;
    for(unsigned i=0;i<hull.count[start];++i){
      if(used[hull.index[hull.offset[start]+i]])continue;
      bool isSil=this->isSilhouette(hull,actUniq,hull.index[hull.offset[start]+i],light);
      if(isSil){
        this->lineLoop.push_back(hull.index[hull.offset[start]+i]);
        prevUniq = actUniq;
        actUniq  = hull.index[hull.offset[start]+i];
        used[actUniq ]=true;
        candidateExists=true;
        break;
      }
    }
    if(!candidateExists)running=false;
  }while(running);
  for(unsigned i=0;i<hull.points.size();++i)
    this->points.push_back(hull.points[i]);
}

std::string ConvexHullSilhouette::toStr(){
  std::stringstream ss;
  ss<<"loop: ";
  for(unsigned l=0;l<this->lineLoop.size();++l)
    ss<<this->lineLoop[l]<<" ";
  ss<<std::endl;
  return ss.str();
}

bool MinFrustumPlanes::isOutside(ConvexHullSilhouette hull,Plane plane,unsigned ai,unsigned bi){
  bool inFrontOf=false;
  bool behind=false;
  for(unsigned l=0;l<hull.lineLoop.size();++l){
    if(l==ai)continue;
    if(l==bi)continue;
    if(plane.distance(hull.points[l])<0)behind=true;
    else inFrontOf=true;
  }
  if(inFrontOf&&behind)return false;
  return true;
}

bool MinFrustumPlanes::isInFrontOf(ConvexHullSilhouette hull,Plane plane,unsigned ai){
  for(unsigned i=0;i<hull.points.size();++i){
    if(i==ai)continue;
    if(plane.distance(hull.points[i])<0)return false;
  }
  return true;
}

MinFrustumPlanes::MinFrustumPlanes(ConvexHullSilhouette hull){
  float bestArea=10e10;

  for(unsigned l=0;l<hull.lineLoop.size()*0+1;++l){
    Plane planeL,planeR,planeB,planeT;

    glm::vec3 a=hull.points[hull.lineLoop[(l+0)%hull.lineLoop.size()]];
    glm::vec3 b=hull.points[hull.lineLoop[(l+1)%hull.lineLoop.size()]];
    glm::vec3 c=hull.light;
    glm::vec3 x=hull.points[hull.lineLoop[(l+2)%hull.lineLoop.size()]];
    planeL=Plane(a,b,c);
    if(planeL.distance(x)<0)planeL=planeL.neg();

    for(unsigned i=0;i<hull.lineLoop.size();++i){
      if(i==l)continue;
      if(i==((l+1)%hull.lineLoop.size()))continue;
      glm::vec3 d=hull.points[hull.lineLoop[i]];
      x=hull.points[hull.lineLoop[l]];
      planeR=Plane(d,d+b-a,c);
      if(planeR.distance(x)<0)planeR=planeR.neg();
      if(!this->isOutside(hull,planeR,i,i))continue;
      else break;
    }

    unsigned tIndex;
    for(unsigned i=0;i<hull.lineLoop.size();++i){
      if(i==l)continue;
      glm::vec3 d=hull.points[hull.lineLoop[i]];
      x=hull.points[hull.lineLoop[l]];
      planeT=Plane(d,d+glm::vec3(planeL.data),c);
      if(planeT.distance(x)<0)planeT=planeT.neg();
      if(!this->isOutside(hull,planeT,i,i))continue;
      else{tIndex=i;break;}
    }

    for(unsigned i=0;i<hull.lineLoop.size();++i){
      if(i==l)continue;
      if(i==tIndex)continue;
      glm::vec3 d=hull.points[hull.lineLoop[i]];
      x=hull.points[hull.lineLoop[l]];
      planeB=Plane(d,d+glm::vec3(planeL.data),c);
      if(planeB.distance(x)<0)planeB=planeB.neg();
      if(!this->isOutside(hull,planeB,i,i))continue;
      else break;
    }
    float alpha = glm::acos(glm::dot(glm::vec3(planeL.data),glm::vec3(-planeR.data)));
    float beta  = glm::acos(glm::dot(glm::vec3(planeB.data),glm::vec3(-planeT.data)));

    float area=glm::tan(alpha/2)*glm::tan(beta/2);
    if(area<bestArea){
      bestArea=area;
      this->bestL=planeL;
      this->bestR=planeR;
      this->bestB=planeB;
      this->bestT=planeT;
    }
  }
  glm::vec3 x=glm::normalize(glm::vec3(this->bestL.data-this->bestR.data));
  glm::vec3 y=glm::normalize(glm::vec3(this->bestB.data-this->bestT.data));
  glm::vec3 nf=-glm::normalize(glm::cross(x,y));
  glm::vec3 nn=-nf;
  for(unsigned i=0;i<hull.points.size();++i){
    Plane np=Plane();
    np.data.x=nn.x;
    np.data.y=nn.y;
    np.data.z=nn.z;
    np.data.w=-glm::dot(nn,hull.points[i]);
    if(this->isInFrontOf(hull,np,i)){
      this->bestN=np;
      break;
    }
  }
  for(unsigned i=0;i<hull.points.size();++i){
    Plane fp=Plane();
    fp.data.x=nf.x;
    fp.data.y=nf.y;
    fp.data.z=nf.z;
    fp.data.w=-glm::dot(nf,hull.points[i]);
    if(this->isInFrontOf(hull,fp,i)){
      this->bestF=fp;
      break;
    }
  }
  this->light=hull.light;
  this->computeMatrices();
}
bool MinFrustumPlanes::isOutside(ConvexHullLineSegments hull,Plane plane,unsigned ai,unsigned bi){
  bool inFrontOf=false;
  bool behind=false;
  for(unsigned l=0;l<hull.points.size();++l){
    if(l==ai)continue;
    if(l==bi)continue;
    float d=plane.distance(hull.points[l]);
    if(glm::abs(d)<0.0001)continue;
    if(d<0)behind=true;
    else inFrontOf=true;
  }
  if(inFrontOf&&behind)return false;
  return true;
}

bool isColinear(glm::vec3 a,glm::vec3 b,glm::vec3 c,float t){
  return glm::acos(glm::abs(glm::dot(glm::normalize(b-a),glm::normalize(a-c))))<t;
}


MinFrustumPlanes::MinFrustumPlanes(ConvexHullLineSegments hull,glm::vec3 light){
  this->light=light;

  float bestArea=10e10;

  this->bestL=Plane(glm::vec4(1.f,0.f,0.f,1.f));
  this->bestR=Plane(glm::vec4(-1.f,0.f,0.f,1.f));
  this->bestB=Plane(glm::vec4(0.f,1.f,0.f,1.f));
  this->bestT=Plane(glm::vec4(0.f,-1.f,0.f,1.f));
  this->bestN.data=glm::vec4(0.f,0.f,-1.f,1.f);
  this->bestF.data=glm::vec4(0.f,0.f,1.f,1.f);

  glm::vec3 center=glm::vec3(0.f);
  for(unsigned p=0;p<hull.points.size();++p)
    center+=hull.points[p];
  center/=(float)hull.points.size();


  glm::vec3 nf=glm::normalize(this->light-center);
  glm::vec3 nn=-nf;

  float nearest=10e10;
  float farest=0;
  for(unsigned i=0;i<hull.points.size();++i){
    Plane np=Plane();
    np.data.x=nn.x;
    np.data.y=nn.y;
    np.data.z=nn.z;
    np.data.w=-glm::dot(nn,hull.points[i]);

    Plane fp=Plane();
    fp.data.x=nf.x;
    fp.data.y=nf.y;
    fp.data.z=nf.z;
    fp.data.w=-glm::dot(nf,hull.points[i]);

    if(nearest>glm::abs(np.distance(this->light))){
      nearest=glm::abs(np.distance(this->light));
      if(np.distance(center)<0)this->bestN=np.neg();
      else this->bestN=np;
    }
    if(farest<glm::abs(fp.distance(this->light))){
      farest=glm::abs(fp.distance(this->light));
      if(fp.distance(center)<0)this->bestF=fp.neg();
      else this->bestF=fp;
    }
  }

  //near far ready

  for(unsigned l=0;l<hull.line.size();l+=2){
    Plane planeL(glm::vec4(1.f,0.f,0.f,1.f));
    Plane planeR(glm::vec4(-1.f,0.f,0.f,1.f));
    Plane planeB(glm::vec4(0.f,1.f,0.f,1.f));
    Plane planeT(glm::vec4(0.f,-1.f,0.f,1.f));

    unsigned ai=hull.line[l+0];
    unsigned bi=hull.line[l+1];
    glm::vec3 a=hull.points[ai];
    glm::vec3 b=hull.points[bi];
    glm::vec3 c=light;
    if(isColinear(a,b,c,glm::pi<float>()/180)){
      //std::cerr<<"colinear"<<std::endl;
      continue;
    }

    planeL=Plane(a,b,c);
    if(planeL.distance(center)<0)planeL=planeL.neg();

    if(!this->isOutside(hull,planeL,ai,bi)){
      //std::cerr<<"non outside"<<std::endl;
      continue;//this is not silhouette edge
    }

    if(planeL.isNan()){
      //std::cerr<<"planeL: rozbita"<<std::endl;
      continue;
    }

    for(unsigned i=0;i<hull.points.size();++i){
      if(i==ai)continue;
      if(i==bi)continue;
      glm::vec3 d=hull.points[i];

      planeR=Plane(d,d+(b-a),c);
      planeR=Plane(d,d+glm::cross(glm::vec3(this->bestN.data),glm::vec3(planeL.data)),c);

      //std::cerr<<"old: "<<planeR.toStr()<<std::endl;


      if(planeR.distance(center)<0)planeR=planeR.neg();
      if(planeR==planeL)continue;
      if(planeR==planeL.neg())continue;
      if(!this->isOutside(hull,planeR,i,i))continue;
      else{
        //std::cerr<<"lr dist: "<<glm::length(planeR.data-planeL.data)<<std::endl;
        break;
      }
    }

    if(planeR.isNan()){
      //std::cerr<<"planeR: rozbita"<<std::endl;
      continue;
    }

    glm::vec3 n1=glm::normalize(glm::cross(glm::vec3(planeL.data),glm::vec3(this->bestN.data)));
    //glm::vec3 n2=glm::normalize(glm::cross(glm::vec3(-planeR.data),glm::vec3(this->bestN.data)));

    //std::cerr<<"n1: "<<n1.x<<" "<<n1.y<<" "<<n1.z<<std::endl;
    //std::cerr<<"n2: "<<n2.x<<" "<<n2.y<<" "<<n2.z<<std::endl;






    glm::vec3 topNormal=glm::normalize(glm::cross(glm::vec3(planeL.data),glm::vec3(planeR.data)));

    glm::vec3 vr=glm::normalize(glm::vec3(planeL.data)-glm::vec3(planeR.data));

    vr= glm::cross(glm::vec3(this->bestN.data),n1);

    for(unsigned i=0;i<hull.points.size();++i){
      glm::vec3 d=hull.points[i];
      planeT=Plane(d,d+vr,c);
      if(planeT.distance(center)<0)planeT=planeT.neg();
      if(!this->isOutside(hull,planeT,i,i))continue;
      else if(glm::dot(glm::vec3(planeT.data),topNormal)<0)continue;
      else break;
    }

    if(planeT.isNan()){
      //std::cerr<<"planeT: rozbita"<<std::endl;
      continue;
    }

    for(unsigned i=0;i<hull.points.size();++i){
      glm::vec3 d=hull.points[i];
      planeB=Plane(d,d+vr,c);
      if(planeB.distance(center)<0)planeB=planeB.neg();
      if(planeB==planeT)continue;
      if(planeB==planeT.neg())continue;
      if(!this->isOutside(hull,planeB,i,i))continue;
      else if(glm::dot(glm::vec3(planeB.data),topNormal)>0)continue;
      else{
        //std::cerr<<"bt dist: "<<glm::length(planeB.data-planeT.data)<<std::endl;
        break;
      }
    }

    if(planeB.isNan()){
      //std::cerr<<"planeB: rozbita"<<std::endl;
      continue;
    }


    float alpha = glm::acos(glm::dot(glm::vec3(planeL.data),glm::vec3(-planeR.data)));
    float beta  = glm::acos(glm::dot(glm::vec3(planeB.data),glm::vec3(-planeT.data)));

    float area=glm::tan(alpha/2)*glm::tan(beta/2);
    if(area==0)continue;

    //std::cerr<<"actArea: "<<area<<std::endl;
    if(area<bestArea){
      bestArea=area;
      this->bestL=planeL;
      this->bestR=planeR;
      this->bestB=planeB;
      this->bestT=planeT;
    }
  }

  //std::cerr<<"L: "<<this->bestL.toStr()<<std::endl;
  //std::cerr<<"R: "<<this->bestR.toStr()<<std::endl;
  //std::cerr<<"B: "<<this->bestB.toStr()<<std::endl;
  //std::cerr<<"T: "<<this->bestT.toStr()<<std::endl;

  //std::cerr<<"area: "<<bestArea<<std::endl;
  //glm::vec3 x=glm::normalize(glm::vec3(this->bestL.data-this->bestR.data));
  //glm::vec3 y=glm::normalize(glm::vec3(this->bestB.data-this->bestT.data));
  //glm::vec3 nf=-glm::normalize(glm::cross(x,y));
  //glm::vec3 nn=-nf;

  this->computeMatrices();
}


void MinFrustumPlanes::computeMatrices(){
  glm::vec3 camPos     = this->light;
  glm::vec3 viewvector = glm::normalize(glm::vec3(this->bestN.data));
  glm::vec3 x          = glm::normalize(glm::vec3(this->bestL.data-this->bestR.data));
  glm::vec3 upVector   = glm::normalize(glm::cross(x,viewvector));
  this->view = glm::lookAt(camPos,camPos+viewvector,upVector);
  float near = glm::abs(this->bestN.distance(this->light));
  float far  = glm::abs(this->bestF.distance(this->light));

  //glm::frustum()
  glm::vec3 nlb=geometry::planes2Point(this->bestN,this->bestL,this->bestB);
  glm::vec3 nlt=geometry::planes2Point(this->bestN,this->bestL,this->bestT);
  glm::vec3 nrb=geometry::planes2Point(this->bestN,this->bestR,this->bestB);
  glm::vec3 center = this->light+glm::vec3(this->bestN.data)*near;
  glm::vec3 xx=nrb-nlb;
  glm::vec3 yy=nlt-nlb;
  glm::vec3 v=center-nlb;
  float xSize=glm::length(xx);
  float ySize=glm::length(yy);
  float xOffset=glm::dot(glm::normalize(xx),v);
  float yOffset=glm::dot(glm::normalize(yy),v);
  float left   = -xOffset;
  float bottom = -yOffset;
  float right  = left+xSize;
  float top    = bottom+ySize;
  this->projection = glm::frustum(left,right,bottom,top,near,far);
}

geometry::ConvexHullPoints geometry::getExtendedHull(
    glm::mat4 p,
    glm::mat4 v,
    glm::vec3 minPoint,
    glm::vec3 maxPoint,
    glm::vec3 light){
  geometry::ConvexHullPlanes cameraHull(p,v);
  geometry::ConvexHullPlanes sceneHull (minPoint,maxPoint);
  geometry::ConvexHullPoints cameraSceneHullPoints = geometry::intersect(cameraHull,sceneHull);
  geometry::ConvexHullPlanes cameraSceneHullPlanes(cameraSceneHullPoints);
  if(cameraSceneHullPlanes.inside(light)){
    return cameraSceneHullPoints;
  }
  cameraSceneHullPoints.points.push_back(light);
  geometry::ConvexHullPlanes cameraSceneExtendedHullPlanes(cameraSceneHullPoints);

  geometry::ConvexHullPoints cameraSceneExtendedHullPoints = 
    geometry::intersect(cameraSceneExtendedHullPlanes,sceneHull);
  return cameraSceneExtendedHullPoints;
}

bool geometry::getMinimalVP(
    glm::mat4*lp,
    glm::mat4*lv,
    glm::mat4 p,
    glm::mat4 v,
    glm::vec3 minPoint,
    glm::vec3 maxPoint,
    glm::vec3 light){

  geometry::ConvexHullPlanes cameraHull(p,v);
  geometry::ConvexHullPlanes sceneHull (minPoint,maxPoint);
  geometry::ConvexHullPoints cameraSceneHullPoints = geometry::intersect(cameraHull,sceneHull);
  geometry::ConvexHullPlanes cameraSceneHullPlanes(cameraSceneHullPoints);

  //std::cerr<<cameraSceneHullPoints.toStr()<<std::endl;
  //std::cerr<<cameraSceneHullPlanes.toStr()<<std::endl;

  if(cameraSceneHullPlanes.inside(light)){
    //std::cerr<<"inside"<<std::endl;
    return false;//omnidirectional light
  }

  cameraSceneHullPoints.points.push_back(light);
  geometry::ConvexHullPlanes cameraSceneExtendedHullPlanes(cameraSceneHullPoints);

  //geometry::ConvexHullPoints cameraSceneExtendedHullPoints = 
  //  geometry::intersect(cameraSceneExtendedHullPlanes,sceneHull);

  geometry::ConvexHullPoints cameraSceneExtendedHullPoints=geometry::getExtendedHull(
      p,
      v,
      minPoint,
      maxPoint,
      light);

  geometry::ConvexHullPointPlane        cameraSceneHullPointPlane  (cameraSceneExtendedHullPoints);
  //geometry::ConvexHullPointPlane        cameraSceneHullPointPlane  (cameraSceneHullPoints);
  geometry::ConvexHullFaces             cameraSceneHullFaces       (cameraSceneHullPointPlane);
  geometry::ConvexHullSortedFaces       cameraSceneHullSortedFaces (cameraSceneHullFaces);
  geometry::ConvexHullLineSegments      cameraSceneHullLineSegments(cameraSceneHullSortedFaces);
  geometry::MinFrustumPlanes            cameraSceneMinFrustum(cameraSceneHullLineSegments,light);

  *lp=cameraSceneMinFrustum.projection;
  *lv=cameraSceneMinFrustum.view;
  return true;
}

