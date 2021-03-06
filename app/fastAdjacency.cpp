#include"fastAdjacency.h"

#include<iostream>
#include<algorithm>
#include<cstring>
#include<sstream>

template<unsigned dim>
class Vertex{
  public:
    const float* data = nullptr;///<global data
    unsigned index = 0;   ///<this vertex starts here
    Vertex(const float*data,unsigned i){
      this->data  = data;
      this->index = i;
    }
    Vertex(){this->data=nullptr;}
    int compare(Vertex<dim>const& b)const{
      for(unsigned d=0;d<dim;++d){
        if(this->data[this->index+d]>b.data[b.index+d])return 1;
        if(this->data[this->index+d]<b.data[b.index+d])return-1;
      }
      return 0;
    }
    std::string toStr(){
      std::stringstream ss;
      for(unsigned d=0;d<dim;++d){
        ss<<this->data[this->index+d];
        if(d<dim-1)ss<<" ";
      }
      return ss.str();
    }
};



class EdgeToTriangle{
  public:
    Vertex<3>a;
    Vertex<3>b;
    Vertex<3>c;
    EdgeToTriangle(Vertex<3>const&a,Vertex<3>const&b,Vertex<3>const&c){
      this->a=a;
      this->b=b;
      this->c=c;
    }
    bool operator<(EdgeToTriangle const& edge)const{
      int aea=this->a.compare(edge.a);
      if(aea<0)return true;
      if(aea>0)return false;
      int beb=this->b.compare(edge.b);
      if(beb<0)return true;
      if(beb>0)return false;
      int cec=this->c.compare(edge.c);
      if(cec<0)return true;
      if(cec>0)return false;
      return false;
    }
    bool edgeEqual(EdgeToTriangle const& edge){
      if(this->a.compare(edge.a)!=0)return false;
      if(this->b.compare(edge.b)!=0)return false;
      return true;
    }
    bool operator==(EdgeToTriangle const& edge)const{
      if(this->a.compare(edge.a)!=0)return false;
      if(this->b.compare(edge.b)!=0)return false;
      if(this->c.compare(edge.c)!=0)return false;
      return true;
    }
};


Adjacency::Adjacency(const float*vertices,unsigned nofTriangles,unsigned maxMult){
  this->_vertices        = vertices;
  this->_maxMultiplicity = maxMult;
  this->_nofTriangles    = nofTriangles;
  std::vector<EdgeToTriangle>edgeToTriangle;
  edgeToTriangle.reserve(3*nofTriangles);
  for(unsigned t=0;t<nofTriangles;++t){//loop over triangles
    Vertex<3>a(vertices,(t*3+0)*3);
    Vertex<3>b(vertices,(t*3+1)*3);
    Vertex<3>c(vertices,(t*3+2)*3);
    int ab=a.compare(b);
    int ac=a.compare(c);
    int bc=b.compare(c);
    if(ab==0)continue;
    if(ac==0)continue;
    if(bc==0)continue;
    if(ab<0)edgeToTriangle.push_back(EdgeToTriangle(a,b,c));
    else    edgeToTriangle.push_back(EdgeToTriangle(b,a,c));
    if(ac<0)edgeToTriangle.push_back(EdgeToTriangle(a,c,b));
    else    edgeToTriangle.push_back(EdgeToTriangle(c,a,b));
    if(bc<0)edgeToTriangle.push_back(EdgeToTriangle(b,c,a));
    else    edgeToTriangle.push_back(EdgeToTriangle(c,b,a));
  }
  std::sort(edgeToTriangle.begin(),edgeToTriangle.end());

  this->_edges.push_back(EdgeAdjacency(edgeToTriangle[0].a.index,edgeToTriangle[0].b.index,0,1));
  this->_opposite.push_back(edgeToTriangle[0].c.index);

  unsigned uniqueIndex=0;
  for(unsigned i=1;i<edgeToTriangle.size();++i){
    if( (--this->_edges.end())->count<this->_maxMultiplicity &&
        edgeToTriangle[uniqueIndex].edgeEqual(edgeToTriangle[i])){
      this->_opposite.push_back(edgeToTriangle[i].c.index);
      (--this->_edges.end())->count++;
      continue;
    }

    unsigned offset=(--this->_edges.end())->count+(--this->_edges.end())->offset;
    this->_edges.push_back(
        EdgeAdjacency(edgeToTriangle[i].a.index,edgeToTriangle[i].b.index,offset,1));
    this->_opposite.push_back(edgeToTriangle[i].c.index);
    uniqueIndex=i;
  }
}

unsigned Adjacency::getNofEdges(){
  return this->_edges.size();
}

unsigned Adjacency::getEdge(unsigned e,unsigned i){
  return this->_edges[e].ab[i];
}

unsigned Adjacency::getNofOpposite(unsigned e){
  return this->_edges[e].count;
}

unsigned Adjacency::getOpposite(unsigned e,unsigned i){
  return this->_opposite[this->_edges[e].offset+i];
}
unsigned Adjacency::getMaxMultiplicity(){
  return this->_maxMultiplicity;
}
const float*Adjacency::getVertices(){
  return this->_vertices;
}
unsigned Adjacency::getNofTriangles(){
  return this->_nofTriangles;
}

