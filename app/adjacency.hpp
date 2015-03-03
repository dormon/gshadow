#ifndef _ADJACENCY_HPP_
#define _ADJACENCY_HPP_

#include<GL/glew.h>
#include<geGL/ProgramObject.h>
#include"../app/SilhouetteShaderGenerator.h"

typedef struct{
	float*Points;
	unsigned NumPoints;
	unsigned*Triangles;
	unsigned NumTriangles;
	unsigned*PointTriangleNum;
	unsigned**PointTriangles;
	unsigned*PointEdgeNum;
	unsigned**PointEdge;
	unsigned**EdgeIndex;
	unsigned**Edges;
	unsigned NumEdges;
	unsigned*EdgeOppositeNum;
	unsigned**EdgeOpposite;
	unsigned**TriangleEdge;
  unsigned*ClassCard;
  unsigned MaxOpposite;
}SAdjecency;

typedef struct{
	GLuint VAO;///vertex array object
	GLuint EBO;///element array object
	unsigned N;/// number of triangles connected to edge
}SEdgePointVAO;

typedef struct{
	GLuint VBOCascade[15];
	GLuint VAOCascade[15];
	unsigned StartCascade[15];
	unsigned LenCascade[15];
	GLuint VBOUniversal;
	GLuint VAOUniversal;
	unsigned StartUniversal;
	unsigned LenUniversal;
}SEdgePointCascade;

typedef struct{
	GLuint VBO;///vertex buffer object
	SEdgePointCascade Cascade;
	SEdgePointVAO*NEdge;
	unsigned MaxN;
	SEdgePointVAO Universal;
}SSpecificEdge;

class CShadowShader
{
	private:
		ge::gl::ProgramObject*SpecificShader[14];
		ge::gl::ProgramObject*UniversalShader;
		ge::gl::ProgramObject*SpecificShaderMulti[14];
		ge::gl::ProgramObject*UniversalShaderMulti;

		ge::gl::ProgramObject*SpecificShaderDeterministic[14];
		ge::gl::ProgramObject*UniversalShaderDeterministic;

		ge::gl::ProgramObject*SpecificStencilNondeterministicZpassShader[14];
		ge::gl::ProgramObject*UniversalStencilNondeterministicZpassShader;

		ge::gl::ProgramObject*SpecStencilNonDetHull[14];
		ge::gl::ProgramObject*SpecStencilNonDetCap[14];
		ge::gl::ProgramObject*UniStencilNonDetHull;
		ge::gl::ProgramObject*UniStencilNonDetCap;

		ge::gl::ProgramObject*SpecificFramebufferNondeterministicZpassShader[14];
		ge::gl::ProgramObject*UniversalFramebufferNondeterministicZpassShader;

		ge::gl::ProgramObject*SpecFrameNonDetHull[14];
		ge::gl::ProgramObject*SpecFrameNonDetCap[14];
		ge::gl::ProgramObject*UniFrameNonDetHull;
		ge::gl::ProgramObject*UniFrameNonDetCap;

		ge::gl::ProgramObject*SpecificStencilDeterministicZpassShader[14];
		ge::gl::ProgramObject*UniversalStencilDeterministicZpassShader;

		ge::gl::ProgramObject*SpecStencilDetHull[14];
		ge::gl::ProgramObject*SpecStencilDetCap[14];
		ge::gl::ProgramObject*UniStencilDetHull;
		ge::gl::ProgramObject*UniStencilDetCap;

		ge::gl::ProgramObject*SpecStencilDetHullGI[14];
		ge::gl::ProgramObject*UniStencilDetHullGI;

		ge::gl::ProgramObject*SpecificFramebufferDeterministicZpassShader[14];
		ge::gl::ProgramObject*UniversalFramebufferDeterministicZpassShader;

		ge::gl::ProgramObject*SpecFrameDetHull[14];
		ge::gl::ProgramObject*SpecFrameDetCap[14];
		ge::gl::ProgramObject*UniFrameDetHull;
		ge::gl::ProgramObject*UniFrameDetCap;

		ge::gl::ProgramObject*SpecificStencilNondeterministicZfailShader[14];
		ge::gl::ProgramObject*UniversalStencilNondeterministicZfailShader;

		ge::gl::ProgramObject*SpecificFramebufferNondeterministicZfailShader[14];
		ge::gl::ProgramObject*UniversalFramebufferNondeterministicZfailShader;

		ge::gl::ProgramObject*SpecificStencilDeterministicZfailShader[14];
		ge::gl::ProgramObject*UniversalStencilDeterministicZfailShader;

		ge::gl::ProgramObject*SpecificFramebufferDeterministicZfailShader[14];
		ge::gl::ProgramObject*UniversalFramebufferDeterministicZfailShader;

		//Atomic
		ge::gl::ProgramObject*SpecFrameAtomicNonDetHull[14];
		ge::gl::ProgramObject*SpecFrameAtomicNonDetCap[14];
		ge::gl::ProgramObject*UniFrameAtomicNonDetHull;
		ge::gl::ProgramObject*UniFrameAtomicNonDetCap;

		ge::gl::ProgramObject*SpecFrameAtomicDetHull[14];
		ge::gl::ProgramObject*SpecFrameAtomicDetCap[14];
		ge::gl::ProgramObject*UniFrameAtomicDetHull;
		ge::gl::ProgramObject*UniFrameAtomicDetCap;

		//AMDExtension
		ge::gl::ProgramObject*SpecAMDExtensionNonDetHull[14];
		ge::gl::ProgramObject*SpecAMDExtensionNonDetCap[14];
		ge::gl::ProgramObject* UniAMDExtensionNonDetHull;
		ge::gl::ProgramObject* UniAMDExtensionNonDetCap;

		ge::gl::ProgramObject*SpecAMDExtensionDetHull[14];
		ge::gl::ProgramObject*SpecAMDExtensionDetCap[14];
		ge::gl::ProgramObject* UniAMDExtensionDetHull;
		ge::gl::ProgramObject* UniAMDExtensionDetCap;


		void LoadShaders();
		void SetUniforms(ge::gl::ProgramObject*Prg,float*M,float*V,float*P,float*L);
		void Draw(GLuint VAO,unsigned Num);
		void DrawUniversal(ge::gl::ProgramObject*Prg,float*M,float*V,float*P,float*L);
		void DrawSpecific(ge::gl::ProgramObject**Prg,float*M,float*V,float*P,float*L);
		SSpecificEdge SpecificEdge;
		void ComputeSpecificEdges(
				SSpecificEdge*SpecificEdge,
				SAdjecency*Adjacency,
				unsigned MinNum);
	public:
		void DrawShadowVolume(float*M,float*V,float*P,float*LightPos);
		void DrawShadowVolumeDeterministic(float*M,float*V,float*P,float*LightPos);
		void DrawShadowVolumeMulti(float*M,float*V,float*P,float*LightPos);
		void DrawShadowVolumeStencilNondeterministicZpassShader(
				float*M,float*V,float*P,float*L);
		void DrawShadowVolumeFramebufferNondeterministicZpassShader(
				float*M,float*V,float*P,float*L);
		void DrawShadowVolumeStencilDeterministicZpassShader(
				float*M,float*V,float*P,float*L);
		void DrawShadowVolumeFramebufferDeterministicZpassShader(
				float*M,float*V,float*P,float*L);
		void DrawShadowVolumeStencilNondeterministicZfailShader(
				float*M,float*V,float*P,float*L);
		void DrawShadowVolumeFramebufferNondeterministicZfailShader(
				float*M,float*V,float*P,float*L);
		void DrawShadowVolumeStencilDeterministicZfailShader(
				float*M,float*V,float*P,float*L);
		void DrawShadowVolumeFramebufferDeterministicZfailShader(
				float*M,float*V,float*P,float*L);

		void DrawShadowVolumeFramebufferAtomicNondeterministicZpassShader(
				float*M,float*V,float*P,float*L);
		void DrawShadowVolumeFramebufferAtomicDeterministicZpassShader(
				float*M,float*V,float*P,float*L);
		void DrawShadowVolumeFramebufferAtomicNondeterministicZfailShader(
				float*M,float*V,float*P,float*L);
		void DrawShadowVolumeFramebufferAtomicDeterministicZfailShader(
				float*M,float*V,float*P,float*L);
		void DrawShadowVolumeAMDExtensionDeterministicZfailShader(
				float*M,float*V,float*P,float*L);

		void DrawShadowVolumeGI(
				float*M,float*V,float*P,float*L);

		CShadowShader(SAdjecency*Adjacency,unsigned MinNum);
		~CShadowShader();
	
};


void ComputeAdjacency(SAdjecency*Adjacency,float*Points,unsigned NumTriangles);
void FreeAdjacency(SAdjecency*Adjacency);


void SortTriangles(
		float**PointsOut,
		unsigned*NumPointsOut,
		unsigned**TrianglesOut,
		unsigned*NumTrianglesOut,
		float*TrianglesIn,
		unsigned NumTrianglesIn);

void ComputePointTriangleAdjacency(
		unsigned**PointTriangleNum,
		unsigned***PointTriangle,
		unsigned NumPoints,
		unsigned*Triangles,
		unsigned NumTriangles);

void ComputePointEdgeAdjacency(
		unsigned**PointEdgeNum,
		unsigned***PointEdge,
		unsigned*PointTriangleNum,
		unsigned**PointTriangle,
		unsigned NumPoints,
		unsigned*Triangles,
		unsigned NumTriangles);

void ComputeEdge(
		unsigned***Edge,
		unsigned*NumEdge,
		unsigned*PointEdgeNum,
		unsigned**PointEdge,
		unsigned NumPoints);

void ComputeEdgeIndex(
		unsigned***EdgeIndex,
		unsigned NumEdge,
		unsigned*PointEdgeNum,
		unsigned**PointEdge,
		unsigned NumPoints);

void ComputeOppositeNum(
		unsigned**EdgeOppositeNum,
		unsigned NumEdge,
		unsigned*PointEdgeNum,
		unsigned**PointEdge,
		unsigned**EdgeIndex,
		unsigned*Triangles,
		unsigned NumTriangles);

void ComputeOpposite(
		unsigned***EdgeOpposite,
		unsigned**EdgeIndex,
		unsigned*EdgeOppositeNum,
		unsigned EdgeNum,
		unsigned**PointEdge,
		unsigned*PointEdgeNum,
		unsigned*Triangles,
		unsigned NumTriangles);

#endif//_ADJACENCY_HPP_
