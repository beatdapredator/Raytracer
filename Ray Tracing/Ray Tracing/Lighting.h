#pragma once
#include <GLM/glm.hpp>

using namespace glm;

class Lighting
{
public:
	
	Lighting(glm::vec3 getDiffuse, glm::vec3 getSpecular, glm::vec3 getLightPosition, glm::vec3 getColour, glm::vec3 getIntensity);
	~Lighting() {};

	vec3 myColour;
	vec3 diffuse;
	vec3 specular;
	vec3 lightPosition;
	vec3 intensity;




private:


};