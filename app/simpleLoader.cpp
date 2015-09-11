#include"simpleLoader.h"

#include<assimp/cimport.h>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

ModelPN::ModelPN(const aiScene*sceneModelPN){
  if(!sceneModelPN){
    std::cerr<<"error while loading opened assimp file"<<std::endl;
    return;
  }

  this->_meshOffset.push_back(0);

  unsigned nofSceneVertices=0;
  std::vector<unsigned>correctMesh;
  for(unsigned m=0;m<sceneModelPN->mNumMeshes;++m)
    if(
        sceneModelPN->mMeshes[m]->HasPositions    ( )&&
        sceneModelPN->mMeshes[m]->HasNormals      ( )/*&&
        !sceneModelPN->mMeshes[m]->HasTextureCoords(0)*/){
      correctMesh.push_back(m);
      nofSceneVertices+=sceneModelPN->mMeshes[m]->mNumFaces*3;
      this->_meshOffset.push_back(nofSceneVertices);
    }
  
  this->_positions = new float[nofSceneVertices*3*sizeof(float)];
  this->_normals   = new float[nofSceneVertices*3*sizeof(float)];

  unsigned curVertex=0;
  for(auto x:correctMesh){
    aiMesh*mesh=sceneModelPN->mMeshes[x];
    for(unsigned i=0;i<mesh->mNumFaces;++i){
      aiFace*face=mesh->mFaces+i;
      for(unsigned j=0;j<3;++j){
        aiVector3D*p=mesh->mVertices+face->mIndices[j];
        aiVector3D*n=mesh->mNormals +face->mIndices[j];
        for(unsigned k=0;k<3;++k)
          this->_positions[curVertex*3+k]=(*p)[k];
        for(unsigned k=0;k<3;++k)
          this->_normals[curVertex*3+k]=(*n)[k];
        curVertex++;
      }
    }
  }
}

ModelPN::~ModelPN(){
  delete[]this->_positions;
  delete[]this->_normals  ;
}

const float*ModelPN::getPositions(){
  return this->_positions;
}

const float*ModelPN::getNormals  (){
  return this->_normals;
}

unsigned ModelPN::getNofMeshes (){
  if(this->_meshOffset.size()==0)return 0;
  return this->_meshOffset.size()-1;
}

unsigned ModelPN::getMeshOffset(unsigned i){
  return this->_meshOffset[i];
}

unsigned ModelPN::getMeshSize  (unsigned i){
  return this->_meshOffset[i+1]-this->_meshOffset[i];
}

unsigned ModelPN::getNofVertices(){
  return this->_meshOffset[this->_meshOffset.size()-1];
}
