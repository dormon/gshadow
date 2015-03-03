#version 440 core

#ifndef MAX_TEXTURES
#define MAX_TEXTURES 256
#endif//MAX_TEXTURES

#ifndef UINT_BIT_SIZE
#define UINT_BIT_SIZE 32
#endif//UINT_BIT_SIZE

#ifndef MAX_TEXTURES_PER_MESH
#define MAX_TEXTURES_PER_MESH 6
#endif//MAX_TEXTURES_PER_MESH


#define BITS_PER_TEXTURE_ID  (uint(ceil(log(MAX_TEXTURES)/log(2))))
#define TEXTURE_IDS_PER_UINT (UINT_BIT_SIZE/BITS_PER_TEXTURE_ID)
#define TEXTURE_ID_MASK      ((1<<BITS_PER_TEXTURE_ID)-1)

#define UINT_TEXURE_ID_PER_MESH\
  (uint(ceil(float(MAX_TEXTURES_PER_MESH*BITS_PER_TEXTURE_ID)/UINT_BIT_SIZE)))


#define GET_TEXTURE_INDEX(y,x)\
  ((y[x/TEXTURE_IDS_PER_UINT]>>(BITS_PER_TEXTURE_ID*(x%TEXTURE_IDS_PER_UINT)))&TEXTURE_ID_MASK)


layout(triangles)in;
layout(triangle_strip,max_vertices=6)out;

in vec3 vPosition [];
in vec3 vNormal   [];
in vec2 vCoord    [];
in vec3 vTangent  [];
in vec3 vBitangent[];

out vec3 gPosition ;
out vec3 gNormal   ;
out vec2 gCoord    ;
out vec3 gTangent  ;
out vec3 gBitangent;

struct MeshData{
  mat4 modelMatrix;
  uint textureId[UINT_TEXURE_ID_PER_MESH];
  uint activeTexture;
};

layout(std430,binding=CGB_BINDING_MESH_DATA)buffer Mesh{MeshData mesh[];};

layout(bindless_sampler)uniform sampler2D textures[MAX_TEXTURES];

flat out sampler2D gDiffuseTexture;
flat out sampler2D gSpecularTexture;
flat out sampler2D gNormalTexture;
flat out sampler2D gSpecularFactorTexture;
flat out sampler2D gOpacityTexture;
flat out sampler2D gLightmapTexture;
flat out uint      gActiveTexture;


flat in uint drawId;

uniform mat4 v,p;

#define        DIFFUSE_TEXTURE 0
#define       SPECULAR_TEXTURE 1
#define         NORMAL_TEXTURE 2
#define SPECULARFACTOR_TEXTURE 3
#define        OPACITY_TEXTURE 4
#define       LIGHTMAP_TEXTURE 5

void main(){
  MeshData meshData      = mesh[drawId];
  gActiveTexture         = meshData.activeTexture;
  gDiffuseTexture        = textures[GET_TEXTURE(meshData.textureId,       DIFFUSE_TEXTURE)];
  gSpecularTexture       = textures[GET_TEXTURE(meshData.textureId,      SPECULAR_TEXTURE)];
  gNormalTexture         = textures[GET_TEXTURE(meshData.textureId,        NORMAL_TEXTURE)];
  gSpecularFactorTexture = textures[GET_TEXTURE(meshData.textureId,SPECULARFACTOR_TEXTURE)];
  gOpacityTexture        = textures[GET_TEXTURE(meshData.textureId,       OPACITY_TEXTURE)];
  gLightmapTexture       = textures[GET_TEXTURE(meshData.textureId,      LIGHTMAP_TEXTURE)];

	for(int i=2;i>=0;--i){
		gl_Position = p*v*meshData.modelMatrix*vec4(vPosition[i],1);;
		gPosition   = vPosition [i];
		gNormal     = (meshData.modelMatrix*vec4(vNormal   [i],0)).xyz;
    gCoord      = vCoord    [i];
    gTangent    = (meshData.modelMatrix*vec4(vTangent  [i],0)).xyz;
    gBitangent  = (meshData.modelMatrix*vec4(vBitangent[i],0)).xyz;
		EmitVertex();
	}
	EndPrimitive();
	for(int i=0;i<3;++i){
		gl_Position = p*v*meshData.modelMatrix*vec4(vPosition[i],1);;
		gPosition   = vPosition [i];
		gNormal     = (meshData.modelMatrix*vec4(vNormal   [i],0)).xyz;
    gCoord      = vCoord    [i];
    gTangent    = (meshData.modelMatrix*vec4(vTangent  [i],0)).xyz;
    gBitangent  = (meshData.modelMatrix*vec4(vBitangent[i],0)).xyz;
    EmitVertex();
	}
	EndPrimitive();
}


