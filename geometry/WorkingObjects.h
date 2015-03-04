#ifndef _WORKINGOBJECTS_H_
#define _WORKINGOBJECTS_H_

#include"Common.h"
#include"Plane.h"

namespace geometry{
  class PlaneC;

  class PointC{
    public:
      unsigned index;
      glm::vec3 point;
      std::vector<PlaneC*>planes;
      std::vector<Plane>creators;
      PointC(glm::vec3 point);
      PointC(glm::vec3 point,PlaneC*a,PlaneC*b,PlaneC*c);
      bool pushOnlyPlane(PlaneC*p);
      bool pushOnlyCreator(Plane p);
      void pushContainedPlane(PlaneC*p);
      void pushContainedPlanes(std::vector<PlaneC*>p);

      bool removePlane(PlaneC*p);//cant remove creators
      bool doesPlanesContain(Plane plane);
      bool doesCreatorsContain(Plane plane);
      void setIndex(unsigned index);
      bool onLineSegment(PlaneC**a,PlaneC**b,PointC*p);
      bool eqByCreators(PointC*p);//at least 3 creators are the same
      bool eqByPlanes(PlaneC*pa,PlaneC*pb,PlaneC*pc);
      bool eqByAny(PointC*);
      void addCreator(PointC*p);//adds creators only if this->point==p->point
      void addPlanes(PointC*p);//adds every plane
      std::string toStr();
      std::string dataToStr();
      std::string planesToStr();
  };

  class PlaneC{
    public:
      unsigned index;
      Plane plane;
      std::vector<PointC*>points;
      PlaneC(Plane plane);
      PlaneC(Plane plane,PointC*a,PointC*b,PointC*c);
      bool eq(PlaneC*p);
      void pushPoint(PointC*p);
      void pushContainedPoint(PointC*p);
      void pushContainedPoints(std::vector<PointC*>&p);
      void pushCreator(PointC*p);
      bool removePoint(PointC*p);
      void removePoints(std::vector<PointC*>&p);
      void setIndex(unsigned index);
      bool contain(PointC*p);
      bool contain3(std::vector<PointC*>&p);
      void sortPoints();
      std::string toStr();
      std::string pointsToStr();
  };
}

#endif//_WORKINGOBJECTS_H_
