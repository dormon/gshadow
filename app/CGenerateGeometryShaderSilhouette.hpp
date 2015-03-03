#ifndef _CGENERATEGEOMETRYSHADERSILHOUETTE_HPP_
#define _CGENERATEGEOMETRYSHADERSILHOUETTE_HPP_

#include<iostream>


typedef struct{
	unsigned Version;//version of shader
	bool Compatible;//compatible profile
	bool Universal;//is shader universal
	bool Framebuffer;//framebuffer instead of stencil buffer
	bool ImageAtomicAdd;//ImageAtomicAdd
	bool AMDExtension;//
	bool Deterministic;//deterministic shader
	bool OneRepresenatative;//one edge determinism
	bool UseGSInvocation;//toggle usage of geometry shader invocation
	bool ZFail;//zfail algorithm
  bool CullFace;
}SGSSilTemplate;

std::string GenVersion(SGSSilTemplate T);
std::string GenGSSilVertex(unsigned Num,SGSSilTemplate Template);

/**
 * @brief Generate input and output layouts
 *
 * @param Num Number of opposite vertices or max number of opposite vertices
 * @param Caps generate layout for caps shader
 * @param Template template
 *
 * @return returns string representation
 */
std::string GenGSSilGeometryInOut(unsigned Num,bool Caps,SGSSilTemplate Template);
std::string GenGSSilGeometryHull(unsigned Num,SGSSilTemplate Template);
std::string GenGSSilGeometryCap(unsigned Num,SGSSilTemplate Template);
std::string GenGSSilFragment(SGSSilTemplate Template);

std::string GenGSSilGeometryHullGI(unsigned Num,SGSSilTemplate Template);

#endif//_CGENERATEGEOMETRYSHADERSILHOUETTE_HPP_
