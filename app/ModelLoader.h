#ifndef _MODELLOADER_H_
#define _MODELLOADER_H_

#include<geGL/geGL.h>

#include<iostream>

#include<assimp/cimport.h>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#define MAX_TEXTURES 256

class ModelLoader
{
  public:
    std::string getDirectory(std::string fileName);
    void loadMaterials(const aiScene*scene);
    ge::gl::VertexArrayObject  *vao;
    ge::gl::BufferObject       *vbo;
    ge::gl::BufferObject       *ebo;
    ge::gl::BufferObject       *dibo;
    ge::gl::BufferObject       *material;
    ge::gl::ProgramObject      *cgbProgram;
    std::vector<ge::gl::TextureObject*>textures;
    GLuint handles[MAX_TEXTURES];
    ModelLoader(std::string fileName);
    ~ModelLoader();
};

#endif//_MODELLOADER_H_
