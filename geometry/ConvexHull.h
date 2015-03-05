#ifndef _CONVEXHULL_H_
#define _CONVEXHULL_H_

#include"Common.h"
#include"Plane.h"
#include"WorkingObjects.h"

namespace geometry{
  class ConvexHull{
    public:
      std::vector<PlaneC*>planes;//every surface plane; can contain creator plane if its on surface
      std::vector<PointC*>points;
      void recomputePoints();
      void deletePlanes();
      void deleteLineSegments();
      void deletePoints();
      void setPlanes(std::vector<Plane>&planes);
      void _getNewPoints(std::vector<PointC*>&newPoints,std::vector<PlaneC*>&aPlanes,std::vector<PlaneC*>&bPlanes);
      void _getNewPoints(
          std::vector<PointC*>&newPoints,
          std::vector<PlaneC*>&aPlanes,
          unsigned aS,unsigned aE,
          std::vector<PlaneC*>&bPlanes,
          unsigned bS,unsigned bE,bool ai,
          std::vector<PlaneC*>&cPlanes,
          unsigned cS,unsigned cE,bool bi,
          std::vector<PointC*>usefull,
          std::vector<PointC*>useless);
      void _getSilhouetteVertices(std::vector<PointC*>&sil,glm::vec3 p);

      void _getUsefulPoints(
          std::vector<PointC*>&useful,
          std::vector<PointC*>&useless,
          std::vector<PointC*>newPoints,
          std::vector<PlaneC*>newPlanes);
      bool _isUseful(
          PointC*newPoint,
          std::vector<PlaneC*>planes);

    public:
      ConvexHull();
      ConvexHull(std::vector<Plane>&planes);
      ConvexHull(glm::mat4 p,glm::mat4 v);
      ConvexHull(glm::vec3 minPoint,glm::vec3 maxPoint);
      ~ConvexHull();
      ConvexHull* intersect(ConvexHull*b);
      bool inside(glm::vec3 point);
      bool containPoint(PlaneC*pa,PlaneC*pb,PlaneC*pc);
      std::string toStr();
      std::string allToStr();
      glm::vec3 getCenter();
      unsigned extend(glm::vec3 p);
      void sortFaces();
      void checks();
      
  };

  void camera2Plane(Plane*plane,unsigned*numPlanes,glm::mat4 p,glm::mat4 v);

  bool getMinVP(glm::mat4*lp,glm::mat4*lv,glm::mat4 p,glm::mat4 v,glm::vec3 minPoint,glm::vec3 maxPoint,glm::vec3 light);

 
}

#endif//_CONVEXHULL_H_
