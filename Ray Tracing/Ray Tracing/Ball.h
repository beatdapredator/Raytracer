#pragma once
#include <GLM/glm.hpp>

using namespace glm;

class Ball
{
public:
	Ball(glm::vec3, glm::vec3, float);
	float getRadius();
	vec3 getCentre();
	vec3 getColour();
	bool intersect(vec3 origin, vec3 dir, float &t, vec3 &intPoint);


private:
	float radius;
	vec3 centre;
	vec3 colour;

};

