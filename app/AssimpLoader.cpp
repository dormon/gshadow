#include"AssimpLoader.h"
#include<map>

namespace ge{
  namespace db{
    class AssimpScene: public Scene{
      protected:
        const aiScene*_data;
        std::map<aiMesh*,MeshGeometry*>_aiMesh2MeshGeometry;
        Mesh* _createMesh (aiMesh*mesh);
        Model*_createModel(aiNode*node,const aiScene*scene);
      public:
        AssimpScene(const aiScene*data);
        ~AssimpScene();
    };
  }
}

using namespace ge::db;

unsigned AssimpLoader::_parseFlags(std::string file){
  //TODO
  return /*aiProcess_CalcTangentSpace|*/
    aiProcess_Triangulate|
    aiProcess_GenNormals |
    aiProcess_SortByPType;
}

Scene*AssimpLoader::load(std::string file){
  const aiScene*data = aiImportFile(file.c_str(),this->_parseFlags(file));
  if(data==NULL)return NULL;
  return new AssimpScene(data);
}

Mesh*AssimpScene::_createMesh(aiMesh*mesh){
  Mesh*m=new Mesh();
  m->matrix = glm::mat4(1.f);

  if(this->_aiMesh2MeshGeometry.count(mesh)){
    m->geometry = this->_aiMesh2MeshGeometry[mesh];
    return m;
  }
  m->geometry = new MeshGeometry();
  this->_aiMesh2MeshGeometry.insert(std::pair<aiMesh*,MeshGeometry*>(mesh,m->geometry));
  this->geometries.push_back(m->geometry);


  std::vector<unsigned>indices;
  for(unsigned f=0;f<mesh->mNumFaces;++f)
    for(unsigned i=0;i<mesh->mFaces[f].mNumIndices;++i)
      indices.push_back(mesh->mFaces[f].mIndices[i]);


  if(mesh->mPrimitiveTypes&aiPrimitiveType_TRIANGLE){
    m->geometry->primitives = new MeshPrimitives(
        indices.size(),
        MeshPrimitives::TRIANGLE,
        3,
        indices);
  }
  //TODO other types of primitives

  m->geometry->vertices = new MeshVertices(indices.size());

  if(mesh->HasPositions()){
    MeshGeometry::createxAttribData(
        m->geometry->vertices,
        &m->geometry->positionData,
        indices.size(),
        VertexAttribDataDescriptor(3,
          VertexAttribDataDescriptor::F32,VertexAttribDataDescriptor::POSITION));
    float*ptr=(float*)m->geometry->positionData;
    for(unsigned i=0;i<indices.size();++i)
      for(unsigned k=0;k<3;++k)
        ptr[i*3+k]=mesh->mVertices[i][k];
  }
  if(mesh->HasNormals()){
    MeshGeometry::createxAttribData(
        m->geometry->vertices,
        &m->geometry->normalData,
        indices.size(),
        VertexAttribDataDescriptor(3,
          VertexAttribDataDescriptor::F32,VertexAttribDataDescriptor::NORMAL));
    float*ptr=(float*)m->geometry->normalData;
    for(unsigned i=0;i<indices.size();++i)
      for(unsigned k=0;k<3;++k)
        ptr[i*3+k]=mesh->mNormals[i][k];
  }
  //TODO other types of attribs



  return m;

}

Model*AssimpScene::_createModel(aiNode*node,const aiScene*scene){
  Model*model=new Model();
  for(unsigned i=0;i<node->mNumMeshes;++i)
    model->meshes.push_back(AssimpScene::_createMesh(
          scene->mMeshes[node->mMeshes[i]]));
  for(unsigned n=0;n<node->mNumChildren;++n)
    model->models.push_back(AssimpScene::_createModel(
          node->mChildren[n],
          scene));
  return model;
}

AssimpScene::AssimpScene(const aiScene*data){
  this->_data=data;
  this->root = AssimpScene::_createModel(
      this->_data->mRootNode,this->_data);
}

AssimpScene::~AssimpScene(){
  aiReleaseImport(this->_data);
  delete (Scene*)this;
}
