#include"CGeometryCapsAlt.hpp"

CGeometryCapsAlt::CGeometryCapsAlt(
    SAdjecency*Adjacency){
  //std::cerr<<"################################## new GeometryCapsAlt sides"<<std::endl;
  this->Adjacency=Adjacency;
  this->VAO=new ge::gl::VertexArrayObject();

  this->VBO=new ge::gl::BufferObject(
      sizeof(float)*4*3*this->Adjacency->NumTriangles,
      NULL,GL_STATIC_DRAW);
  float*Ptr=(float*)this->VBO->map();
	for(unsigned t=0;t<this->Adjacency->NumTriangles;++t){
		for(unsigned p=0;p<3;++p){
			for(unsigned i=0;i<3;++i)
				Ptr[(t*3+p)*4+i]=
          this->Adjacency->Points[this->Adjacency->Triangles[t*3+p]*3+i];
			Ptr[(t*3+p)*4+3]=1;
		}
	}
  this->VBO->unmap();

  this->Program=new ge::gl::ProgramObject(
      ShaderDir+"methods/GeometryCapsAlt/geometrycaps.vp",
      ShaderDir+"methods/GeometryCapsAlt/geometrycaps.gp",
      ShaderDir+"methods/GeometryCapsAlt/geometrycaps.fp");

  //this->VAO->AddAttrib(this->VBO,this->Program,"Position");

  
  this->VAO->bind();
  this->VBO->bind(GL_ARRAY_BUFFER);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,
      sizeof(float)*4,(GLvoid*)(sizeof(float)*0));
  this->VAO->unbind();
  this->VBO->unbind(GL_ARRAY_BUFFER);

}

void CGeometryCapsAlt::DrawCaps(float*mvp,simulation::Light*Light){
  this->Program->use();
  this->Program->set("mvp",1,GL_FALSE,mvp);
  this->Program->set("LightPosition",1,glm::value_ptr(Light->position));
  this->VAO->bind();
  glDrawArrays(GL_TRIANGLES,0,this->Adjacency->NumTriangles*3);
}

CGeometryCapsAlt::~CGeometryCapsAlt(){
  //std::cerr<<"################################## delete GeometryCapsAlt sides"<<std::endl;
  delete this->VBO;
  delete this->VAO;
  delete this->Program;
}
