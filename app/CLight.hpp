#ifndef _CLIGHT_HPP_
#define _CLIGHT_HPP_

#include<iostream>
#include<vector>
#include<AntTweakBar.h>

#define GLM_FORCE_RADIANS
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>

class CLight
{
	public:
    glm::vec4 position;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

class COmnidirectionLight: public CLight
{
	public:
		float R;
};

class CLightManager
{
	public:
		std::vector<CLight*>Lights;
		void CreateBar(TwBar*Bar);
};

#endif//_CLIGHT_HPP_
