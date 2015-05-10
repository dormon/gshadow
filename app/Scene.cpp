#include"Scene.h"

using namespace ge::db;

VertexAttribDataDescriptor::VertexAttribDataDescriptor(
    unsigned      components,
    enum Type     type      ,
    enum Semantic semantic  ){
  this->components = components;
  this->type       = type      ;
  this->semantic   = semantic  ;
}

unsigned VertexAttribDataDescriptor::size(){
  const unsigned typeBytes[]={
    sizeof(char              ),
    sizeof(short             ),
    sizeof(int               ),
    sizeof(long long         ),
    sizeof(unsigned char     ),
    sizeof(unsigned short    ),
    sizeof(unsigned          ),
    sizeof(unsigned long long),
    sizeof(float             ),
    sizeof(double            ),
  };
  return typeBytes[this->type]*this->components;
}

VertexAttrib::VertexAttrib(
    const void*ptr                           ,
    VertexAttribDataDescriptor dataDescriptor,
    unsigned   offset                        ,
    unsigned   stride                        ){
  this->_ptr            = ptr                                       ;
  this->_dataDescriptor = dataDescriptor                            ;
  this->_stride         = stride?stride:this->_dataDescriptor.size();
  this->_offset         = offset                                    ;
}

const void*VertexAttrib::get(unsigned vertex){
  return (char*)this->_ptr+this->_offset+this->_stride*vertex;
}

const void*VertexAttrib::getPtr(){
  return this->_ptr;
}

unsigned VertexAttrib::getStride(){
  return this->_stride;
}

unsigned VertexAttrib::getOffset(){
  return this->_offset;
}

VertexAttribDataDescriptor&VertexAttrib::getDataDescriptor(){
  return this->_dataDescriptor;
}

MeshVertices::MeshVertices(unsigned nofVertices){
  this->_nofVertices = nofVertices;
}

unsigned MeshVertices::getNofAttribs(){
  return this->_attribs.size();
}

unsigned MeshVertices::addVertexAttrib(VertexAttrib&attrib){
  this->_attribs.push_back(attrib);
  return this->_attribs.size()-1;
}

const void*MeshVertices::getAttrib(
    unsigned index,
    unsigned vertex){
  return this->_attribs[index].get(vertex);
}

unsigned   MeshVertices::getAttribSize(unsigned index){
  return this->_attribs[index].getDataDescriptor().size();
}

MeshPrimitives::MeshPrimitives(
    unsigned  nofVertices            ,
    enum Type type                   ,
    unsigned  nofVerticesPerPrimitive){
  this->type                    = type                   ;
  this->nofVerticesPerPrimitive = nofVerticesPerPrimitive;
  this->indexed                 = false                  ;
  this->restartIndex            = 0xffffffff             ;
  this->enableRestart           = false                  ;
  this->nofVertices             = nofVertices            ;

}

MeshPrimitives::MeshPrimitives(
    unsigned              nofVertices            ,
    enum Type             type                   ,
    unsigned              nofVerticesPerPrimitive,
    std::vector<unsigned>&indices                ,
    bool                  enableRestart          ,
    unsigned              restartIndex           ){
  this->type                    = type                   ;
  this->nofVerticesPerPrimitive = nofVerticesPerPrimitive;
  this->indexed                 = true                   ;
  this->restartIndex            = restartIndex           ;
  this->enableRestart           = enableRestart          ;
  this->nofVertices             = nofVertices            ;
}

void MeshGeometry::createxAttribData(
    MeshVertices*vertices,
    void**data,
    unsigned nofVertices,
    VertexAttribDataDescriptor dataDescriptor){
    *data = new char*[dataDescriptor.size()*nofVertices];
    VertexAttrib attrib((const void*)*data,dataDescriptor);
    vertices->addVertexAttrib(attrib);
}

MeshGeometry::MeshGeometry(){
  this->primitives = NULL;
  this->vertices   = NULL;
  this->positionData = NULL;
  this->normalData   = NULL;
}
MeshGeometry::~MeshGeometry(){
  if(this->primitives)delete this->primitives;this->primitives = NULL;
  if(this->vertices  )delete this->vertices  ;this->vertices   = NULL;
  if(positionData)delete (char*)this->positionData;this->positionData = NULL;
  if(normalData  )delete (char*)this->normalData  ;this->normalData   = NULL;
}

Model::~Model(){
  for(unsigned i=0;i<this->models.size();++i){
    delete this->models[i];
  }
  this->meshes.clear();
}


Scene::~Scene(){
  for(unsigned i=0;i<this->geometries.size();++i)delete this->geometries[i];
  this->geometries.clear();
  delete this->root;
}

bool ModelLoaderManager::_canLoad(std::string regexp,std::string file){
  if(regexp=="")return true;
  //TODO
  return false;
}
void ModelLoaderManager::registerLoader(ModelLoader*loader,std::string regexp){
  this->_loaders.push_back(loader);
  this->_loadersRegExps.push_back(regexp);
}

Scene*ModelLoaderManager::load(std::string file){
  unsigned curLoader=0;
  Scene*scene=NULL;
  while(curLoader<this->_loaders.size()){
    if(this->_canLoad(this->_loadersRegExps[curLoader],file))
      scene = this->_loaders[curLoader]->load(file);
    if(scene)return scene;
  }
  return scene;
}

