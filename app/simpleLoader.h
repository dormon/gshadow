#pragma once

#include<iostream>
#include<vector>

class aiScene;

class ModelPN{
  protected:
    float*_positions = nullptr;
    float*_normals   = nullptr;
    std::vector<unsigned>_meshOffset;
  public:
    ModelPN(const aiScene*sceneModel);
    ~ModelPN();
    const float*getPositions();
    const float*getNormals  ();
    unsigned getNofMeshes ();
    unsigned getMeshOffset(unsigned i);
    unsigned getMeshSize  (unsigned i);
    unsigned getNofVertices();
};
