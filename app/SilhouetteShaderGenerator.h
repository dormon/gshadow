/**
 * @file
 * SilhouetteShaderGenerator header.
 *
 * @author Dormon (Tomáš Milet)
 */

#ifndef _SILHOUETTESHADERGENERATOR_H_
#define _SILHOUETTESHADERGENERATOR_H_

#include<iostream>

/**
 * @brief Type of shader profile
 */
enum EShaderProfile{
  NOTHING,///< not specified
  COMPATIBILITY,///< compatibility profile
  CORE,///< core profile
  ES///< embeded systems profile
};

/**
 * @brief Translates enum to string
 *
 * @param Profile shader profile
 *
 * @return string representation of shader profile
 */
std::string TranslateShaderProfile(enum EShaderProfile Profile);

/**
 * @brief Silhouette Vertex Shader Program
 */
namespace NVertexSilhouetteShader{
  //TODO
}

/**
 * @brief Silhouette Geometry Shader Program
 */
namespace NGeometrySilhouetteShader{
  /**
   * @brief Template for Geometry Shader generation
   */
  typedef struct{
    bool Deterministic;///< deterministic version
    bool ReferenceEdge;///< Reference edge version
    bool Universal;///< universal shader
    bool UseVS2GSArray;///< toggle usage of array as way of transfer
    bool UseVertexArray;///< inputs into vertex shader are in array
    bool UseLayouts;///< inputs have layout interface
    bool UseStencilValueExport;///< toggle usage of AMD extension
    bool CCWIsFrontFace;///< Front Face is CCW
    bool FrontFaceInside;///< Front Face are oriented inside Shadow Volume
    bool CullSides;///< toggle culling of sides
    bool Visualize;///< visualize SV with colors
    bool FrontCapToInfinity;///< shift front cap to infinity
    bool GenerateSides;///< toggle generation of sides
    bool GenerateCaps;///< toggle generation of caps
    std::string Matrix;///< mvp matrix uniform name
    unsigned Version;///< version of shader
    enum EShaderProfile Profile;///< profile version
    unsigned MaxMultiplicity;///< maximal number of opposite vertices
    std::string LightUniform;///< light position uniform name
    std::string VertexAttribName;///< name of vertex attribs
  }STemplate;

  /**
   * @brief Gets vertex shader
   *
   * @param Template template
   *
   * @return string representation of shader
   */
  std::string getVertex  (STemplate Template);

  /**
   * @brief Gets geometry shader
   *
   * @param Template template
   *
   * @return string representation of shader
   */
  std::string getGeometry(STemplate Template);

  /**
   * @brief Gets fragment shader
   *
   * @param Template template
   *
   * @return string representation of shader
   */
  std::string getFragment(STemplate Template);

  /**
   * @brief Generates template from number
   *
   * @param Number number 
   *
   * @return template
   */
  STemplate getTemplate(unsigned Number);
}

/**
 * @brief Tessellation Silhouette Shader Program
 */
namespace NTessellationSilhouetteShader{
  //TODO
}

/**
 * @brief Compute Silhouette Shader Program
 */
namespace NComputeSilhouetteShader{
  //TODO
}

#endif//_SILHOUETTESHADERGENERATOR_H_
