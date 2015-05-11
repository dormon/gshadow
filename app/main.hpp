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

#define GLM_FORCE_RADIANS
#include<glm/glm.hpp>
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

#include<geGL/geGL.h>
#include<geGL/UniformCommands.h>
#include<geGL/ProgramCommands.h>

#include<geUtil/WindowObject.h>
#include<geUtil/ArgumentObject.h>
#include<geUtil/FPSPrinter.h>
#include"../GPUPerfApi/GpuPerfApi.h"

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


#include"../methods/ComputeGeometry/ComputeGeometry.h"
#include"../methods/RTW/RTW.h"

#include"AssimpLoader.h"
#include"Scene.h"

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

#include"testConvexHull.h"
#include"../objectConfiguration/lightConfiguration.h"
#include"../objectConfiguration/cameraConfiguration.h"
#include"../objectConfiguration/cameraPathConfiguration.h"
#include"../objectConfiguration/SimDataConfiguration.h"



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


aiScene*SceneModel;


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
