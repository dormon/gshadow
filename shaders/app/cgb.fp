#version 440 core

layout(location=0)out uvec4 fColor;
layout(location=1)out vec3  fPosition;
layout(location=2)out vec4  fNormal;

in gl_PerVertex{
  vec3 vPosition ;
  vec3 vNormal   ;
  vec2 vCoord    ;
  vec3 vTangent  ;
  vec3 vBitangent;
  flat sampler2D vTextures[MAX_TEXTURES_PER_MESH];
  flat uint vActiveTextures[NOF_ACTIVE_TEXTURES_UINTS];
};

#define        DIFFUSE_TEXTURE 0
#define       SPECULAR_TEXTURE 1
#define         NORMAL_TEXTURE 2
#define SPECULARFACTOR_TEXTURE 3
#define        OPACITY_TEXTURE 4
#define       LIGHTMAP_TEXTURE 5

#define isTextureActive(y,x)\
  (bool((y>>x)&1))


vec3 Ka=vec3(.5);

void main(){
  vec3  diffuseColor   = vec3(.5);
  vec3  specularColor  = vec3(1);
  vec3  normal         = gNormal;
  vec3  lightmap       = vec3(1);
  float specularFactor = 1;

  if(isTextureActive(gActiveTexture,OPACITY_TEXTURE))
    if(texture(gOpacityTexture,gCoord).x==0)
      discard;
  if(isTextureActive(gActiveTexture,DIFFUSE_TEXTURE))
    diffuseColor  = texture(gDiffuseTexture,gCoord).xyz;
  if(isTextureActive(gActiveTexture,SPECULAR_TEXTURE))
    specularColor = texture(gSpecularTexture,gCoord).xyz;
  if(isTextureActive(gActiveTexture,NORMAL_TEXTURE)){
    mat3 M;
    M[0]   = normalize(gTangent  );
    M[1]   = normalize(gBitangent);
    M[2]   = normalize(gNormal   );
    normal = normalize(M*normalize(texture(gNormalTexture,gCoord).xyz));
  }
  if(isTextureActive(gActiveTexture,LIGHTMAP_TEXTURE))
    lightmap = texture(gLightmapTexture,gCoord).x;
  if(isTextureActive(gActiveTexture,SPECULARFACTOR_TEXTURE))
    specularFactor = texture(gSpecularFactorTexture,gCoord).x;
    
  uvec4 color  = uvec4(0);
  color.xyz   += uvec3(diffuseColor*lightmap*0xff);
  color.xyz   += uvec3(specularColor*lightmap*0xff)<<8;
  color.w      = uint (specularFactor*0xff);

	fColor    = color;
	fPosition = gPosition;
	fNormal   = vec4(normal,-dot(gPosition,normal));

}
