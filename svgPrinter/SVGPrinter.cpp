#include"SVGPrinter.h"

#include<fstream>
#include<sstream>
#include<algorithm>

#include<limits>
using namespace svg;

namespace glm{
  bool insideVF(glm::vec4 p){
    return
      glm::any(glm::greaterThan(glm::vec3(p),glm::vec3(-p.w)))&&
      glm::any(glm::lessThan   (glm::vec3(p),glm::vec3(+p.w)));
  }
}

Triangle3D::Triangle3D(glm::vec4 a,glm::vec4 b,glm::vec4 c,glm::vec3 color){
  this->p[0]=a;
  this->p[1]=b;
  this->p[2]=c;
  this->color=color;
}
void Triangle3D::toNDC(){
  for(int i=0;i<3;++i)this->p[i]/=this->p[i].w;
}

std::string Triangle3D::toSvg(unsigned size[2]){
  if(!this->useful())return"";
  glm::vec2 p2[3];
  for(int i=0;i<3;++i)p2[i]=(glm::vec2(this->p[i])/this->p[i].w*.5f+.5f)*glm::vec2((float)size[0],(float)size[1]);
  std::stringstream ss;
  ss<<"<path style=\"fill:#";
  for(int i=0;i<3;++i)ss<<std::hex<<(int)(this->color[i]*255);
  ss<<"\" d=\"M ";
  for(int i=0;i<3;++i)ss<<std::dec<<p2[i].x<<","<<p2[i].y<<" ";
  ss<<"z\"/>"<<std::endl;
  return ss.str();
}

bool isEdgeVisible(glm::vec4 A,glm::vec4 B){
  glm::vec3 M=glm::vec3( A)+glm::vec3(A.w);
  glm::vec3 N=glm::vec3( B)+glm::vec3(B.w);
  glm::vec3 O=glm::vec3(-A)+glm::vec3(A.w);
  glm::vec3 P=glm::vec3(-B)+glm::vec3(B.w);
  glm::vec3 NM=N-M;
  glm::vec3 PO=P-O;
  float Left=0;
  float Right=1;
  for(int i=0;i<3;++i){
    if(NM[i]==0){
      if(M[i]<0)return false;
    }else{
      if(NM[i]>0)Left=glm::max(Left,-M[i]/NM[i]);
      else Right=glm::min(Right,-M[i]/NM[i]);
    }
                                              
    if(PO[i]==0){
      if(O[i]<0)return false;
    }else{
      if(PO[i]>0)Left=glm::max(Left,-O[i]/PO[i]);
      else Right=glm::min(Right,-O[i]/PO[i]);
    }
  }
  return Left<=Right;
}

bool isInsideTriangle(glm::vec4 p[3],glm::vec2 x){
  return false;
}

Triangle3D::TriangleNDCCollision Triangle3D::useful(){
  for(int i=0;i<3;++i)
    if(isEdgeVisible(this->p[i],this->p[(i+1)%3]))
      return Triangle3D::EDGE;
  for(int i=0;i<4;++i)
    if(isInsideTriangle(this->p,glm::vec2(-1.f+2.f*(float)(i%2),-1.f+2.f*(float)(i/2))))
      return Triangle3D::FULL;
  return Triangle3D::NONE;
}

int Triangle3D::operator<(const Triangle3D&b) const{
  //TODO
  return 0;
}


Print::Print(std::string name,unsigned windowSize[2]){
  this->_size[0]=windowSize[0];
  this->_size[1]=windowSize[1];
  this->_f.open(name.c_str());
  this->_f<<"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"<<std::endl;
  this->_f<<"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">"<<std::endl;
  this->_f<<"<svg width=\""<<this->_size[0]<<"\" height=\""<<this->_size[1]<<"\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">"<<std::endl;
}

void Print::_clip(std::vector<Triangle3D>&clipped,Triangle3D tri){
  glm::vec4 p[3];
  glm::vec4 v[3];
  for(int i=0;i<3;++i){
    p[i]=tri.p[i];
    v[i]=tri.p[(i+1)%3]-tri.p[i];
  }
  glm::vec2 b=glm::vec2(-1.f,+1.f);
  float t[3][2][2];
  for(int i=0;i<3;++i)
    for(int j=0;j<2;++j)
      for(int k=0;k<2;++k){
        if(v[i][j]==0)t[k][j][i]=std::numeric_limits<float>::infinity();
        else{
          t[k][j][i]=(b[k]-p[i][j])/v[i][j];
          if(t[k][j][i]<0||t[k][j][i]>1)t[k][j][i]=std::numeric_limits<float>::infinity();
        }
      }



  //(pi+vi*t).j=b.k
  //pi.j+vi.j*t=b.k
  //(b.k-pi.j)/vi.j=t
  //t=b.k/vi.j - pi.j/vi.j
  //TODO
}

void Print::addTriangle(
    svg::Triangle3D triangle){
  triangle.toNDC();
  Triangle3D::TriangleNDCCollision type=triangle.useful();
  if(type==Triangle3D::NONE)return;
  if(type==Triangle3D::FULL){


    return;
  }
  triangle.toNDC();
  std::vector<Triangle3D>clipped;
  this->_clip(clipped,triangle);
  for(int i=0;i<3;++i)this->_triangles.push_back(clipped[i]);
}

Print::~Print(){
  /*
     std::vector<unsigned>a;
     a.push_back(2);
     a.push_back(3);
     a.push_back(1);
     std::sort(a.begin(),a.end());*/
  std::sort(this->_triangles.begin(),this->_triangles.end());
  for(unsigned i=0;i<this->_triangles.size();++i)
    this->_f<<this->_triangles[i].toSvg(this->_size);
  this->_f<<"</svg>";
  this->_f.close();
}

