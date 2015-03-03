#include"CTessellationSides.hpp"
#include"CGenerateTessellationShaderSilhouette.hpp"

CTessellationSides::CTessellationSides(
    SAdjecency*A,
    bool UseReferenceEdge,
    bool CullSides,
    bool UseStencilValueExport){ 

  //std::cerr<<"################################## new TessellationSides sides "<<std::endl;
  this->Adjacency=A;
  unsigned PatchVertices=2+1+this->Adjacency->MaxOpposite;
  this->VAO=new ge::gl::VertexArrayObject();
  this->EBO=new ge::gl::BufferObject(
      sizeof(unsigned)*PatchVertices*this->Adjacency->NumEdges,
      NULL,GL_STATIC_DRAW);
  unsigned*EPtr=(unsigned*)this->EBO->map();
  for(unsigned e=0;e<this->Adjacency->NumEdges;++e){//loop over edges
		EPtr[e*PatchVertices+0]=this->Adjacency->Edges[e][0];
		EPtr[e*PatchVertices+1]=this->Adjacency->Edges[e][1];
		EPtr[e*PatchVertices+2]=this->Adjacency->NumPoints+e;
		for(unsigned o=0;o<this->Adjacency->MaxOpposite;++o)
			if(o<this->Adjacency->EdgeOppositeNum[e]){
				EPtr[e*PatchVertices+3+o]=this->Adjacency->EdgeOpposite[e][o];
			}else{
				EPtr[e*PatchVertices+3+o]=0;
			}
	}
  this->EBO->unmap();

  this->VBO=new ge::gl::BufferObject(
      sizeof(float)*4*
      (this->Adjacency->NumPoints+
      this->Adjacency->NumEdges),
      NULL,GL_STATIC_DRAW);
  float*Ptr=(float*)this->VBO->map();
	for(unsigned p=0;p<this->Adjacency->NumPoints;++p){//loop over points
		for(unsigned e=0;e<3;++e)
      Ptr[p*4+e]=this->Adjacency->Points[p*3+e];
		Ptr[p*4+3]=1;
	}
	for(unsigned e=0;e<this->Adjacency->NumEdges;++e){//loop over edges
		Ptr[(this->Adjacency->NumPoints+e)*4+0]=
      this->Adjacency->EdgeOppositeNum[e];
	}
  this->VBO->unmap();

  this->VAO->bind();
  this->EBO->bind(GL_ELEMENT_ARRAY_BUFFER);
  this->VBO->bind(GL_ARRAY_BUFFER);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,
    sizeof(float)*4,(GLvoid*)(sizeof(float)*0));
  this->VAO->unbind();
  this->EBO->unbind(GL_ELEMENT_ARRAY_BUFFER);
  this->VBO->unbind(GL_ARRAY_BUFFER);

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
        this->Adjacency->MaxOpposite,TTS));
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
  unsigned PatchVertices=2+1+this->Adjacency->MaxOpposite;
  this->VAO->bind();
  this->Program->use();
  this->Program->set("mvp",1,GL_FALSE,mvp);
  this->Program->set("LightPosition",1,glm::value_ptr(Light->position));
  glPatchParameteri(GL_PATCH_VERTICES,PatchVertices);
  glDrawElements(GL_PATCHES,PatchVertices*this->Adjacency->NumEdges,
      GL_UNSIGNED_INT,NULL);
  this->VAO->unbind();
}

