#ifndef _MAIN_HPP_
#define _MAIN_HPP_


#include<iostream>
#include<fstream>
#include<istream>
#include<sstream>
using namespace std;


//#include<glbinding/gl/gl.h>
//#include<glbinding/Binding.h>
#include<GL/glew.h>
//#include<GL/glext.h>
//#include<GL/gl.h>
#include<SDL2/SDL.h>

#include<glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>

#include<assimp/cimport.h>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
/*
#include<opencv/cv.h>
#include<opencv/highgui.h>
*/

//#include"light.h"
//#include"terrain.hpp"
//#include"window.hpp"
#include"deferred.hpp"

//#include"CLight.hpp"

#include<geGL/BufferObject.h>
#include<geGL/ProgramObject.h>
#include<geGL/VertexArrayObject.h>
#include<geGL/AsynchronousQueryObject.h>
#include<geGL/DebugMessage.h>

#include<geUtil/WindowObject.h>
#include<geUtil/ArgumentObject.h>
#include"../GPUPerfApi/CGpuPerfApi.hpp"

#include"CGenerateGeometryShaderSilhouette.hpp"
#include"CGenerateVertexShaderSilhouette.hpp"
#include"CGenerateTessellationShaderSilhouette.hpp"

#include"methodsparam.hpp"
#include"../methods/TessellationSides/CTessellationSides.hpp"
#include"../methods/GeometryCaps/CGeometryCaps.hpp"
#include"../methods/GeometrySides/CGeometrySides.hpp"
#include"../methods/GeometryCapsAlt/CGeometryCapsAlt.hpp"
#include"../methods/GeometrySidesCaps/CGeometrySidesCaps.hpp"

#include"../methods/ComputeSidesSOEPlane/CComputeSidesSOEPlane.hpp"
#include"../methods/ComputeSidesSOE/CComputeSidesSOE.hpp"
#include"../methods/ComputeSides/CComputeSides.hpp"
#include"../methods/VertexSides/CVertexSides.hpp"
#include"../methods/VertexCaps/CVertexCaps.hpp"
#include"../methods/Sintorn/CSintorn.hpp"
#include"../methods/ShadowMapping/CShadowMapping.hpp"
#include"../methods/NavyMapping/NavyMapping.h"
#include"../methods/CubeShadowMapping/CubeShadowMapping.h"



#include<geUtil/CameraObject.h>
#include<geUtil/CameraPath.h>

#include"shaderdir.hpp"

#include"../drawPrimitive/drawPrimitive.h"

#include<geGL/ProgramPipelineObject.h>

#include"SilhouetteShaderGenerator.h"

#include<AntTweakBar.h>

#include"adjacency.hpp"

#include"init_shadowmapping.hpp"
#include"BoundingBox.h"

#include"../geometry/Geometry.h"
#include"../simulationData/SimulationData.h"
#include"../argumentLoader/ArgumentLoader.h"

#define ILUT_USE_OPENGL
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>


#define DEBUG
#ifdef DEBUG
#define glError() {\
	GLenum err=glGetError();\
	while(err!=GL_NO_ERROR){\
		fprintf(stderr,"glError: %s caught at %s:%u\n", (char *)gluErrorString(err), __FILE__, __LINE__); \
		err=glGetError();\
	}\
}
#else
#define glError() 
#endif

/**
 * @brief This enumerate represent selected scene
 */
typedef enum{
	EScene_Terrain=0,//terrain
	EScene_City,//city
	EScene_Cathedral//cathedral
}EScene;

/**
 * @brief This enumerate represent selected method
 */
typedef enum{
	EMethod_Base=0,//without deferred shading endless light contribution
	EMethod_BaseAttenuation,//without deferred shading
	EMethod_Scissor,//every light separate
	EMethod_Grid,//our solution - viewspace 
	EMethod_GridScreenSpace,//out solution - screenspace
	EMethod_GeometryShaderSparse,//sorting with geometry shader to sparse textures
	EMethod_GeometryShader//sorting with geomery shader
}EMethod;

/**
 * @brief This enumerate represent selected light placement
 */
typedef enum{
	ELightPlacement_Random=0,//random light placement
	ELightPlacement_City,//city light placement
	ELightPlacement_Cathedral//cathedral light placement
}ELightPlacement;

EScene Scene=EScene_City;//EScene_Terrain;//Scene
EMethod Method=EMethod_Grid; //EMethod_Base;//method
ELightPlacement LightPlacement=ELightPlacement_Random;//light placement
float LightSize=5*4;
aiScene*SceneModel;


void InitLightTexture(unsigned NumLights,unsigned NumSegments);
void CityGenerate();
void CityRegenerateLights(float Spacing,float Range,float Color);
//void InitScreenParam(SScreenParam*Param,float Fovy,float Near,float Far,unsigned Width,unsigned Height);
void InitModel(const char*File);

void DrawQuad(unsigned*X,unsigned*S);

void DrawScene();
void DrawShadow();
void Draw();
/*
IplImage*TakeGroundTruth();
IplImage*ComputeDifference(IplImage*A,IplImage*B);
*/
#endif//_MAIN_HPP_
