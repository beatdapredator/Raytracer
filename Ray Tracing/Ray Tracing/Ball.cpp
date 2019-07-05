#include "Ball.h"
#include <glm/glm.hpp>
#include <algorithm>


Ball::Ball(glm::vec3 objCentre, glm::vec3 objColour, float objRadius) 
{
	centre = objCentre;
	colour = objColour;
	radius = objRadius;
}

float Ball::getRadius()
{ 
	return radius;
}

vec3 Ball::getCentre()
{
	return centre;
}

vec3 Ball::getColour()
{
	return colour;
}


bool Ball::intersect(vec3 origin, vec3 dir,  float &t,vec3 &intPoint)
{
	float hitDistance, rayRightangle;
	vec3 intersection = centre - origin;
	hitDistance = dot(intersection, dir);

	if (hitDistance < 0) 
	{
		return false;
	}

	rayRightangle = dot(intersection, intersection) - hitDistance*hitDistance;

	if (rayRightangle > radius * radius) 
	{
		return false;
	}

	 
	float tna = sqrt((radius*radius) - rayRightangle);

	float t0 = hitDistance - tna;  
	float t1 = hitDistance + tna;

	// set the first point of intersection as lowest value
	if (t0>t1)
	{
		std::swap(t0, t1);
	}

	// if t0 is a negative number then use t1 as the first intersection
	if (t0 < 0)
	{
		t0 = t1;
		if (t0 < 0)
		{
			return false;// this would mean that both the sections are negative and there is no intersections.
		}
	}


	t = t0;
	intPoint.x = dir.x*t; intPoint.y = dir.y*t; intPoint.z = dir.z*t;

	return true;
} 