#include"ConvexTriangles.h"

#include<limits>

#include<iostream>
#include<sstream>


using namespace geometry;

ConvexTriangles::ConvexTriangles(ConvexHull*hull){
  //std::cerr<<"ConvexTriangles"<<std::endl;
  //std::cerr<<hull->allToStr();
  hull->sortFaces();
  //std::cerr<<hull->allToStr();


  //std::cerr<<"serazeno"<<std::endl;


  for(unsigned i=0;i<hull->points.size();++i)
    this->points.push_back(hull->points[i]->point);

  //std::cerr<<"jo0"<<std::endl;
  for(unsigned i=0;i<hull->planes.size();++i){
    PlaneC*currentPlane=hull->planes[i];
    for(unsigned j=1;j<currentPlane->points.size()-1;++j){
      this->triangles.push_back(currentPlane->points[0]->index);
      this->triangles.push_back(currentPlane->points[j]->index);
      this->triangles.push_back(currentPlane->points[j+1]->index);
    }
  }
  //std::cerr<<"ConvexTriangles end"<<std::endl;

}
void ConvexTriangles::draw(DrawPrimitive*drawer){
  for(unsigned t=0;t<this->triangles.size()/3;++t)
    drawer->triangle(
        this->points[this->triangles[t*3+0]],
        this->points[this->triangles[t*3+1]],
        this->points[this->triangles[t*3+2]]);
}

