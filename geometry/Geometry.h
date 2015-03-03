#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include"Common.h"
#include"Plane.h"
#include"ConvexHull.h"
#include"ConvexTriangles.h"

namespace geometry{
 
  class ConvexHullPoints;

  class ConvexHullPlanes
  {
    public:
      std::vector<Plane>planes;
      ConvexHullPlanes();
      ConvexHullPlanes(ConvexHullPoints hull);
      ConvexHullPlanes(glm::mat4 p,glm::mat4 v);
      ConvexHullPlanes(glm::vec3 minPoint,glm::vec3 maxPoint);
      void insert(Plane plane);
      bool inside(glm::vec3 point);
      bool inside(glm::vec3 point,unsigned aPlane,unsigned bPlane,unsigned cPlane);
      std::string toStr();
  };

  ConvexHullPoints intersect(ConvexHullPlanes a,ConvexHullPlanes b);

  class ConvexHullPoints
  {
    public:
      std::vector<glm::vec3>points;
      std::vector<Plane>planes;
      std::vector<unsigned>aPlane;
      std::vector<unsigned>bPlane;
      std::vector<unsigned>cPlane;
      ConvexHullPoints();
      ConvexHullPoints(ConvexHullPlanes hull);
      int hullPlane(Plane plane,unsigned ai,unsigned bi,unsigned ci);
      std::string toStr();
  };

  class ConvexHullPointPlane
  {
    public:
      std::vector<glm::vec3>points;
      std::vector<Plane>planes;
      std::vector<unsigned>planeIndices;
      std::vector<unsigned>count;
      std::vector<unsigned>offset;
      ConvexHullPointPlane(ConvexHullPoints hull);
      std::string toStr();
  };

  class ConvexHullFaces
  {
    public:
      std::vector<glm::vec3>points;
      std::vector<unsigned>index;
      std::vector<unsigned>count;
      std::vector<unsigned>offset;
      std::vector<Plane>planes;
      ConvexHullFaces(ConvexHullPointPlane hull);
      std::string toStr();
  };

  class ConvexHullTriangles
  {
    public:
      std::vector<glm::vec3>points;
      std::vector<unsigned>triangles;
      void _constructor(ConvexHullFaces hull);
      ConvexHullTriangles(ConvexHullFaces hull);
      ConvexHullTriangles(ConvexHullPoints hull);
      ConvexHullTriangles(ConvexHullPlanes hull);

      std::string toStr();
      void draw(DrawPrimitive*drawer);
  };

  class ConvexHullSortedFaces
  {
    public:
      std::vector<glm::vec3>points;
      std::vector<unsigned>index;
      std::vector<unsigned>count;
      std::vector<unsigned>offset;
      std::vector<Plane>planes;
      ConvexHullSortedFaces(ConvexHullFaces hull);
      std::string toStr();
  };

  class ConvexHullLineSegments
  {
    public:
      std::vector<glm::vec3>points;
      std::vector<unsigned>line;
      std::vector<Plane>planes;
      ConvexHullLineSegments(ConvexHullSortedFaces hull);
      std::string toStr();
  };

  class ConvexHullPointLineSegments
  {
    public:
      std::vector<glm::vec3>points;
      std::vector<unsigned>uniqPoints;
      std::vector<unsigned>lookUp;
      std::vector<unsigned>index;
      std::vector<unsigned>count;
      std::vector<unsigned>offset;
      std::vector<Plane>planes;
      ConvexHullPointLineSegments(ConvexHullSortedFaces hull);
      std::string toStr();
  };

  class ConvexHullSilhouette
  {
    public:
      std::vector<glm::vec3>points;
      std::vector<unsigned>lineLoop;
      glm::vec3 light;
      bool isSilhouette(ConvexHullPointLineSegments hull,unsigned ai,unsigned bi,glm::vec3 light);
      ConvexHullSilhouette(ConvexHullPointLineSegments hull,glm::vec3 light);
      std::string toStr();
  };

  class MinFrustumPlanes
  {
    public:
      Plane bestL;
      Plane bestR;
      Plane bestB;
      Plane bestT;
      Plane bestN;
      Plane bestF;
      glm::mat4 view;
      glm::mat4 projection;
      glm::vec3 light;
      bool isOutside(ConvexHullSilhouette hull,Plane plane,unsigned ai,unsigned bi);
      bool isInFrontOf(ConvexHullSilhouette hull,Plane plane,unsigned ai);
      bool isOutside(ConvexHullLineSegments hull,Plane plane,unsigned ai,unsigned bi);

      MinFrustumPlanes(ConvexHullSilhouette hull);
      MinFrustumPlanes(ConvexHullLineSegments hull,glm::vec3 light);
      void computeMatrices();
  };

  bool getMinimalVP(glm::mat4*lp,glm::mat4*lv,glm::mat4 p,glm::mat4 v,glm::vec3 minPoint,glm::vec3 maxPoint,glm::vec3 light);

  geometry::ConvexHullPoints getExtendedHull(glm::mat4 p,glm::mat4 v,glm::vec3 minPoint,glm::vec3 maxPoint,glm::vec3 light);
}




#endif//_GEOMETRY_H_
