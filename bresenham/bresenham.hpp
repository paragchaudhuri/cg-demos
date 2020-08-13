/*
  A program which opens a window and draws the "color cube."

  Use the arrow keys and PgUp,PgDn, 
  keys to make the cube move.

  Written by - 
               Parag Chaudhuri
*/
#ifndef _COLORCUBE_HPP_
#define _COLORCUBE_HPP_

// Defining the ESCAPE Key Code
#define ESCAPE 27
// Defining the DELETE Key Code
#define DELETE 127

#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


// Translation Parameters
GLfloat xpos=0.0,ypos=0.0,zpos=0.0;
// Rotation Parameters
GLfloat xrot=0.0,yrot=0.0,zrot=0.0;

double cursorXpos, cursorYpos;

//Running variable to toggle culling on/off
bool enable_culling=true;
//Running variable to toggle wireframe/solid modelling
bool solid=true;

void line1_F(int x0, int y0, int x1, int y1);
void line8_F(int x0, int y0, int x1, int y1);
void line1_I(int x0, int y0, int x1, int y1);
void line8_I(int x0, int y0, int x1, int y1);
void pointsForGL(int x0, int y0, int x1, int y1);

void reloadBuffers(CONTEXT context);


  GLFWwindow* window1;
  GLFWwindow* window2;

//-------------------------------------------------------------------------

#endif
