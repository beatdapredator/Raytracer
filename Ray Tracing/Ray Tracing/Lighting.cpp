#include "Lighting.h"
#include <glm/glm.hpp>
#include <algorithm>

Lighting::Lighting(glm::vec3 getDiffuse, glm::vec3 getSpecular, glm::vec3 getlightPosition, glm::vec3 getColour, glm::vec3 getIntensity)
{
	myColour = getColour;
	diffuse = getDiffuse;
	specular = getSpecular;
	lightPosition = getlightPosition;
	intensity = getIntensity;
}