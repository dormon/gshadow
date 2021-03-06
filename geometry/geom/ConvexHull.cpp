#include"ConvexHull.h"
#include <algorithm>
using namespace geom;

#include<iostream>
#include<geCore/dtemplates.h>

void ConvexHull::_createUsingCorners(glm::vec3 p[8]){
  Point points[8];
  Plane planes[6];
  for(unsigned i=0;i<8;++i)points[i]=Point(p[i]);

  planes[0] = Plane(points[0],points[1],points[2]);//-z
  planes[1] = Plane(points[0],points[2],points[4]);//-x
  planes[2] = Plane(points[0],points[4],points[1]);//-y
  planes[3] = Plane(points[7],points[5],points[6]);//+z
  planes[4] = Plane(points[7],points[3],points[5]);//+x
  planes[5] = Plane(points[7],points[6],points[3]);//+y

  planes[0].addPoint(points[3]);
  planes[1].addPoint(points[6]);
  planes[2].addPoint(points[5]);
  planes[3].addPoint(points[4]);
  planes[4].addPoint(points[1]);
  planes[5].addPoint(points[2]);

  points[0].add(planes[0],planes[1],planes[2]);
  points[1].add(planes[0],planes[4],planes[2]);
  points[2].add(planes[0],planes[1],planes[5]);
  points[3].add(planes[0],planes[4],planes[5]);
  points[4].add(planes[3],planes[1],planes[2]);
  points[5].add(planes[3],planes[4],planes[2]);
  points[6].add(planes[3],planes[1],planes[5]);
  points[7].add(planes[3],planes[4],planes[5]);

  for(unsigned i=0;i<6;++i)
    this->_planes.insert(planes[i]);
  for(unsigned i=0;i<8;++i)
    this->_points.insert(points[i]);
}

ConvexHull::ConvexHull(glm::mat4 p,glm::mat4 v){
  glm::mat4 ip=glm::inverse(p);
  glm::mat4 iv=glm::inverse(v);
  glm::vec3 P[8];
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
  if(infinity){
    glm::vec3 CP=glm::vec3(iv*glm::vec4(0.f,0.f,0.f,1.f));
    Point points[5];
    for(int j=0;j<4;++j)points[j]=Point(P[j]);
    points[4]=Point(CP);
    Plane planes[5];
    planes[0]=Plane(points[4],points[0],points[2]);
    planes[1]=Plane(points[4],points[3],points[1]);
    planes[2]=Plane(points[4],points[1],points[0]);
    planes[3]=Plane(points[4],points[2],points[3]);
    planes[4]=Plane(points[0],points[2],points[1]);
    planes[4].add(points[3]);
    points[0].add(planes[0],planes[2],planes[4]);
    points[1].add(planes[1],planes[2],planes[4]);
    points[2].add(planes[0],planes[3],planes[4]);
    points[3].add(planes[1],planes[3],planes[4]);
    for(unsigned i=0;i<5;++i){
      this->_points.insert(points[i]);
      this->_planes.insert(planes[i]);
    }
  }else{
    this->_createUsingCorners(P);
  }
}

ConvexHull::ConvexHull(glm::vec3 minPoint,glm::vec3 maxPoint){
  glm::vec3 p[8];
  for(unsigned i=0;i<8;++i){
    glm::vec3 t=glm::vec3(float((i>>0)&1),float((i>>1)&1),float((i>>2)&1));
    glm::vec3 omt=glm::vec3(1.f)-t;
    p[i]=omt*minPoint+t*maxPoint;
  }
  this->_createUsingCorners(p);
}


ConvexHull::ConvexHull(ConvexHull const&a,ConvexHull const&b){
  if(a._planes.size()==0||b._planes.size()==0)return;
  const ConvexHull hulls[2]={a,b};

  for(unsigned i=0;i<2;++i)
    for(auto x:hulls[1-i]._points)
      if(hulls[i].inside(x))this->_points.insert(x);

  typedef std::set<Plane>::iterator iter;

  for(unsigned h=0;h<2;++h)
    for(iter i=hulls[h]._planes.begin();i!=hulls[h]._planes.end();++i){
      iter j=i;++j;
      for(;j!=hulls[h]._planes.end();++j)
        for(iter k=hulls[1-h]._planes.begin();k!=hulls[1-h]._planes.end();++k){
          Point newPoint=Point(*i,*j,*k);
          if(newPoint.invalid())continue;
          bool insert=true;
          for(unsigned f=0;f<2;++f){
            bool inFrontOf=true;
            for(auto x:hulls[f]._planes)
              if(newPoint<x){
                inFrontOf=false;
                break;
              }
            if(!inFrontOf){
              insert=false;
              break;
            }
          }
          if(!insert)continue;
          this->_points.insert(newPoint);
        }
    }

  for(unsigned i=0;i<2;++i)
    for(auto x:hulls[i]._planes){
      bool insert=false;
      for(auto y:this->_points)
        if(x==y){
          insert=true;
          break;
        }
      if(insert)this->_planes.insert(x);
    }

  for(auto x:this->_points)
    for(auto y:this->_planes)
      if(x==y){
        x.add(y);
        y.add(x);
      }

  if(this->getPlanar()!=this->_planes.end()){
    this->_planes.clear();
    this->_points.clear();
  }
}


ConvexHull::ConvexHull(ConvexHull const&hull,Point const&point){
  if(hull.inside(point)){
    this->_points = hull._points;
    this->_planes = hull._planes;
    return;
  }
  
  std::set<Plane>::iterator planar=hull.getPlanar();
  if(planar!=hull._planes.end()){
    this->_points.insert(point);
    this->_planes.insert(Plane(*planar,point));
    return;
  }
  
  std::vector<Plane>frontFacingPlanes;
  std::vector<Plane>backFacingPlanes;
  for(auto x:hull._planes){
    if(point>x)frontFacingPlanes.push_back(x);
    else        backFacingPlanes.push_back(x);
  }
  
  std::map<Point,std::vector<std::set<Plane>>>point2FBPlane;
  for(auto x:hull._points){
    point2FBPlane[x].push_back(std::set<Plane>());
    point2FBPlane[x].push_back(std::set<Plane>());

    for(auto y:frontFacingPlanes)
      if(y==x)
        point2FBPlane[x][0].insert(y);
    for(auto y:backFacingPlanes)
      if(y==x)
        point2FBPlane[x][1].insert(y);

    if(!point2FBPlane[x][0].size()||!point2FBPlane[x][1].size())
      point2FBPlane.erase(x);
  }
  
  typedef std::map<Point,std::vector<std::set<Plane>>>::iterator iter;
  iter curIndex  = point2FBPlane.begin();
  std::vector<iter>loop;
  loop.push_back(curIndex);
  for(;;){
    bool stop=true;
    iter next;
    for(next=point2FBPlane.begin();next!=point2FBPlane.end();++next){
      bool exists=false;
      for(auto x:loop)
        if(x==next){
          exists=true;
          break;
        }
      if(exists)continue;
      bool fbFound[2]={false,false};
      for(unsigned j=0;j<2;++j)
        for(auto x:curIndex->second[j])
          if(next->second[j].find(x)!=next->second[j].end()){
            fbFound[j]=true;
            break;
          }
      if(fbFound[0]&&fbFound[1]){
        curIndex=next;
        loop.push_back(next);
        stop=false;
        break;
      }
    }
    if(stop)break;
  }
  Point center=hull.center();

  if(center<=Plane(loop[0]->first,loop[1]->first,point))
    std::reverse(loop.begin(),loop.end());

  for(auto x:hull._points){
    bool insert=false;
    for(auto y:frontFacingPlanes)
      if(y==x){
        insert=true;
        break;
      }
    if(insert)this->_points.insert(x);
  }
  this->_points.insert(point);

  for(auto x:frontFacingPlanes)
    this->_planes.insert(x);

  for(unsigned i=0;i<loop.size();++i)
    this->_planes.insert(Plane(loop[i]->first,loop[(i+1)%loop.size()]->first,point));

  for(auto x:this->_points)
    for(auto y:this->_planes)
      if(x==y){
        x.add(y);
        y.add(x);
      }
}

bool ConvexHull::inside (Point const&a)const{
  if(this->_points.find(a)!=this->_points.end())return true;
  for(auto x:this->_planes)
    if(a<x)return false;
  return true;
}

Point ConvexHull::center()const{
  glm::vec3 cen=glm::vec3(0.f);
  for(auto x:this->_points)
    cen+=(glm::vec3)x;
  cen/=float(this->_points.size());
  return Point(cen);
}

std::set<Plane>::iterator ConvexHull::getPlanar()const{
  std::set<Plane>::iterator iter;
  for(iter=this->_planes.begin();iter!=this->_planes.end();++iter){
    bool every=true;
    for(auto y:this->_points)
      if(y!=*iter){
        every=false;
        break;
      }
    if(every)return iter;
  }
  return this->_planes.end();
}

std::string ConvexHull::toStr()const{
  std::stringstream ss;
  for(auto x:this->_points)
    ss<<x.toStr()<<std::endl;
  for(auto x:this->_planes)
    ss<<x.toStr()<<std::endl;
  return ss.str();
}
