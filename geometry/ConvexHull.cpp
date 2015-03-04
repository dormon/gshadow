#include"ConvexHull.h"

#include<limits>

#include<iostream>
#include<sstream>


using namespace geometry;

void geometry::camera2Plane(Plane*plane,unsigned*numPlanes,glm::mat4 p,glm::mat4 v){
  glm::mat4 ip=glm::inverse(p);
  glm::mat4 iv=glm::inverse(v);
  glm::vec3 P[8];
  glm::vec3 CP;
  bool infinity=false;
  for(unsigned i=0;i<8;++i){
    glm::vec4 pp=ip*glm::vec4(-1.f+2.f*((i>>0)&1),-1.f+2.f*((i>>1)&1),-1.f+2.f*((i>>2)&1),1.f);
    if(pp.w==0)
      infinity=true;
    else{
      glm::vec4 ppp=iv*glm::vec4(glm::vec3(pp)/pp.w,1.f);
      P[i]=glm::vec3(ppp);
    }
  }
  CP=glm::vec3(iv*glm::vec4(0.f,0.f,0.f,1.f));
  plane[4]=Plane(P[1],P[0],P[3]);
  if(!infinity)plane[5]=Plane(P[4],P[5],P[6]);
  plane[0]=Plane(CP,P[0],P[2]);
  plane[1]=Plane(CP,P[3],P[1]);
  plane[2]=Plane(CP,P[1],P[0]);
  plane[3]=Plane(CP,P[2],P[3]);
  if(infinity)*numPlanes=5;
  else*numPlanes=6;
}

void ConvexHull::deletePlanes(){
  for(unsigned i=0;i<this->planes.size();++i)
    delete this->planes[i];
  this->planes.clear();
}

void ConvexHull::deletePoints(){
  for(unsigned i=0;i<this->points.size();++i)
    delete this->points[i];
  this->points.clear();
}

void ConvexHull::setPlanes(std::vector<Plane>&planes){
  std::vector<PlaneC*>allPlanes;
  std::vector<PlaneC*>allCreators;

  for(unsigned i=0;i<planes.size();++i){
    bool uniq=true;
    for(unsigned j=i+1;j<planes.size();++j)
      if(planes[i].data==planes[j].data){
        uniq=false;
        break;
      }
    if(uniq)allPlanes.push_back(new PlaneC(planes[i]));
  }
  //std::cerr<<"allPlanes.size(): "<<allPlanes.size()<<std::endl;


  std::vector<PointC*>usefulPoints;
  std::vector<PointC*>empty;
  this->_getNewPoints(usefulPoints,allPlanes,0,2,allPlanes,1,1,true,allPlanes,1,0,true,empty,empty);

  std::vector<PlaneC*>usefulPlanes;
  std::vector<PlaneC*>uselessPlanes;
  for(unsigned i=0;i<allPlanes.size();++i){
    PlaneC*currentPlane=allPlanes[i];
    if(currentPlane->contain3(usefulPoints))usefulPlanes.push_back(currentPlane);
    else uselessPlanes.push_back(currentPlane);
  }

  for(unsigned i=0;i<usefulPoints.size();++i)
    for(unsigned j=0;j<uselessPlanes.size();++j)
      usefulPoints[i]->removePlane(uselessPlanes[j]);

  for(unsigned i=0;i<uselessPlanes.size();++i)
    delete uselessPlanes[i];

  for(unsigned i=0;i<usefulPoints.size();++i){
    usefulPoints[i]->setIndex(i);
    this->points.push_back(usefulPoints[i]);
  }
  for(unsigned i=0;i<usefulPlanes.size();++i){
    usefulPlanes[i]->setIndex(i);
    this->planes.push_back(usefulPlanes[i]);
  }

  this->checks();
}

void ConvexHull::checks(){
  //std::cerr<<this->allToStr();
  bool error=false;

  //1. EVERY FACE HAS TO HAVE EVERY VERTEX UNIQUE, AND MORE THAN 2 VERTICES - no face
  for(unsigned i=0;i<this->planes.size();++i){
    for(unsigned j=0;j<this->planes[i]->points.size()-1;++j){
      for(unsigned k=j+1;k<this->planes[i]->points.size();++k){
        if((void*)this->planes[i]->points[j]==(void*)this->planes[i]->points[k]){
          std::cerr<<"no unique face"<<i<<": "<<j<<"-"<<k<<std::endl;
          error=true;
        }
      }
    }
  }
  //if(error)exit(0);

  //2. EVERY VERTEX HAS TO HAVE EVERY PLANE UNIQUE, AND MORE THAN 2 PLANES - no vertex
  for(unsigned i=0;i<this->points.size();++i){
    for(unsigned j=0;j<this->points[i]->planes.size()-1;++j){
      for(unsigned k=j+1;k<this->points[i]->planes.size();++k){
        if(this->points[i]->planes[j]==this->points[i]->planes[k]){
          std::cerr<<"no unique vertex"<<i<<": "<<j<<"-"<<k<<std::endl;
          error=true;
        }
      }
    }
  }
  //if(error)exit(0);

  //3. EVERY FACE HAS TO BE UNIQUE
  for(unsigned i=0;i<this->planes.size()-1;++i){
    for(unsigned j=i+1;j<this->planes.size();++j){
      unsigned nofSameVertices=0;
      for(unsigned k=0;k<this->planes[i]->points.size();++k){
        for(unsigned l=0;l<this->planes[j]->points.size();++l){
          if((void*)this->planes[i]->points[k]==(void*)this->planes[j]->points[l]){
            nofSameVertices++;
          }
        }
      }
      if(nofSameVertices==this->planes[i]->points.size()){
        std::cerr<<"face "<<i<<"<"<<j<<std::endl;
        error=true;
      }
      if(nofSameVertices==this->planes[j]->points.size()){
        std::cerr<<"face "<<j<<"<"<<i<<std::endl;
        error=true;
      }
      if(nofSameVertices>2){
        std::cerr<<"faces "<<i<<" "<<j<<" share more than 2 vertices"<<std::endl;
        std::cerr<<"  "<<this->planes[i]->plane.toStr()<<" : "<<this->planes[j]->plane.toStr()<<std::endl;
        for(unsigned k=0;k<this->planes[i]->points.size();++k)
          std::cerr<<"  "<<this->planes[i]->points[k]->index<<" "<<this->planes[i]->points[k]->dataToStr()
            <<" - "
            <<this->planes[i]->plane.distance(this->planes[i]->points[k]->point)<<" : "
            <<this->planes[j]->plane.distance(this->planes[i]->points[k]->point)<<std::endl;

        std::cerr<<std::endl;
        for(unsigned k=0;k<this->planes[j]->points.size();++k)
          std::cerr<<"  "<<this->planes[j]->points[k]->index<<" "<<this->planes[j]->points[k]->dataToStr()
            <<" - "
            <<this->planes[j]->plane.distance(this->planes[j]->points[k]->point)<<" : "
            <<this->planes[i]->plane.distance(this->planes[j]->points[k]->point)<<std::endl;


        error=true;
      }
    }
  }
  //if(error)exit(0);

  //4. EVERY VERTEX HAS TO HAVE UNIQUE PLANES
  for(unsigned i=0;i<this->points.size()-1;++i){
    for(unsigned j=i+1;j<this->points.size();++j){
      unsigned nofSamePlanes=0;
      for(unsigned k=0;k<this->points[i]->planes.size();++k){
        for(unsigned l=0;l<this->points[j]->planes.size();++l){
          if(this->points[i]->planes[k]==this->points[j]->planes[l]){
            nofSamePlanes++;
          }
        }
      }
      if(nofSamePlanes==this->points[i]->planes.size()){
        std::cerr<<"point "<<i<<"<"<<j<<std::endl;
        error=true;
      }
      if(nofSamePlanes==this->points[j]->planes.size()){
        std::cerr<<"point "<<j<<"<"<<i<<std::endl;
        error=true;
      }
      if(nofSamePlanes>2){
        std::cerr<<"points "<<i<<" "<<j<<" share more than 2 planes"<<std::endl;
        error=true;
      }
    }
  }
  if(error)exit(0);

}

ConvexHull::ConvexHull(){

}

ConvexHull::ConvexHull(std::vector<Plane>&planes){
  this->setPlanes(planes);
}


ConvexHull::ConvexHull(glm::mat4 p,glm::mat4 v){
  geometry::Plane frustumPlanes[6];
  unsigned numFrustumPlanes=0;
  geometry::camera2Plane(
      frustumPlanes,
      &numFrustumPlanes,
      p,
      v);
  std::vector<Plane>planes;

  for(unsigned i=0;i<numFrustumPlanes;++i)
    planes.push_back(frustumPlanes[i]);

  this->setPlanes(planes);
}

ConvexHull::ConvexHull(glm::vec3 minPoint,glm::vec3 maxPoint){
  glm::vec3 scenePoints[8];
  for(unsigned i=0;i<8;++i){
    glm::vec3 t=glm::vec3(float((i>>0)&1),float((i>>1)&1),float((i>>2)&1));
    glm::vec3 omt=glm::vec3(1.f)-t;
    scenePoints[i]=omt*minPoint+t*maxPoint;
  }
  std::vector<Plane>planes;

  planes.push_back(geometry::Plane(scenePoints[0],scenePoints[1],scenePoints[2]));
  planes.push_back(geometry::Plane(scenePoints[0],scenePoints[2],scenePoints[4]));
  planes.push_back(geometry::Plane(scenePoints[0],scenePoints[4],scenePoints[1]));
  planes.push_back(geometry::Plane(scenePoints[7],scenePoints[5],scenePoints[6]));
  planes.push_back(geometry::Plane(scenePoints[7],scenePoints[3],scenePoints[5]));
  planes.push_back(geometry::Plane(scenePoints[7],scenePoints[6],scenePoints[3]));

  this->setPlanes(planes);
}
ConvexHull::~ConvexHull(){
  for(unsigned i=0;i<this->points.size();++i)
    delete this->points[i];
  for(unsigned i=0;i<this->planes.size();++i)
    delete this->planes[i];
}
void ConvexHull::recomputePoints(){
}
void ConvexHull::_getNewPoints(
    std::vector<PointC*>&newPoints,
    std::vector<PlaneC*>&aPlanes,
    unsigned aS,unsigned aE,
    std::vector<PlaneC*>&bPlanes,
    unsigned bS,unsigned bE,bool ai,
    std::vector<PlaneC*>&cPlanes,
    unsigned cS,unsigned cE,bool bi,
    std::vector<PointC*>useful,
    std::vector<PointC*>useless){
  for(unsigned i=aS;i<aPlanes.size()-aE;++i){
    for(unsigned j=i*ai+bS;j<bPlanes.size()-bE;++j){
      for(unsigned k=j*bi+cS;k<cPlanes.size()-cE;++k){
        PlaneC*pa=aPlanes[i];
        PlaneC*pb=bPlanes[j];
        PlaneC*pc=cPlanes[k];

        glm::vec3 p=geometry::planes2Point(pa->plane,pb->plane,pc->plane);
        if(glm::any(glm::isnan(p)))continue;

        PointC*newPoint=new PointC(p,pa,pb,pc);
        for(unsigned l=0;l<useful.size();++l){
          if(useful[l]->eqByAny(newPoint)){
            useful[l]->addCreator(newPoint);
            delete newPoint;
            newPoint=NULL;
            break;
          }
        }
        if(!newPoint)continue;
        for(unsigned l=0;l<useless.size();++l){
          if(useless[l]->eqByAny(newPoint)){
            useless[l]->addCreator(newPoint);
            delete newPoint;
            newPoint=NULL;
            break;
          }
        }
        if(!newPoint)continue;
        if(!this->_isUseful(newPoint,aPlanes)){
          delete newPoint;
          newPoint=NULL;
        }
        if(!newPoint)continue;
        if(bPlanes!=aPlanes){
          if(!this->_isUseful(newPoint,bPlanes)){
            delete newPoint;
            newPoint=NULL;
          }
        }
        if(!newPoint)continue;
        if(cPlanes!=aPlanes||cPlanes!=bPlanes){
          if(!this->_isUseful(newPoint,cPlanes)){
            delete newPoint;
            newPoint=NULL;
          }
        }
        if(!newPoint)continue;
        bool same=false;

        for(unsigned l=0;l<newPoints.size();++l)
          if(newPoints[l]->point==newPoint->point){
            if(newPoints[l]->pushOnlyPlane(pa))pa->pushPoint(newPoints[l]);
            if(newPoints[l]->pushOnlyPlane(pb))pb->pushPoint(newPoints[l]);
            if(newPoints[l]->pushOnlyPlane(pc))pc->pushPoint(newPoints[l]);
            newPoints[l]->pushOnlyCreator(pa->plane);
            newPoints[l]->pushOnlyCreator(pb->plane);
            newPoints[l]->pushOnlyCreator(pc->plane);
            same=true;
            break;
          }
        if(!same){
          newPoint->pushOnlyPlane(pa);
          newPoint->pushOnlyPlane(pb);
          newPoint->pushOnlyPlane(pc);
          pa->pushPoint(newPoint);
          pb->pushPoint(newPoint);
          pc->pushPoint(newPoint);
          newPoints.push_back(newPoint);
        }else delete newPoint;
      }
    }
  }

}

void ConvexHull::_getUsefulPoints(
    std::vector<PointC*>&useful,
    std::vector<PointC*>&useless,
    std::vector<PointC*>newPoints,
    std::vector<PlaneC*>newPlanes){
  for(unsigned i=0;i<newPoints.size();++i){
    PointC*curPoint=newPoints[i];
    bool inFrontOf=true;
    for(unsigned j=0;j<newPlanes.size();++j){
      PlaneC*curPlane=newPlanes[j];
      bool skip=false;
      if(curPoint->doesCreatorsContain(curPlane->plane)){
        skip=true;
      }
      if(curPoint->doesPlanesContain(curPlane->plane)){
        skip=true;
      }
      if(skip)continue;
      if(curPlane->plane.distance(curPoint->point)<0){
        //std::cerr<<"point: "<<curPoint->dataToStr()<<std::endl;
        //std::cerr<<"plane: "<<curPlane->plane.toStr()<<std::endl;
        //std::cerr<<"dist: "<<curPlane->plane.distance(curPoint->point)<<std::endl;
        inFrontOf=false;
        break;
      }
    }
    if(inFrontOf)useful.push_back(newPoints[i]);
    else useless.push_back(newPoints[i]);
  }
}

bool ConvexHull::_isUseful(
    PointC*newPoint,
    std::vector<PlaneC*>planes){
  for(unsigned j=0;j<planes.size();++j){
    PlaneC*curPlane=planes[j];
    bool skip=false;
    if(newPoint->doesCreatorsContain(curPlane->plane))skip=true;
    if(newPoint->doesPlanesContain  (curPlane->plane))skip=true;
    if(skip)continue;
    if(curPlane->plane.distance(newPoint->point)<0)return false;
  }
  return true;
}


ConvexHull*ConvexHull::intersect(ConvexHull*b){
  //std::cerr<<"TADY-1"<<std::endl;
  ConvexHull*a=this;
  ConvexHull*hull[2]={a,b};

  std::vector<PointC*>allPoints;
  std::vector<PlaneC*>allPlanes;
  unsigned pointOffset[2]={0,(unsigned)a->points.size()};
  unsigned planeOffset[2]={0,(unsigned)a->planes.size()};

  for(unsigned h=0;h<2;++h){
    for(unsigned i=0;i<hull[h]->points.size();++i){
      PointC*np=new PointC(hull[h]->points[i]->point);
      np->setIndex(pointOffset[h]+i);
      np->addCreator(hull[h]->points[i]);
      allPoints.push_back(np);
    }

    for(unsigned i=0;i<hull[h]->planes.size();++i){
      allPlanes.push_back(new PlaneC(hull[h]->planes[i]->plane));
      allPlanes[planeOffset[h]+i]->index=planeOffset[h]+i;
    }
  }

  for(unsigned h=0;h<2;++h){
    for(unsigned i=0;i<hull[h]->points.size();++i){
      for(unsigned j=0;j<hull[h]->points[i]->planes.size();++j)
        allPoints[pointOffset[h]+i]->pushOnlyPlane(allPlanes[planeOffset[h]+hull[h]->points[i]->planes[j]->index]);
    }

    for(unsigned i=0;i<hull[h]->planes.size();++i){
      for(unsigned j=0;j<hull[h]->planes[i]->points.size();++j)
        allPlanes[planeOffset[h]+i]->pushPoint(allPoints[pointOffset[h]+hull[h]->planes[i]->points[j]->index]);
    }
  }
  /*
     for(unsigned i=0;i<allPlanes.size();++i){
     std::cerr<<allPlanes[i]->toStr()<<std::endl;
     }
     */
  //COPY IS READY

  std::vector<unsigned>equalPoint;
  for(unsigned i=0;i<allPoints.size();++i)equalPoint.push_back(i);
  for(unsigned i=0;i<hull[0]->points.size();++i){
    for(unsigned j=0;j<hull[1]->points.size();++j){
      if(allPoints[pointOffset[0]+i]->eqByAny(allPoints[pointOffset[1]+j])){
        equalPoint[pointOffset[0]+i]=pointOffset[1]+j;
        equalPoint[pointOffset[1]+j]=pointOffset[0]+i;
        break;
      }
    }
  }

  std::vector<unsigned>equalPlane;
  for(unsigned i=0;i<allPlanes.size();++i)equalPlane.push_back(i);
  for(unsigned i=0;i<hull[0]->planes.size();++i){
    for(unsigned j=0;j<hull[1]->planes.size();++j){
      if(allPlanes[planeOffset[0]+i]->eq(allPlanes[planeOffset[1]+j])){
        equalPlane[planeOffset[0]+i]=planeOffset[1]+j;
        equalPlane[planeOffset[1]+j]=planeOffset[0]+i;
        break;
      }
    }
  }
  //EQUIVALENCE READY

  std::vector<PointC*>uniqPoints;
  for(unsigned i=0;i<allPoints.size();++i){
    if(equalPoint[i]==i){//this point i uniq
      PointC*curPoint=allPoints[i];
      for(unsigned j=0;j<curPoint->planes.size();++j){
        unsigned planeID=curPoint->planes[j]->index;
        if(equalPlane[planeID]<planeID)curPoint->planes[j]=allPlanes[equalPlane[planeID]];
      }
      uniqPoints.push_back(curPoint);
    }else if(equalPoint[i]>i){//there exist redundant point with greater index
      PointC*curPoint = allPoints[i];
      PointC*eqPoint  = allPoints[equalPoint[i]];
      for(unsigned j=0;j<eqPoint->planes.size();++j){
        unsigned planeID=eqPoint->planes[j]->index;
        if(equalPlane[planeID]<planeID)curPoint->pushOnlyPlane(allPlanes[equalPlane[planeID]]);
        else curPoint->pushOnlyPlane(allPlanes[planeID]);
      }
      curPoint->addCreator(eqPoint);
      uniqPoints.push_back(curPoint);
    }else{//this point i redundant
      for(unsigned j=0;j<allPlanes.size();++j)
        if(allPlanes[j]->removePoint(allPoints[i]))
          allPlanes[j]->pushPoint(allPoints[equalPoint[i]]);
      delete allPoints[i];
      allPoints[i]=NULL;
    }
  }
  for(unsigned i=0;i<allPlanes.size();++i){
    if(equalPlane[i]<i){
      delete allPlanes[i];
      allPlanes[i]=NULL;
    }
  }

  for(unsigned i=0;i<allPlanes.size();++i){
    if(!allPlanes[i])continue;
    allPlanes[i]->pushContainedPoints(uniqPoints);
  }
  for(unsigned i=0;i<uniqPoints.size();++i){
    if(!uniqPoints[i])continue;
    uniqPoints[i]->pushContainedPlanes(allPlanes);
  }

  /*
  std::cerr<<"{{{{{{{{{{{{{{{{1\n";
  for(unsigned i=0;i<allPoints.size();++i)
    if(allPoints[i])std::cerr<<allPoints[i]->toStr();
  std::cerr<<"}}}}}}}}}}}}}}}}1\n";

  std::cerr<<"[[[[[[[[[[[[[[[[1\n";
  for(unsigned i=0;i<allPlanes.size();++i)
    if(allPlanes[i])std::cerr<<allPlanes[i]->toStr();
  std::cerr<<"]]]]]]]]]]]]]]]]1\n";
  // */

  /*
     for(unsigned i=0;i<uniqPoints.size();++i)
     std::cerr<<uniqPoints[i]->dataToStr()<<std::endl;
     std::cerr<<"TADY1"<<std::endl;
     */

  std::vector<PlaneC*>uniqPlanes;
  std::vector<PlaneC*>aPlanes;
  std::vector<PlaneC*>bPlanes;
  std::vector<unsigned>planeOrigin;
  for(unsigned i=0;i<allPlanes.size();++i){
    if(!allPlanes[i])continue;
    bool uniq=false;
    for(unsigned j=0;j<uniqPoints.size();++j)
      if(uniqPoints[j]->doesPlanesContain(allPlanes[i]->plane)){
        uniq=true;
        break;
      }
    if(uniq){
      uniqPlanes.push_back(allPlanes[i]);
      if(i<a->planes.size())aPlanes.push_back(allPlanes[i]);
      else bPlanes.push_back(allPlanes[i]);
    }
    else delete allPlanes[i];//TODO REMOVE FROM POINTS?
  }
  //MERGED SAME DATA
  /*
     for(unsigned i=0;i<uniqPlanes.size();++i)
     std::cerr<<uniqPlanes[i]->plane.toStr()<<std::endl;
     std::cerr<<"TADY2"<<std::endl;
     */
  /*
  std::cerr<<"(((((((((((0\n";
  for(unsigned i=0;i<uniqPlanes.size();++i){
    std::cerr<<uniqPlanes[i]->toStr();
  }
  std::cerr<<")))))))))))0\n";
  // */

  std::vector<PointC*>usefulPoints;
  std::vector<PointC*>uselessPoints;
  this->_getUsefulPoints(
      usefulPoints,
      uselessPoints,
      uniqPoints,
      uniqPlanes);//TODO TODO
  /*
  std::cerr<<"(((((((((((.5\n";
  for(unsigned i=0;i<uniqPlanes.size();++i){
    std::cerr<<uniqPlanes[i]->toStr();
  }
  std::cerr<<"))))))))))).5\n";
  // */

  /*
     std::cerr<<"usefulPoints.size(): "<<usefulPoints.size()<<std::endl;
     std::cerr<<"uselessPoints.size(): "<<uselessPoints.size()<<std::endl;
     for(unsigned i=0;i<usefulPoints.size();++i)
     std::cerr<<usefulPoints[i]->dataToStr()<<std::endl;

     std::cerr<<"TADY3"<<std::endl;
     */
  /*
  std::cerr<<"(((((((((((.5\n";
  for(unsigned i=0;i<bPlanes.size();++i){
    std::cerr<<bPlanes[i]->toStr()<<" "<<bPlanes[i]->plane.toStr()<<std::endl;
  }
  std::cerr<<"))))))))))).5\n";
  // */

  std::vector<PointC*>newPoints;
  this->_getNewPoints(newPoints,aPlanes,0,1,aPlanes,1,0,true,bPlanes,0,0,false,usefulPoints,uselessPoints);
  this->_getNewPoints(newPoints,bPlanes,0,1,bPlanes,1,0,true,aPlanes,0,0,false,usefulPoints,uselessPoints);
  //std::cerr<<"newPoints.size(): "<<newPoints.size()<<std::endl;
  /*
  std::cerr<<"(((((((((((.75\n";
  for(unsigned i=0;i<newPoints.size();++i){
    std::cerr<<newPoints[i]->toStr();
  }
  std::cerr<<"))))))))))).75\n";
  // */
  /*
  std::cerr<<"(((((((((((.85\n";
  for(unsigned i=0;i<bPlanes.size();++i){
    std::cerr<<bPlanes[i]->toStr();
  }
  std::cerr<<"))))))))))).85\n";
  // */

  //std::cerr<<"TADY4"<<std::endl;
  /*
  std::cerr<<"(((((((((((.75\n";
  for(unsigned i=0;i<uniqPlanes.size();++i){
    std::cerr<<uniqPlanes[i]->toStr();
  }
  std::cerr<<"))))))))))).75\n";
  // */


  for(unsigned i=0;i<uniqPlanes.size();++i)uniqPlanes[i]->removePoints(uselessPoints);
  for(unsigned i=0;i<uselessPoints.size();++i)delete uselessPoints[i];
  /*
  std::cerr<<"(((((((((((.86\n";
  for(unsigned i=0;i<bPlanes.size();++i){
    std::cerr<<bPlanes[i]->toStr();
  }
  std::cerr<<"))))))))))).86\n";
  // */

  /*
  std::cerr<<"(((((((((((2\n";
  for(unsigned i=0;i<uniqPlanes.size();++i){
    std::cerr<<uniqPlanes[i]->toStr();
  }
  std::cerr<<")))))))))))2\n";
  // */


  ConvexHull*newHull=new ConvexHull();

  for(unsigned i=0;i<usefulPoints.size();++i){
    newHull->points.push_back(usefulPoints[i]);
    usefulPoints[i]->setIndex(i);
  }

  for(unsigned i=0;i<newPoints.size();++i){
    newHull->points.push_back(newPoints[i]);
    newPoints[i]->setIndex(usefulPoints.size()+i);
  }

  //std::cerr<<"TADY8"<<std::endl;
  /*
  std::cerr<<"[[[[[[[[[[[[[[[[1\n";
  for(unsigned i=0;i<newHull->points.size();++i)
    std::cerr<<newHull->points[i]->toStr();
  std::cerr<<"]]]]]]]]]]]]]]]]1\n";

  std::cerr<<"(((((((((((3\n";
  for(unsigned i=0;i<uniqPlanes.size();++i){
    std::cerr<<uniqPlanes[i]->toStr();
  }
  std::cerr<<")))))))))))3\n";
  // */

  for(unsigned i=0,counter=0;i<uniqPlanes.size();++i){
    if(uniqPlanes[i]->contain3(newHull->points)){
      newHull->planes.push_back(uniqPlanes[i]);
      uniqPlanes[i]->setIndex(counter++);
    }else{
      for(unsigned j=0;j<newHull->points.size();++j)
        newHull->points[j]->removePlane(uniqPlanes[i]);
      delete uniqPlanes[i];
    }
  }
  //std::cerr<<"TADY9"<<std::endl;
  //std::cerr<<newHull->allToStr();
  return newHull;
}


bool ConvexHull::inside(glm::vec3 point){
  for(unsigned i=0;i<this->points.size();++i)
    if(this->points[i]->point==point)return true;
  for(unsigned i=0;i<this->planes.size();++i)
    if(this->planes[i]->plane.distance(point)<0)return false;
  return true;
}
bool ConvexHull::containPoint(PlaneC*pa,PlaneC*pb,PlaneC*pc){
  for(unsigned i=0;i<this->points.size();++i)
    if(this->points[i]->eqByPlanes(pa,pb,pc))return true;
  return false;
}

std::string ConvexHull::toStr(){
  std::stringstream ss;
  for(unsigned p=0;p<this->planes.size();++p){
    ss<<"pl"<<p<<": "<<std::endl;
    for(int k=0;k<4;++k)
      ss<<this->planes[p]->plane.data[k]<<" ";
    ss<<std::endl;
    ss<<this->planes[p]->toStr();
    //ss<<"  points: "<<this->planes[p]->points.size()<<std::endl;
    //ss<<"  index:  "<<this->planes[p]->index<<std::endl;
  }
  for(unsigned p=0;p<this->points.size();++p){
    ss<<"p"<<p<<": "<<std::endl;
    for(int k=0;k<3;++k)
      ss<<this->points[p]->point[k]<<" ";
    ss<<std::endl;
    ss<<this->points[p]->toStr();
    //ss<<"  points: "<<this->planes[p]->points.size()<<std::endl;
    //ss<<"  index:  "<<this->planes[p]->index<<std::endl;
  }

  ss<<std::endl;

  return ss.str();
}

glm::vec3 ConvexHull::getCenter(){
  glm::vec3 center=glm::vec3(0.f);
  for(unsigned i=0;i<this->points.size();++i)
    center+=this->points[i]->point;
  center/=(float)this->points.size();
  return center;
}
std::string ConvexHull::allToStr(){
  std::stringstream ss;
  for(unsigned i=0;i<this->planes.size();++i)
    ss<<"apl"<<i<<": "<<this->planes[i]->pointsToStr()<<" "<<this->planes[i]->plane.toStr()<<std::endl;
  for(unsigned i=0;i<this->points.size();++i)
    ss<<"ap"<<i<<": "<<this->points[i]->planesToStr()<<" "<<this->points[i]->dataToStr()<<std::endl;
  ss<<std::endl;
  return ss.str();
}

unsigned ConvexHull::extend(glm::vec3 p){
  if(this->inside(p))return this->points.size();

  glm::vec3 center=this->getCenter();

  std::vector<bool>frontFacing;
  std::vector<bool>backFacing;
  std::vector<PlaneC*>frontFacingPlanes;
  std::vector<PlaneC*>backFacingPlanes;
  std::vector<PointC*>frontFacingPoints;
  std::vector<PointC*>backFacingPoints;
  for(unsigned i=0;i<this->points.size();++i){
    frontFacing.push_back(false);
    backFacing.push_back(false);
  }

  for(unsigned i=0;i<this->planes.size();++i){
    PlaneC*currentPlane=this->planes[i];
    if(currentPlane->plane.distance(p)<0){
      backFacingPlanes.push_back(currentPlane);
      for(unsigned j=0;j<currentPlane->points.size();++j)
        backFacing[currentPlane->points[j]->index]=true;
    }else{
      frontFacingPlanes.push_back(currentPlane);
      for(unsigned j=0;j<currentPlane->points.size();++j)
        frontFacing[currentPlane->points[j]->index]=true;
    }
  }
  std::vector<PointC*>silhouetteVertices;
  for(unsigned i=0;i<this->points.size();++i)
    if(frontFacing[i]&&backFacing[i])
      silhouetteVertices.push_back(this->points[i]);
    else{
      if(frontFacing[i])frontFacingPoints.push_back(this->points[i]);
      if(backFacing[i])backFacingPoints.push_back(this->points[i]);
    }

  //LIST OF FRONT/BACK FACING VECTICES/PLANES ARE CONSTRUCTED
  //AND LIST OF SILHOUETTE VERTICES AS WELL

  /*
     for(unsigned i=0;i<frontFacingPlanes.size();++i)
     std::cerr<<"f"<<i<<": "<<frontFacingPlanes[i]->index<<" "<<frontFacingPlanes[i]->plane.toStr()<<std::endl;
     for(unsigned i=0;i<backFacingPlanes.size();++i)
     std::cerr<<"b"<<i<<": "<<backFacingPlanes[i]->index<<" "<<backFacingPlanes[i]->plane.toStr()<<std::endl;

     for(unsigned i=0;i<frontFacingPoints.size();++i)
     std::cerr<<"fp"<<i<<": "<<frontFacingPoints[i]->index<<" "<<frontFacingPoints[i]->dataToStr()<<std::endl;
     for(unsigned i=0;i<backFacingPoints.size();++i)
     std::cerr<<"bp"<<i<<": "<<backFacingPoints[i]->index<<" "<<backFacingPoints[i]->dataToStr()<<std::endl;
     std::cerr<<std::endl;
     for(unsigned i=0;i<silhouetteVertices.size();++i)
     std::cerr<<"sil"<<i<<": "<<silhouetteVertices[i]->index<<" "<<silhouetteVertices[i]->dataToStr()<<std::endl;
     std::cerr<<std::endl;

  // */
  std::vector<bool>used;
  for(unsigned i=0;i<silhouetteVertices.size();++i)used.push_back(false);

  std::vector<PointC*>connected;

  for(unsigned i=0;i<silhouetteVertices.size();++i)
    connected.push_back(NULL);

  PointC*last=silhouetteVertices[0];
  PointC*lastlast=silhouetteVertices[0];
  unsigned counter=0;
  while(counter<silhouetteVertices.size()){
    for(unsigned i=0;i<silhouetteVertices.size();++i){
      PointC*c=silhouetteVertices[i];
      if((void*)c==(void*)last)continue;
      if((void*)c==(void*)lastlast)continue;
      PlaneC*pa;
      PlaneC*pb;
      if(last->onLineSegment(&pa,&pb,c)){
        bool aback=false;
        bool bback=false;
        for(unsigned k=0;k<backFacingPlanes.size();++k){
          if(backFacingPlanes[k]==pa)aback=true;
          if(backFacingPlanes[k]==pb)bback=true;
          if(aback&&bback)break;
        }
        if(((aback)&&(bback))||((!aback)&&(!bback)))continue;
        lastlast=last;
        last=silhouetteVertices[i];
        //std::cerr<<"e: "<<last->index<<std::endl;
        connected[counter]=last;
        counter++;
        break;
      }
    }
  }

  //CONNECTED LIST IS CONSTRUCED

  for(unsigned i=0;i<backFacingPlanes.size();++i)
    for(unsigned j=0;j<connected.size();++j)
      connected[j]->removePlane(backFacingPlanes[i]);

  for(unsigned i=0;i<backFacingPlanes.size();++i)
    delete backFacingPlanes[i];
  backFacingPlanes.clear();

  for(unsigned i=0;i<backFacingPoints.size();++i)
    delete backFacingPoints[i];
  backFacingPoints.clear();



  PointC*newPoint=new PointC(p);

  std::vector<PlaneC*>newPlanes;
  for(unsigned i=0;i<connected.size();++i){
    Plane np=Plane(newPoint->point,connected[i]->point,connected[(i+1)%connected.size()]->point);
    if(np.distance(center)<0)np=np.neg();
    PlaneC*npc=new PlaneC(np,newPoint,connected[i],connected[(i+1)%connected.size()]);

    newPoint->pushOnlyPlane(npc);
    connected[i]->pushOnlyPlane(npc);
    connected[(i+1)%connected.size()]->pushOnlyPlane(npc);
    newPlanes.push_back(npc);
  }

  this->points.clear();
  this->points.push_back(newPoint);
  newPoint->setIndex(0);

  for(unsigned i=0;i<connected.size();++i){
    connected[i]->setIndex(1+i);
    this->points.push_back(connected[i]);
  }

  for(unsigned i=0;i<frontFacingPoints.size();++i){
    frontFacingPoints[i]->setIndex(1+i+connected.size());
    this->points.push_back(frontFacingPoints[i]);
  }

  this->planes.clear();
  for(unsigned i=0;i<newPlanes.size();++i){
    newPlanes[i]->setIndex(i);
    this->planes.push_back(newPlanes[i]);
  }
  for(unsigned i=0;i<frontFacingPlanes.size();++i){
    frontFacingPlanes[i]->setIndex(newPlanes.size()+i);
    this->planes.push_back(frontFacingPlanes[i]);
  }

  this->checks();
  return 0;
}

void ConvexHull::sortFaces(){
  for(unsigned i=0;i<this->planes.size();++i)
    this->planes[i]->sortPoints();
}

void ConvexHull::_getSilhouetteVertices(std::vector<PointC*>&sil,glm::vec3 p){
  if(this->inside(p))return;

  std::vector<bool>frontFacing;
  std::vector<bool>backFacing;
  std::vector<PlaneC*>frontFacingPlanes;
  std::vector<PlaneC*>backFacingPlanes;
  std::vector<PointC*>frontFacingPoints;
  std::vector<PointC*>backFacingPoints;
  for(unsigned i=0;i<this->points.size();++i){
    frontFacing.push_back(false);
    backFacing.push_back(false);
  }

  for(unsigned i=0;i<this->planes.size();++i){
    PlaneC*currentPlane=this->planes[i];
    if(currentPlane->plane.distance(p)<0){
      backFacingPlanes.push_back(currentPlane);
      for(unsigned j=0;j<currentPlane->points.size();++j)
        backFacing[currentPlane->points[j]->index]=true;
    }else{
      frontFacingPlanes.push_back(currentPlane);
      for(unsigned j=0;j<currentPlane->points.size();++j)
        frontFacing[currentPlane->points[j]->index]=true;
    }
  }
  std::vector<PointC*>silhouetteVertices;
  for(unsigned i=0;i<this->points.size();++i)
    if(frontFacing[i]&&backFacing[i])
      silhouetteVertices.push_back(this->points[i]);
    else{
      if(frontFacing[i])frontFacingPoints.push_back(this->points[i]);
      if(backFacing[i])backFacingPoints.push_back(this->points[i]);
    }

  std::vector<bool>used;
  for(unsigned i=0;i<silhouetteVertices.size();++i)used.push_back(false);

  for(unsigned i=0;i<silhouetteVertices.size();++i)
    sil.push_back(NULL);

  PointC*last=silhouetteVertices[0];
  PointC*lastlast=silhouetteVertices[0];
  unsigned counter=0;
  while(counter<silhouetteVertices.size()){
    for(unsigned i=0;i<silhouetteVertices.size();++i){
      PointC*c=silhouetteVertices[i];
      if((void*)c==(void*)last)continue;
      if((void*)c==(void*)lastlast)continue;
      PlaneC*pa;
      PlaneC*pb;
      if(last->onLineSegment(&pa,&pb,c)){
        bool aback=false;
        bool bback=false;
        for(unsigned k=0;k<backFacingPlanes.size();++k){
          if(backFacingPlanes[k]==pa)aback=true;
          if(backFacingPlanes[k]==pb)bback=true;
          if(aback&&bback)break;
        }
        if(((aback)&&(bback))||((!aback)&&(!bback)))continue;
        lastlast=last;
        last=silhouetteVertices[i];
        sil[counter]=last;
        counter++;
        break;
      }
    }
  }
}
