#include"CreateCountMap.h"

CreateCountMap::CreateCountMap(
    std::string dir,
    ge::gl::TextureObject*viewSamples,
    ge::gl::TextureObject*countMap   ,
    ge::gl::TextureObject*desiredView,
    unsigned resolution,
    unsigned width,
    unsigned height,
    unsigned wgsx,
    unsigned wgsy){
  this->_dir = dir;
  this->_viewSamples = viewSamples;
  this->_countMap = countMap;
  this->_desiredView = desiredView;
  this->_resolution = resolution;
  this->_width  = width;
  this->_height = height;
  this->_wgsx = wgsx;
  this->_wgsy = wgsy;
  this->_program = new ge::gl::ProgramObject(
      this->_dir+"countMap.comp",
      ge::gl::ShaderObject::define("WORKGROUP_SIZE_X",this->_wgsx)+
      ge::gl::ShaderObject::define("WORKGROUP_SIZE_Y",this->_wgsy)+
      ge::gl::ShaderObject::include(this->_dir+"dv.vp")+ge::gl::ShaderObject::include(this->_dir+"nv.vp")+
      "");

}

CreateCountMap::~CreateCountMap(){
  delete this->_program;
}

void CreateCountMap::setResolution(unsigned resolution){
  this->_resolution = resolution;
}
void CreateCountMap::setViewSamples(ge::gl::TextureObject*viewSamples){
  this->_viewSamples = viewSamples;
}

void CreateCountMap::setCountMap   (ge::gl::TextureObject*countMap   ){
  this->_countMap = countMap;
}

void CreateCountMap::setDesiredView(ge::gl::TextureObject*desiredView){
  this->_desiredView = desiredView;
}

void CreateCountMap::operator()(){
  unsigned data[]={1};
  glClearTexImage(this->_countMap->getId(),0,GL_RED_INTEGER,GL_UNSIGNED_INT,data);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  this->_viewSamples->bindImage(0,0);
  this->_countMap   ->bindImage(1,0);
  this->_desiredView->bindImage(2,0);
  this->_program->use();
  this->_program->set("shadowMapSize",this->_resolution);
  this->_program->set("windowSize",this->_width,this->_height);
  unsigned workSizex=ge::core::getDispatchSize(this->_width ,this->_wgsx);
  unsigned workSizey=ge::core::getDispatchSize(this->_height,this->_wgsy);
  glDispatchCompute(workSizex,workSizey,1);
}

