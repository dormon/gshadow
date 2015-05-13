#include"CTessellationSides.hpp"
#include"CGenerateTessellationShaderSilhouette.hpp"

#include"../../app/fastAdjacency.h"

CTessellationSides::CTessellationSides(
    Adjacency*ad,
    bool UseReferenceEdge,
    bool CullSides,
    bool UseStencilValueExport){ 

  this->_patchVertices = 2+1+ad->getMaxMultiplicity();
  this->_nofEdges      = ad->getNofEdges();
  this->EBO = new ge::gl::BufferObject(
      sizeof(unsigned)*this->_patchVertices*ad->getNofEdges());
  unsigned*eptr=(unsigned*)this->EBO->map();
  for(unsigned e=0;e<ad->getNofEdges();++e){//loop over edges
    unsigned base=e*this->_patchVertices;
		eptr[base+0]=ad->getEdge(e,0)/3;
		eptr[base+1]=ad->getEdge(e,1)/3;
		eptr[base+2]=ad->getNofTriangles()*3+e;
		for(unsigned o=0;o<ad->getMaxMultiplicity();++o)
			if(o<ad->getNofOpposite(e))eptr[base+3+o]=ad->getOpposite(e,o)/3;
			else eptr[base+3+o]=0;
  }
  this->EBO->unmap();

  this->VBO=new ge::gl::BufferObject(
      sizeof(float)*4*(ad->getNofTriangles()*3+ad->getNofEdges()));
  float*ptr=(float*)this->VBO->map();
	for(unsigned p=0;p<ad->getNofTriangles()*3;++p){//loop over points
		for(unsigned e=0;e<3;++e)
      ptr[p*4+e]=ad->getVertices()[p*3+e];
		ptr[p*4+3]=1;
	}
	for(unsigned e=0;e<ad->getNofEdges();++e)
		ptr[(ad->getNofTriangles()*3+e)*4+0]=ad->getNofOpposite(e);
  this->VBO->unmap();

  this->VAO=new ge::gl::VertexArrayObject();
  this->VAO->addAttrib(this->VBO,0,4,GL_FLOAT);
  this->VAO->addElementBuffer(this->EBO);

	STSSilTemplate TTS;
	TTS.Version=430;
	TTS.UseLayouts=true;
	TTS.Universal=true;
	TTS.UseSillyPerPatchLevel=true;
	TTS.UseOptimizedDegeneration=true;
	TTS.UseCompatibility=false;
	TTS.LightPositionUniformName="LightPosition";
	TTS.MatrixUniformName="mvp";
	TTS.VertexAttribName="Position";
	TTS.UseReferenceEdge=UseReferenceEdge;
  TTS.CullSides=CullSides;
  TTS.UseStencilValueExport=UseStencilValueExport;

	ge::gl::ShaderObject*TSSilVerHull2=new ge::gl::ShaderObject(
      GL_VERTEX_SHADER,GenTSSilVertexHull(TTS));
	ge::gl::ShaderObject*TSSilConHull2=new ge::gl::ShaderObject(
      GL_TESS_CONTROL_SHADER,GenTSSilControlHull(
      ad->getMaxMultiplicity(),TTS));
	ge::gl::ShaderObject*TSSilEvaHull2=new ge::gl::ShaderObject(
      GL_TESS_EVALUATION_SHADER,GenTSSilEvaluationHull(TTS));
	ge::gl::ShaderObject*TSSilFragHull2=new ge::gl::ShaderObject(
      GL_FRAGMENT_SHADER,GenTSSilFragmentHull(TTS));
	this->Program=new ge::gl::ProgramObject(
      4,TSSilVerHull2,TSSilConHull2,TSSilEvaHull2,TSSilFragHull2);

  //delete TSSilVerHull2;
  //delete TSSilConHull2;
  //delete TSSilEvaHull2;
  //delete TSSilFragHull2;
}

CTessellationSides::~CTessellationSides(){
  //std::cerr<<"################################## delete TessellationSides sides "<<std::endl;

  delete this->VBO;
  delete this->EBO;
  delete this->VAO;
  delete this->Program;
}

void CTessellationSides::DrawSides(
    float*mvp,
    simulation::Light*Light){
  //unsigned PatchVertices=2+1+this->_adjacency->MaxOpposite;
  this->VAO->bind();
  this->Program->use();
  this->Program->set("mvp",1,GL_FALSE,(const float*)mvp);
  this->Program->set("LightPosition",1,glm::value_ptr(Light->position));
  glPatchParameteri(GL_PATCH_VERTICES,this->_patchVertices);
  glDrawElements(GL_PATCHES,this->_patchVertices*this->_nofEdges,
      GL_UNSIGNED_INT,NULL);
  this->VAO->unbind();
}

