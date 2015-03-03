#ifndef _CGENERATESHADERSILHOUETTE_HPP_
#define _CGENERATESHADERSILHOUETTE_HPP_

#include<iostream>
typedef enum{
	SHADER_TYPE_VERTEX,
	SHADER_TYPE_TESSELLATION,
	SHADER_TYPE_GEOMETRY,
	SHADER_TYPE_COMPUTE
}EShaderType;
typedef struct{
	EShaderType Type;///<type of shader
	bool UseLayouts;
	bool Universal;
}SShaderSilhouetteTemplate;

#endif//_CGENERATESHADERSILHOUETTE_HPP_
