#include"AdjacencyToVBO.h"

AdjacencyToVBO::AdjacencyToVBO(ge::gl::BufferObject**vbo,Adjacency*adjacency){
  this->_vbo = vbo;
  this->_adjacency = adjacency;
}

void AdjacencyToVBO::operator()(){
  unsigned numVer=2+this->_adjacency->getMaxMultiplicity();
  *this->_vbo=new ge::gl::BufferObject(sizeof(float)*4*numVer*this->_adjacency->getNofEdges());

  float*Ptr=(float*)(*this->_vbo)->map();

  for(unsigned e=0;e<this->_adjacency->getNofEdges();++e){
    //A
    for(int k=0;k<3;++k)
      Ptr[(e*numVer+0)*4+k]=this->_adjacency->getVertices()[this->_adjacency->getEdge(e,0)+k];
    Ptr[(e*numVer+0)*4+3]=this->_adjacency->getNofOpposite(e);
    //B
    for(int k=0;k<3;++k)
      Ptr[(e*numVer+1)*4+k]=this->_adjacency->getVertices()[this->_adjacency->getEdge(e,1)+k];
    Ptr[(e*numVer+1)*4+3]=1;
    //O
    for(unsigned o=0;o<this->_adjacency->getNofOpposite(e);++o){
      for(int k=0;k<3;++k)
        Ptr[(e*numVer+2+o)*4+k]=this->_adjacency->getVertices()[this->_adjacency->getOpposite(e,o)+k];
      Ptr[(e*numVer+2+o)*4+3]=1;
    }
    for(unsigned o=this->_adjacency->getNofOpposite(e);o<this->_adjacency->getMaxMultiplicity();++o){
      for(int k=0;k<4;++k)
        Ptr[(e*numVer+2+o)*4+k]=0;
    }
  }
  (*this->_vbo)->unmap();
}

