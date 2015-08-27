#include"GeometryConvexHull.h"

using namespace geom;

ConvexHull::ConvexHull(glm::mat4 p,glm::mat4 v){
  //TODO
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
    //glm::vec3 CP=glm::vec3(iv*glm::vec4(0.f,0.f,0.f,1.f));
  }else{
    /*
    plane[0]=Plane(CP,P[0],P[2]);
    plane[1]=Plane(CP,P[3],P[1]);
    plane[2]=Plane(CP,P[1],P[0]);
    plane[3]=Plane(CP,P[2],P[3]);
    plane[4]=Plane(P[1],P[0],P[3]);
    plane[5]=Plane(P[4],P[5],P[6]);
    */
  }
}

ConvexHull::ConvexHull(glm::vec3 minPoint,glm::vec3 maxPoint){
  Point points[8];
  Plane planes[6];
  for(unsigned i=0;i<8;++i){
    glm::vec3 t=glm::vec3(float((i>>0)&1),float((i>>1)&1),float((i>>2)&1));
    glm::vec3 omt=glm::vec3(1.f)-t;
    points[i]=Point(omt*minPoint+t*maxPoint);
  }

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

ConvexHull::ConvexHull(ConvexHull const&a,ConvexHull const&b){
  //TODO
}

void ConvexHull::extend(Point const&point){
  //TODO
}

