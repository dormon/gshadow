#include"WorkingObjects.h"

#include<limits>

#include<iostream>
#include<sstream>

using namespace geometry;

PointC::PointC(glm::vec3 point){
  this->index=0;
  this->point=point;
}

PointC::PointC(glm::vec3 point,PlaneC*a,PlaneC*b,PlaneC*c){
  this->index=0;
  this->point=point;
  this->creators.push_back(a->plane);
  this->creators.push_back(b->plane);
  this->creators.push_back(c->plane);
}

bool PointC::pushOnlyPlane(PlaneC*p){
  for(unsigned i=0;i<this->planes.size();++i)
    if(this->planes[i]==p)return false;
  this->planes.push_back(p);
  return true;
}

bool PointC::pushOnlyCreator(Plane p){
  for(unsigned i=0;i<this->creators.size();++i)
    if(this->creators[i]==p)return false;
  this->creators.push_back(p);
  return true;
}

bool PointC::removePlane(PlaneC*p){
  bool found=false;
  for(unsigned i=0;i<this->planes.size();++i){
    if(this->planes[i]==p){
      this->planes[i]=this->planes[this->planes.size()-1];
      found=true;
      break;
    }
  }
  if(found){
    this->planes.pop_back();
    return true;
  }
  return false;
}

bool PointC::doesPlanesContain(Plane plane){
  for(unsigned i=0;i<this->planes.size();++i)
    if(glm::all(glm::equal(this->planes[i]->plane.data,plane.data)))return true;
//    if(this->planes[i]->plane.data==plane.data)return true;
  return false;
}

bool PointC::doesCreatorsContain(Plane plane){
  for(unsigned i=0;i<this->creators.size();++i)
    if(glm::all(glm::equal(this->creators[i].data,plane.data)))return true;
//    if(this->creators[i].data==plane.data)return true;
  return false;
}

void PointC::setIndex(unsigned index){
  this->index=index;
}

std::string PointC::toStr(){
  std::stringstream ss;
  ss<<this->index<<" ";
  //ss<<this->planes.size()<<" ";
  for(unsigned i=0;i<this->planes.size();++i)ss<<this->planes[i]->index<<" ";
  ss<<std::endl;
  return ss.str();
}

std::string PointC::dataToStr(){
  std::stringstream ss;
  ss<<this->point[0]<<" "<<this->point[1]<<" "<<this->point[2];
  return ss.str();
}
std::string PointC::planesToStr(){
  std::stringstream ss;
  for(unsigned i=0;i<this->planes.size();++i)
    ss<<this->planes[i]->index<<" ";
  return ss.str();
}

bool PointC::onLineSegment(PlaneC**a,PlaneC**b,PointC*p){
  if((void*)this==(void*)p)return false;
  unsigned nofShared=0;
  for(unsigned i=0;i<this->planes.size();++i)
    if(this->planes[i]->contain(p)){
      if(nofShared==0)if(a)*a=this->planes[i];
      if(nofShared==1)if(b)*b=this->planes[i];
      nofShared++;
    }
  if(nofShared>=2)return true;
  return false;
}

bool PointC::eqByCreators(PointC*p){
  unsigned nofShared=0;
  for(unsigned i=0;i<this->creators.size();++i)
    for(unsigned j=0;j<p->creators.size();++j)
      if(this->creators[i].data==p->creators[j].data)
        nofShared++;
  return nofShared>2;
}

bool PointC::eqByPlanes(PlaneC*pa,PlaneC*pb,PlaneC*pc){
  bool aOk=false;
  bool bOk=false;
  bool cOk=false;
  for(unsigned i=0;i<this->planes.size();++i){
    if(this->planes[i]->plane==pa->plane)aOk=true;
    if(this->planes[i]->plane==pb->plane)bOk=true;
    if(this->planes[i]->plane==pc->plane)cOk=true;
  }
  return aOk&&bOk&&cOk;
}

bool PointC::eqByAny(PointC*p){
  if(this->point==p->point)return true;
  unsigned nofSame=0;
  for(unsigned i=0;i<this->planes.size();++i)
    for(unsigned j=0;j<p->planes.size();++j)
      if(this->planes[i]->plane.data==p->planes[j]->plane.data)
        nofSame++;
  if(nofSame>2)return true;

  for(unsigned i=0;i<this->planes.size();++i)
    for(unsigned j=0;j<p->creators.size();++j)
      if(this->planes[i]->plane.data==p->creators[j].data)
        nofSame++;
  if(nofSame>2)return true;

  for(unsigned i=0;i<this->creators.size();++i)
    for(unsigned j=0;j<p->planes.size();++j)
      if(this->creators[i].data==p->planes[j]->plane.data)
        nofSame++;
  if(nofSame>2)return true;

  for(unsigned i=0;i<this->creators.size();++i)
    for(unsigned j=0;j<p->creators.size();++j)
      if(this->creators[i].data==p->creators[j].data)
        nofSame++;
  if(nofSame>2)return true;
  return false;
}
void PointC::addCreator(PointC*p){
  if(this->point!=p->point)return;
  for(unsigned i=0;i<p->creators.size();++i)
    this->pushOnlyCreator(p->creators[i]);
}
void PointC::addPlanes(PointC*p){
  for(unsigned i=0;i<p->planes.size();++i)
    this->pushOnlyPlane(p->planes[i]);
}










PlaneC::PlaneC(Plane plane){
  this->index=0;
  this->plane=plane;
}

PlaneC::PlaneC(Plane plane,PointC*a,PointC*b,PointC*c){
  this->index=0;
  this->plane=plane;
  this->points.push_back(a);
  this->points.push_back(b);
  this->points.push_back(c);
}

bool PlaneC::eq(PlaneC*p){
  return this->plane.data==p->plane.data;
}

void PlaneC::pushPoint(PointC*p){
  this->points.push_back(p);
}


void PlaneC::removePoint(PointC*p){
  bool found=false;
  for(unsigned i=0;i<this->points.size();++i){
    if(((void*)this->points[i])==((void*)p)){
      this->points[i]=this->points[this->points.size()-1];
      found=true;
      break;
    }
  }
  if(found)this->points.pop_back();
}

void PlaneC::removePoints(std::vector<PointC*>&p){
  for(unsigned i=0;i<p.size();++i)
    this->removePoint(p[i]);
}



void PlaneC::setIndex(unsigned index){
  this->index=index;
}

bool PlaneC::contain(PointC*p){
  for(unsigned i=0;i<this->points.size();++i)
    if(((void*)this->points[i])==((void*)p))return true;
  return false;
}

bool PlaneC::contain3(std::vector<PointC*>&p){
  unsigned nofContaining=0;
  for(unsigned i=0;i<p.size();++i){
    if(this->contain(p[i]))nofContaining++;
    if(nofContaining>2)return true;
  }
  return false;
}


void PlaneC::sortPoints(){
  //return;
  std::vector<PointC*>sorted;
  sorted.push_back(this->points[0]);
  PointC*current=this->points[0];

  std::vector<bool>used;
  for(unsigned i=0;i<this->points.size();++i)
    used.push_back(i==0);

  /*
  std::cerr<<"end1 "<<this->points.size()<<std::endl;
  std::cerr<<this->pointsToStr()<<std::endl;
  std::cerr<<"this plane: "<<this->index<<std::endl;
  for(unsigned i=0;i<this->points.size();++i){
    std::cerr<<"  "<<this->points[i]->index<<" ";
    for(unsigned j=0;j<this->points[i]->planes.size();++j){
      std::cerr<<this->points[i]->planes[j]->index<<" ";
    }
    std::cerr<<" "<<this->points[i]->dataToStr();
    std::cerr<<std::endl;
  }
  // */
  while(sorted.size()!=this->points.size()){
    for(unsigned j=0;j<this->points.size();++j){
      if(used[j])continue;
      PlaneC*pa;
      PlaneC*pb;
      if(current->onLineSegment(&pa,&pb,this->points[j])){
        /*
        std::cerr<<current->index<<" : "<<this->points[j]->index
          <<" - "<<
          pa->index<<" : "<<pb->index<<std::endl;
        // */
        sorted.push_back(this->points[j]);
        used[j]=true;
        current=this->points[j];
        break;
      }
    }
  }
  //std::cerr<<"end2"<<std::endl;

  bool inOrder;
  unsigned ai=0;
  unsigned bi=sorted.size()/3;
  unsigned ci=(2*sorted.size())/3;
  glm::vec3 a=sorted[ai]->point;
  glm::vec3 b=sorted[bi]->point;
  glm::vec3 c=sorted[ci]->point;

  inOrder=glm::dot(glm::normalize(glm::cross(b-a,c-b)),glm::vec3(this->plane.data))>0;


  this->points.clear();
  if(inOrder)
    for(unsigned i=0;i<sorted.size();++i)
      this->points.push_back(sorted[i]);
  else
    for(unsigned i=0;i<sorted.size();++i)
      this->points.push_back(sorted[sorted.size()-1-i]);
  //std::cerr<<this->pointsToStr()<<std::endl;
  //std::cerr<<"3"<<std::endl;
}


std::string PlaneC::toStr(){
  std::stringstream ss;
  ss<<this->index<<" ";
  //ss<<this->points.size()<<" ";
  for(unsigned i=0;i<this->points.size();++i)ss<<this->points[i]->index<<" ";
  ss<<std::endl;
  return ss.str();
}
std::string PlaneC::pointsToStr(){
  std::stringstream ss;
  for(unsigned i=0;i<this->points.size();++i)
    ss<<this->points[i]->index<<" ";
  return ss.str();
}

