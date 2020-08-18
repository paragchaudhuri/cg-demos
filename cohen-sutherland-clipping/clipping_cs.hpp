/*
 A program which opens a window and clips the drawn lines
  and polygon.

  Modified from
  {
	  A program which opens a window and draws the "color cube."

	  Modified from An Introduction to OpenGL Programming, 
	  Ed Angel and Dave Shreiner, SIGGRAPH 2013

	  Written by Parag Chaudhuri, 2015
  }
*/
#ifndef _COLORCUBE_HPP_
#define _COLORCUBE_HPP_

// Defining the ESCAPE Key Code
#define ESCAPE 27
// Defining the DELETE Key Code
#define DELETE 127

//Include Files
#include "../common/glm/vec4.hpp"
#include "../common/glm/mat4x4.hpp"
#include "gl_framework.hpp"
#include "../common/shader_util.hpp"
#include <unistd.h>

//Variable to capture mouseclick positions
double cursorPosX, cursorPosY;
/*
Running variable to toggle the clipping primitive:
1. lines (false), or
2. polygons (true)
Initially set to line mode (false)
*/
bool clipPrim_mode = false;
//Running variable to start/stop clipping
bool startClip = false;
//Running variable for displaying animated clipping process
bool animDisplay = false;

void drawLines(int x1, int y1, int x2, int y2, bool prim_mode);
void reloadBuffers();
void clearBuffer();
void selectLines();
void clipLines();
void updateLines();
void clipPolygon();
int computeCode(double x, double y);
std::vector<float> cohenSutherlandLine(float x1, float y1, float x2, float y2);

#endif
