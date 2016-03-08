#ifndef _CSINTORN_HPP_
#define _CSINTORN_HPP_

#if 0

#include<GL/glew.h>
#include<stdarg.h>
#include<math.h>

#include<geGL/BufferObject.h>
#include<geGL/VertexArrayObject.h>

#include<glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>


#include"sintorn_tiles.hpp"

#include"../../simulationData/SimulationData.h"

#include"shaderdir.hpp"

class CSintorn
{
	public:
		unsigned WindowSize[2];///window size
		unsigned NumLevels;///number of level
    bool     UseUniformTileSizeInClipSpace;
    bool     UseUniformTileDivisibility;
		//GLuint*HDB;///hierarchical depth textures
		GLuint*FBO;///framebuffers for generation of HDB
    GLuint HDBTexture;
    GLuint HSBTexture;
    GLuint FinalStencilMask;
    unsigned ShadowFrustaPerWorkGroup;
		ge::gl::BufferObject*ShadowFrusta;
		ge::gl::BufferObject*HSB;
		ge::gl::BufferObject*HDB;

    GLuint*HDT;
    GLuint*HST;

    GLuint HDBImage;
    GLuint HSBImage;

		unsigned WavefrontSize;
		unsigned*TileCount;///size of texture in each level
		unsigned*HDBOffset;///offset of depth texture in each level
		unsigned*TileSizeInPixels;///size of tiles in pixels in each level
		unsigned*TileDivisibility;
		float*TileSizeInClipSpace;

		ge::gl::ProgramObject*WriteDepthProgram;
    ge::gl::ProgramObject*WriteDepthTextureProgram;
		ge::gl::ProgramObject*HierarchicalDepthProgram;
    ge::gl::ProgramObject*HierarchicalDepthTextureProgram;
		ge::gl::ProgramObject*ShadowFrustumProgram;
		ge::gl::ProgramObject*RasterizeProgram;
    ge::gl::ProgramObject*RasterizeTextureProgram;
    ge::gl::ProgramObject*MergeProgram;
    ge::gl::ProgramObject*MergeTextureProgram;
    ge::gl::ProgramObject*WriteStencilProgram;
    ge::gl::ProgramObject*WriteStencilTextureProgram;
    ge::gl::ProgramObject*ClearStencilProgram;

		ge::gl::ProgramObject*HDBProgram;
		ge::gl::ProgramObject*SFProgram;

    ge::gl::ProgramObject*DrawHDBProgram;
    ge::gl::ProgramObject*DrawHSBProgram;

    ge::gl::ProgramObject*CopyHDBProgram;
    ge::gl::ProgramObject*CopyHSBProgram;

    ge::gl::VertexArrayObject*EmptyVAO;
		/*GLint WDWGS[3];
		GLint HDBWGS[3];
		GLint SFWGS[3];
		GLint RWGS[3];*/
		//GLuint ShadowFrusta;
    SAdjecency*Adjacency;
    ge::gl::BufferObject*Triangles;
		unsigned GetTileSize(unsigned Level,unsigned Axis);
		unsigned GetTileCount(unsigned Level,unsigned Axis);
		std::string GenerateRasterizeProgram(
				unsigned NumLevels,
				unsigned WaveforntSize,
				unsigned ShadowFrustaPerWorkGroup);
		std::string GenerateWriteDepth(int*TileSize);
	public:
		CSintorn(
				unsigned*WindowSize,
				SAdjecency*Adjacency,
				unsigned WavefrontSize,
				unsigned ShadowFrustaPerWorkGroup);
		//CSintorn(unsigned Width,unsigned Height,unsigned WavefrontSize,unsigned NumTriangles);
		CSintorn(unsigned Width,unsigned Height,unsigned NumTriangles,unsigned NumLevels,...);
    ~CSintorn();
		void GenerateHierarchy(GLuint DepthTexture,GLuint NormalTexture,float*LightPosition);
    void GenerateHierarchyTexture(GLuint DepthTexture,GLuint NormalTexture,float*LightPosition);
		void ComputeShadowFrusta(float*LP,glm::mat4 mvp);
		void Rasterize();
    void RasterizeTexture();
    void Merge();
    void MergeTexture();
    void CopyHDB();
    void CopyHSB();
    void DrawHDB(unsigned Level,unsigned DrawNear);
    void DrawHSB(unsigned Level);
};

#endif

#endif//_CSINTORN_HPP_
