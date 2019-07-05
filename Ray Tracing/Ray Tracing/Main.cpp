///////////////////////////////////////////////////////////////////////////////////////////////////
//This document has been created by Matthew Dunphy
//I am using to create raytracing and see if I can make it so that I can see objects in a scene 
//Version 0.0.1
//////////////////////////////////////////////////////////////////////////////////////////////////
#define SDL_MAIN_HANDLED
#include <iostream>
#include <glm/glm.hpp>
#include <cmath>
#include <fstream>
#include <algorithm>
#include "Ball.h"
#include "Lighting.h"
#include <vector>
#include "SDL.h"

#define pi 3.14159
using namespace std;
using namespace glm;

// set a smaller aspect ratio for the scene so that there is less for the computer to compute and reduce the risk of the computer crashing
const int Width = 640;
const int Height = 480;

//these are the elements of my display and when they come together they create an array for my displa
float Aspectratio, PixelNdx, PixelNdy, PixelRdx, PixelRdy, tanvalue, Pcamex, Pcamey;

SDL_Event event = { 0 };

Ball ball1(vec3(0.0, 0.0, -20), vec3(1.0, 0.32, 0.36), 4.0);
Ball ball2(vec3(5.0, -1.0, -15), vec3(0.9, 0.76, 0.46), 2.0);
Ball ball3(vec3(5.0, 0.0, -25), vec3(0.65, 0.77, 0.97), 3.0);
Ball ball4(vec3(-5.5, 0.0, -15), vec3(0.5, 0.3, 0.7), 3.0);
Ball ball5(vec3(0.0, -10004.0, -20), vec3(1, 0, 1), 10000.0);
vector <Ball> ballList = { ball1, ball2, ball3, ball4, ball5 };

// lighting(vec3(0.7, 0.7, 0.7), vec3(0.3, 0.3, 0.3), vec3(0.0, 20.0, -2), vec3(1.0, 1.0, 1.0), vec3(0.8, 0.8, 0.8));




void CaculateColor(vec3 lightPosition, vec3 intPoint, vec3 dir, vec3 centre, float &colourValue)
{
	vec3 lightToPos, objNormal, rVec, ttVec;
	float Amb, Diff, Spec, tt;
	float vecdot;

	lightToPos = normalize(lightPosition - intPoint);
	objNormal = normalize(intPoint - centre);
	Diff = std::max(0.0, (double)dot(lightToPos, objNormal));
	Amb = 0.5;

	vecdot = dot(objNormal, lightToPos);
	ttVec.x = objNormal.x*2.0 * vecdot;
	ttVec.y = objNormal.x*2.0 * vecdot;
	ttVec.z = objNormal.x*2.0 * vecdot;

	rVec = ttVec - lightToPos;
	tt = std::max(0.0, (double)dot(rVec, -dir));
	Spec = pow(tt, 20)*0.7;

	//colourValue = Amb + Diff + Spec;
	colourValue = Amb + Diff + Spec;
}
/*bool planeIntersect(vec3 dir, vec3 origin, float & t, vec3 & P)
{
	vec3 ray;
	t = (dot((center - origin), normal)/ (dot(dir, normal)));
	if (t <= 0.0f)
		return false;
	else
	{
		ray = origin + (t * dir);
		P = origin + (t * dir);
	}
	return true;
}*/


void RayTracing(SDL_Surface* surface)
{
	// I start by creating a vec 3 which points at the variable width and make it an array to make the base length of my display
	vec3 **Displayport = new vec3*[Width];
	vec3 mat_color, ballCenter, rayColor;
	float calColorVal;
	Uint32 colorSDL;
	Uint32* bufp;
	vec3 lightPosition = vec3(0.0, 20.0, -2);

	for (int i = (0); i < Width; i++) Displayport[i] = new vec3[Height];

	// this sets the aspect ratio of the display port
	Aspectratio = Width / (float)Height;

	// ths creates the tangent for me so that I can use it for later equations
	tanvalue = tanf((70 * pi / 180.0f) / 2);


	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++) //create a loop that runs through for the lengths of the height and width of the display port
		{
			//normalise the pixels and make it so that rays will shot through the center of each pixel rather then on the edges
			PixelNdx = (x + 0.5) / (float)Width;
			PixelNdy = (y + 0.5) / (float)Height;

			// I remap the coordinates so that they are set to a [-1,:1] so that the rays can pass through them correcly
			PixelRdx = (2 * PixelNdx - 1) * Aspectratio;
			PixelRdy = 1 - 2 * PixelNdy;

			// to get the camera position you take the remapped pixel plane and times it by the tangent to get the correct position. This will also be where the rays are being shot from
			Pcamex = PixelRdx * tanvalue;
			Pcamey = PixelRdy * tanvalue;


			vec3 dir = normalize(vec3(Pcamex, Pcamey, -1) - vec3(0, 0, 0));
			float hitDistance, rayRightangle, t;
			vec3 intPoint;


			for (int i = 0; i < ballList.size(); i++)
			{
				bool intersect = ballList[i].intersect(vec3(0, 0, 0), dir, t, intPoint);
				// if there is an intersection then show colour for that pixel in that area
				if (intersect)
				{
					mat_color = ballList[i].getColour();
					ballCenter = ballList[i].getCentre();
					CaculateColor(lightPosition, intPoint, dir, ballCenter, calColorVal);
					rayColor.r = mat_color.r*calColorVal;
					rayColor.g = mat_color.g*calColorVal;
					rayColor.b = mat_color.b*calColorVal;
					//Displayport[x][y] = rayColor;
					int r = (int)(rayColor.r*255);
					int g = (int)(rayColor.g*255);
					int b = (int)(rayColor.b*255);

					//r = b = g = 122;
					/*if (r > 255) r = 255;
					if (g > 255) g = 255;
					if (b > 255) b = 255;*/

					colorSDL = SDL_MapRGB(surface->format, r, g, b);
					bufp = (Uint32*)surface->pixels + y * surface->pitch / 4 + x;
					*bufp = colorSDL;
					break;
				}// if statement 

			}// for loop for each ball
		}// for loop width
	}//for loop height

}//end of function

bool close(bool quit_if_esc, bool delay) //delay makes CPU have some free time, use once per frame to avoid 100% usage of a CPU core
{
	if (delay) SDL_Delay(5); //so it consumes less processing power
	int done = 0;
	if (!SDL_PollEvent (& event)) return 0;
	// read keys
	// User requests quit
	if (event.type == SDL_QUIT)
	{
		done = 1;
	}
	//User presses a key
	else if (event.type == SDL_KEYDOWN)
	{
		//Select surfaces based on key press
		switch (event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			done = 1;
			break;

		default:
			break;
		}
	}

	return done;
}


int main(int argc, char* args[])
{
	SDL_Window* window = NULL;

	SDL_Surface* screenSurface = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialise! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//create the window
		window = SDL_CreateWindow("Ray Tracer - Matthew Dunphy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			screenSurface = SDL_GetWindowSurface(window);

			while (!close(true, false))
			{

				RayTracing(screenSurface);

				//Update the surface
				SDL_UpdateWindowSurface(window);

			}
			SDL_DestroyWindow(window);

			SDL_Quit();

			return 0;
		}
	}
}

