#include"CreateDesiredView.h"
#include<geCore/dtemplates.h>

CreateDesiredView::CreateDesiredView(
    std::string shaderDir,
    unsigned width ,
    unsigned height,
    ge::gl::TextureObject*viewSamples,
    ge::gl::TextureObject*desiredView,
    unsigned wgsx,
    unsigned wgsy){
  this->_dir    = shaderDir;
  this->_width  = width;
  this->_height = height;
  this->_viewSamples = viewSamples;
  this->_wgsx = wgsx;
  this->_wgsy = wgsy;

  unsigned size[2]={
    ge::core::getDispatchSize(this->_width ,this->_wgsx),
    ge::core::getDispatchSize(this->_height,this->_wgsy),
  };
  while(size[0]!=1||size[1]!=1){
    this->_dvsWorkSize.push_back(glm::uvec2(size[0],size[1]));
    this->_dvsTex.push_back(new ge::gl::TextureObject(GL_TEXTURE_2D,GL_RGBA32F,1,size[0],size[1]));
    size[0]=ge::core::getDispatchSize(size[0],this->_wgsx);
    size[1]=ge::core::getDispatchSize(size[1],this->_wgsy);
  }
  this->_dvsWorkSize.push_back(glm::uvec2(1u,1u));
  this->_dvsTex.push_back(desiredView);

  this->_fastdv0Program = new ge::gl::ProgramObject(
      this->_dir+"fastdv0.comp",
      ge::gl::ShaderObject::define("WORKGROUP_SIZE_X",(int)this->_wgsx)+
      ge::gl::ShaderObject::define("WORKGROUP_SIZE_Y",(int)this->_wgsy)+
      "");

  this->_fastdvProgram  = new ge::gl::ProgramObject(
      this->_dir+"fastdv.comp" ,
      ge::gl::ShaderObject::define("WORKGROUP_SIZE_X",(int)this->_wgsx)+
      ge::gl::ShaderObject::define("WORKGROUP_SIZE_Y",(int)this->_wgsy)+
      "");
}

void CreateDesiredView::setViewSamples(ge::gl::TextureObject*viewSamples){
  this->_viewSamples = viewSamples;
}

void CreateDesiredView::setDesiredView(ge::gl::TextureObject*desiredView){
  this->_dvsTex[this->_dvsTex.size()-1]=desiredView;
}

CreateDesiredView::~CreateDesiredView(){
  for(unsigned i=0;i<this->_dvsTex.size()-1;++i)delete this->_dvsTex[i];
  this->_dvsTex.clear();

  delete this->_fastdv0Program;
  delete this->_fastdvProgram;
}

void CreateDesiredView::operator()(){
  this->_fastdv0Program->use();
  this->_fastdv0Program->set("windowSize",this->_width,this->_height);
  this->_viewSamples->bindImage(0,0);
  this->_dvsTex[0]->bindImage(1,0);
  glDispatchCompute(this->_dvsWorkSize[0].x,this->_dvsWorkSize[0].y,1);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  this->_fastdvProgram->use();
  for(unsigned i=0;i<this->_dvsTex.size()-1;++i){
    this->_dvsTex[i+0]->bindImage(0,0);
    this->_dvsTex[i+1]->bindImage(1,0);
    this->_fastdvProgram->set("windowSize",this->_dvsWorkSize[i].x,this->_dvsWorkSize[i].y);
    glDispatchCompute(this->_dvsWorkSize[i+1].x,this->_dvsWorkSize[i+1].y,1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  }
}

