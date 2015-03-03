#ifndef _CGENERATEVERTEXSHADERSILHOUETTE_HPP_
#define _CGENERATEVERTEXSHADERSILHOUETTE_HPP_

#include<iostream>

/**
 * @brief Struct configurates generation of vertex shader for silhouettes
 */
typedef struct{
	unsigned Version;///<version of shader
	bool UseLayouts;///<toggle usage of layouts in shader
	bool UseVertexAttribArray;///<simplify vertex attrib specification
	bool Universal;///<universal vertex shader silhouette
	bool UseCompatibility;///<toggle usage of compatibiliti profile
	std::string LightPositionUniformName;///<name of LightPosition uniform
	std::string MatrixUniformName;///<name of matrix uniform
	std::string VertexAttribName;///<name of input vertex attribs
}SVSSilTemplate;

/**
 * @brief Function generates vertex shader silhouette code for hulls
 *
 * @param Num number of opposite vertices
 * @param Template template of shader
 *
 * @return Function returns string with shader
 */
std::string GenVSSilVertexHull(unsigned Num,SVSSilTemplate Template);

/**
 * @brief Function generates fragment shader silhoette code for hulls
 *
 * @return Funcion return string with shader
 */
std::string GenVSSilFragmentHull(SVSSilTemplate Template);

/**
 * @brief Function generates vertex shader silhouette code for caps
 *
 * @param Template templte of shader
 *
 * @return Function returns string with shader
 */
std::string GenVSSilVertexCap(SVSSilTemplate Template);

/**
 * @brief Function generates fragment shader silhoette code for caps
 *
 * @return Funcion return string with shader
 */
std::string GenVSSilFragmentCap(SVSSilTemplate Template);

#endif//_CGENERATEVERTEXSHADERSILHOUETTE_HPP_
