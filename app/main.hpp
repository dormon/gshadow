#ifndef _MAIN_HPP_
#define _MAIN_HPP_


#include<iostream>
#include<fstream>
#include<istream>
#include<sstream>

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

#include"deferred.hpp"

//#include"CLight.hpp"

#include<geGL/geGL.h>
#include<geCore/geCore.h>

#include<geAd/WindowObject/WindowObject.h>
#include<geUtil/ArgumentObject.h>
#include<geUtil/FPSPrinter.h>
#include"../GPUPerfApi/GpuPerfApi.h"

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
#include"../methods/CubeNavyMapping/CubeNavyMapping.h"
#include"../methods/RayTrace/RayTrace.h"
#include"../methods/DualParaboloid/DualParaboloid.h"
#include"../methods/NavyDualParaboloid/NavyDualParaboloid.h"


#include"../methods/ComputeGeometry/ComputeGeometry.h"
#include"../methods/DormonShadowTest0/dormonShadowTest0.h"
#include"../methods/RTW/RTW.h"

#include"copyTex.h"

#include"AssimpLoader.h"
#include"Scene.h"

#include<geUtil/CameraObject.h>
#include<geUtil/CameraPath.h>

#include"shaderdir.hpp"

#include"../drawPrimitive/drawPrimitive.h"

#include<geGL/ProgramPipelineObject.h>

#include"SilhouetteShaderGenerator.h"

#include<AntTweakBar.h>

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

#include"../geometry/geom/ConvexHull.h"

#include"simpleLoader.h"
#include"ModelPN2VAO.h"
#include"Model2AABB.h"
#include"Model2DIBO.h"

#include<geUtil/NamespaceWithUsers.h>
#include<geUtil/copyArgumentManager2Namespace.h>
#include<geUtil/ArgumentManager/ArgumentManager.h>

#include"../objectConfiguration/Namespace2AntTweakbar.h"
#include"core.h"

#define ILUT_USE_OPENGL
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include<geCore/interpret.h>
#include<geCore/stdFunctions.h>

#endif//_MAIN_HPP_
