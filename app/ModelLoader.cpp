#include"ModelLoader.h"

#include<map>

#include<glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>


#include<assimp/cimport.h>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#define ILUT_USE_OPENGL
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#define MAX_TEXTURES 256

#define UINT_BIT_SIZE 32
#define MAX_TEXTURES_PER_MESH 6
#define BITS_PER_TEXTURE_ID  ((unsigned)(ceil(std::log(MAX_TEXTURES)/std::log(2))))
#define TEXTURE_IDS_PER_UINT (UINT_BIT_SIZE/BITS_PER_TEXTURE_ID)
#define TEXTURE_ID_MASK      ((1<<BITS_PER_TEXTURE_ID)-1)


#define UINT_TEXURE_ID_PER_MESH\
  ((unsigned)(std::ceil((float)(MAX_TEXTURES_PER_MESH*BITS_PER_TEXTURE_ID)/UINT_BIT_SIZE)))


#define SET_TEXTURE_INDEX(y,x,d)\
  y[x/TEXTURE_IDS_PER_UINT]|=d<<(BITS_PER_TEXTURE_ID*(x%TEXTURE_IDS_PER_UINT))

#define        DIFFUSE_TEXTURE 0
#define       SPECULAR_TEXTURE 1
#define         NORMAL_TEXTURE 2
#define SPECULARFACTOR_TEXTURE 3
#define        OPACITY_TEXTURE 4
#define       LIGHTMAP_TEXTURE 5

const aiTextureType texType[]={//types of textures
	aiTextureType_AMBIENT,
	aiTextureType_DIFFUSE,
	aiTextureType_SPECULAR,
	aiTextureType_EMISSIVE,
	aiTextureType_SHININESS,
	aiTextureType_HEIGHT,//should be normal in assimp
	aiTextureType_OPACITY,
	aiTextureType_LIGHTMAP
};


struct MeshData{
  float    modelMatrix[16];
  unsigned textureId[UINT_TEXURE_ID_PER_MESH];
  unsigned activeTexture;
};

std::string getDirectory(std::string fileName){
  int pos=fileName.rfind("/");
  if(pos==-1)return"./";
  return fileName.substr(pos);
}

void ModelLoader::loadMaterials(const aiScene*scene){
  std::map<unsigned,unsigned>material2Mesh;
  for(unsigned m=0;m<scene->mNumMeshes;++m)
    if(!material2Mesh.count(scene->mMeshes[m]->mMaterialIndex))
      material2Mesh.insert(std::pair<unsigned,unsigned>(scene->mMeshes[m]->mMaterialIndex,m));

  this->material=new ge::gl::BufferObject(sizeof(MeshData)*scene->mNumMeshes);
  MeshData*meshData=(MeshData*)this->material->map();
  for(unsigned m=0;m<scene->mNumMeshes;++m){
    unsigned alreadyCreated=material2Mesh[scene->mMeshes[m]->mMaterialIndex];
    if(alreadyCreated==m){
      //aiMaterial*mat=scene->mMaterials[scene->mMeshes[m]->mMaterialIndex];
      /*
      SET_TEXTURE_INDEX(meshData[m].textureId,       DIFFUSE_TEXTURE,);
      SET_TEXTURE_INDEX(meshData[m].textureId,      SPECULAR_TEXTURE,);
      SET_TEXTURE_INDEX(meshData[m].textureId,        NORMAL_TEXTURE,);
      SET_TEXTURE_INDEX(meshData[m].textureId,SPECULARFACTOR_TEXTURE,);
      SET_TEXTURE_INDEX(meshData[m].textureId,       OPACITY_TEXTURE,);
      SET_TEXTURE_INDEX(meshData[m].textureId,      LIGHTMAP_TEXTURE,);
      */
    }else{
      for(unsigned t=0;t<UINT_TEXURE_ID_PER_MESH;++t)
        meshData[m].textureId[t]=meshData[alreadyCreated].textureId[t];
      meshData[m].activeTexture=meshData[alreadyCreated].activeTexture;
    }
    std::memcpy(meshData[m].modelMatrix,glm::value_ptr(glm::mat4(1.f)),sizeof(float)*16);
  } 
  this->material->unmap();
}

ModelLoader::ModelLoader(std::string fileName){
  const aiScene*model;
  model=aiImportFile(fileName.c_str(),
      aiProcess_CalcTangentSpace|
      aiProcess_Triangulate     |
      aiProcess_GenNormals      |
      //aiProcess_JoinIdenticalVertices|
      aiProcess_SortByPType);
  if(model==NULL)
    throw std::string("model error: "+fileName);

  /*
  for(unsigned m=0;m<model->mNumMeshes;++m){
    aiMesh*mesh = model->mMeshes[m];
  }*/

  this->handles[0]=0;

  /*
  for(unsigned m=0;m<model->mNumMaterials;++m){
    aiMaterial*mat=model->mMaterials[m];
		aiTextureType texType[]={//types of textures
			aiTextureType_AMBIENT,
			aiTextureType_DIFFUSE,
			aiTextureType_SPECULAR,
			aiTextureType_EMISSIVE,
			aiTextureType_SHININESS,
			aiTextureType_HEIGHT,//should be normal in assimp
			aiTextureType_OPACITY,
			aiTextureType_LIGHTMAP
		};
		for(unsigned t=0;t<sizeof(texType)/sizeof(aiTextureType);++t){//loop over types of textures
			unsigned count=mat->GetTextureCount(TexType[t]);//number of texture per type
			aiString path;//path to texture
			if(AI_SUCCESS==mat->Get(AI_MATKEY_TEXTURE(TexType[t],Count-1),Path)){
				std::string File=Dir+Path.data;//comple path to texture
				std::size_t Where;//position of backslash
				while((Where=File.find("\\"))!=std::string::npos)//are there backslashes
					File.replace(Where,1,"/");//replace backslashes
				int UVIndex;
				if(!(AI_SUCCESS==Material->Get(AI_MATKEY_UVWSRC(TexType[t],Count-1),UVIndex)))
					UVIndex=0;
				if(UVIndex>0)UVIndex--;
				this->TexCoord[t]=UVIndex;
				this->Textures[t]=TextureManager->InsertTexture(File);//load texture
				this->HasTexture[t]=true;//texture of this type is present
				
				//glBindTexture(GL_TEXTURE_2D,this->Textures[t]);//bind texture
				//glGenerateMipmap(GL_TEXTURE_2D);//generate mipmaps
				//glBindTexture(GL_TEXTURE_2D,0);//unbind texture
				int Wrapping;//texture wrapping
				GLint GLWrapping=GL_REPEAT;//opengl texture wrapping
				if(AI_SUCCESS==Material->Get(AI_MATKEY_MAPPINGMODE_U(TexType[t],Count-1),Wrapping)){
					if(Wrapping==aiTextureMapMode_Wrap)//repeat
						GLWrapping=GL_REPEAT;
					if(Wrapping==aiTextureMapMode_Clamp)//clamp
						GLWrapping=GL_CLAMP;
				}

				glGenSamplers(1,this->Samplers+t);//generate sampler
				glSamplerParameteri(this->Samplers[t],GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glSamplerParameteri(this->Samplers[t],GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
				glSamplerParameteri(this->Samplers[t],GL_TEXTURE_WRAP_S,GLWrapping);
				glSamplerParameteri(this->Samplers[t],GL_TEXTURE_WRAP_T,GLWrapping);
			}else{
				this->HasTexture[t]=false;//texture of this type is not present
			}
		}
  }*/
}
