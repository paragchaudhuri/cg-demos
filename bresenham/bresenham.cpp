/*
CSX75 Demo:
  A program which opens 2 windows and draws a "line"
  Modified from:
  {
    CSX75 Tutorial 3

    A program which opens 1 windows and draws a "color cube."

    Modified from An Introduction to OpenGL Programming, 
    Ed Angel and Dave Shreiner, SIGGRAPH 2013

    Written by Parag Chaudhuri, 2015
  }
  2020
*/


#include "bresenham.hpp"

GLuint shaderProgram;
GLuint vbo[1], vao[1];

glm::mat4 rotation_matrix;
glm::mat4 ortho_matrix;
glm::mat4 modelview_matrix;
GLuint uModelViewMatrix;


//-----------------------------------------------------------------


glm::vec4 startPt,endPt,deltaPos;


std::vector<glm::vec4> vectorOfPoints,vectorOfColors,vectorOf2Points,vectorOf2Colors;
//Note that the vectors of known lengths are vectors just for code consistency


int winSizeX=512,winSizeY=512;

std::size_t sizeInBytes(std::vector<glm::vec4> arbitraryVector){ //can and should be overloaded
  return arbitraryVector.size() * sizeof(arbitraryVector[0]) ;
}


void swap (int &x, int &y)
{
  int temp = x;
  x = y; y = temp;
}



//Basic Bresenham line drawing(works only in first octant)
void line1_F (int x0, int y0, int x1, int y1)
{
  vectorOfPoints.clear();
  vectorOfColors.clear();
  int deltax = x1 - x0;
  int deltay = y1 - y0;
  
  float error = 0;
  float newerr = 0;
  float deltaerr = (float)deltay / (float)deltax;

  int y = y0;
  int newy = y0;

  for (int x = x0; x < x1; x++){

      error = error + deltaerr;

      //glVertex2f(x, y);
      vectorOfPoints.push_back( glm::vec4(2*float(x)/winSizeX - 1 , 1 - 2*float(y)/winSizeY ,0,1) );
      vectorOfColors.push_back(glm::vec4(1.0, 0.1, 0.1, 1.0) );

      if (error > 0.5) 
      {
        y = y + 1;
        error = error - 1.0;
      }
    }
}

//Generalized Bresenham (works for all lines)
// line1_F Works in one octant.
// for steep cases, swapping x and y makes it work in X 2 octants
// for decreasing x cases, swapping x1,y1 and x2,y2 makes it work in X 2 octants
// for negative slope cases ,  considering negative steps makes it work in X 2 octants
// total: 1 X 8 octants
void line8_F (int x0, int y0, int x1, int y1)
{
  vectorOfPoints.clear();
  vectorOfColors.clear();
  bool steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep)
    {
      swap(x0, y0);
      swap(x1, y1);
    }
  if (x0 > x1)
    {
      swap(x0, x1);
      swap(y0, y1);
    }
  int deltax = x1 - x0;
  int deltay = abs(y1 - y0);

  float error = 0.0;
  float deltaerr = (float)deltay / (float)deltax;

  int ystep;
  int y = y0;

  if (y0 < y1) ystep = 1; else ystep = -1;


  for (int x=x0; x < x1; x++)
    {
      if (steep){
        vectorOfPoints.push_back( glm::vec4(2*float(y)/winSizeX - 1 , 1 - 2*float(x)/winSizeY ,0,1) );
        vectorOfColors.push_back(glm::vec4(0.1, 1.0, 0.1, 1.0) );
      }
      else{
        vectorOfPoints.push_back( glm::vec4(2*float(x)/winSizeX - 1 , 1 - 2*float(y)/winSizeY ,0,1) );
        vectorOfColors.push_back(glm::vec4(0.1, 1.0, 0.1, 1.0) );
      } 
    

      error = error + deltaerr;
      if (error >= 0.5) 
  {
    y = y + ystep;
    error = error - 1.0;
  }
    }
  //glEnd();
}


//Integer Optimized Bresenham line drawing(works only in first octant)
void line1_I (int x0, int y0, int x1, int y1)
{

  //Please refer to notes posted for the derivation of this integer-optimized version.

  vectorOfPoints.clear();
  vectorOfColors.clear();
  int deltax = x1 - x0;
  int deltay = y1 - y0;

  int d = 2*deltay- deltax;


  int y = y0;

  for (int x = x0; x < x1; x++){


    if(d<0){
      d += 2*deltay;
    }
    else{
      d += 2*deltay - 2*deltax;
      y++;
    }
      //The following part has float values for point plotting in OpenGL but floats are not used in the bresenham component! 
      vectorOfPoints.push_back( glm::vec4(2*float(x)/winSizeX - 1 , 1 - 2*float(y)/winSizeY ,0,1) );
      vectorOfColors.push_back(glm::vec4(1.0, float(y-y0)/deltay, float(y-y0)/deltay, 1.0) );

    }
}


//Integer Optimized Bresenham line drawing(works in all octant)
void line8_I (int x0, int y0, int x1, int y1)
{
  vectorOfPoints.clear();
  vectorOfColors.clear();

  bool steep = abs(y1-y0) > abs(x1 - x0);

  int mult=1;

  if(steep){
    swap(x0,y0);
    swap(x1,y1);
  }

  if(x0>x1){
    swap(x0,x1);
    swap(y0,y1);
  }

  if(y0>y1){
    mult =-1;
  }

  y0*=mult;
  y1*=mult;

  int deltax = x1 - x0;
  int deltay = y1 - y0;


  int d = 2*deltay- deltax;


  int y = y0;

  for (int x = x0; x < x1; x++){


    if(d<0){
      d += 2*deltay;
    }
    else{
      d += 2*deltay - 2*deltax;
      y++;
    }
      //The following part has float values for point plotting in OpenGL but floats are not used in the bresenham component! 
    if(steep)
      vectorOfPoints.push_back( glm::vec4(2*float(mult*y)/winSizeX - 1 , 1 - 2*float(x)/winSizeY ,0,1) );
    else
      vectorOfPoints.push_back( glm::vec4(2*float(x)/winSizeX - 1 , 1 - 2*float(mult*y)/winSizeY ,0,1) );

      vectorOfColors.push_back(glm::vec4(float(y-y0)/deltay, 1.0, float(y-y0)/deltay, 1.0) );

    }
}



void pointsForGL(int x0, int y0, int x1, int y1){
      vectorOf2Points.clear();
      vectorOf2Colors.clear();
      int x = x0;
      int y = y0;
      vectorOf2Points.push_back( glm::vec4(2*float(x)/winSizeX - 1 , 1 - 2*float(y)/winSizeY ,0,1) );
      vectorOf2Colors.push_back(glm::vec4(1.0, 0.5, 1.0, 1.0) );
      x=x1; y=y1;
      vectorOf2Points.push_back( glm::vec4(2*float(x)/winSizeX - 1 , 1 - 2*float(y)/winSizeY ,0,1) );
      vectorOf2Colors.push_back(glm::vec4(1.0, 0.5, 1.0, 1.0) );
}


void initBuffersGL(CONTEXT context)
{
  assert(context == WIN_BRESENHAM || context == WIN_OPENGL);
  if( context == WIN_BRESENHAM )
    line1_F(-1,-1,0,0);
  if( context == WIN_OPENGL )
    pointsForGL(-1,-1,0,0);

  //Ask GL for a Vertex Attribute Object (vao)
  glGenVertexArrays (1, &vao[0]);
  //Set it as the current array to be used by binding it
  glBindVertexArray (vao[0]);

  //Ask GL for a Vertex Buffer Object (vbo)
  glGenBuffers (1, &vbo[0]);
  //Set it as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[0]);
  //Copy the points into the current buffer
  if( context == WIN_BRESENHAM ){
  glBufferData (GL_ARRAY_BUFFER, sizeInBytes(vectorOfPoints) + sizeInBytes(vectorOfColors), NULL, GL_DYNAMIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeInBytes(vectorOfPoints), &vectorOfPoints[0][0] );
  glBufferSubData( GL_ARRAY_BUFFER, sizeInBytes(vectorOfPoints), sizeInBytes(vectorOfColors), &vectorOfColors[0][0] );
  }

  if( context == WIN_OPENGL ){
  glBufferData (GL_ARRAY_BUFFER, sizeInBytes(vectorOf2Points) + sizeInBytes(vectorOf2Colors), NULL, GL_DYNAMIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeInBytes(vectorOf2Points), &vectorOf2Points[0][0] );
  glBufferSubData( GL_ARRAY_BUFFER, sizeInBytes(vectorOf2Points), sizeInBytes(vectorOf2Colors), &vectorOf2Colors[0][0] );
  }
  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("bh_vshader.glsl");
  std::string fragment_shader_file("bh_fshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = csX75::CreateProgramGL(shaderList);
  glUseProgram( shaderProgram );

  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  
  GLuint vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
  glEnableVertexAttribArray( vColor );
  if( context == WIN_BRESENHAM )
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeInBytes(vectorOfPoints)) );
  if( context == WIN_OPENGL )
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeInBytes(vectorOf2Points)) );

  glPointSize(1);
  //uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");
}


void reloadBuffers(CONTEXT context)
{
  assert(context == WIN_BRESENHAM || context == WIN_OPENGL);

  if(context==WIN_BRESENHAM){
  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER, sizeInBytes(vectorOfPoints) + sizeInBytes(vectorOfColors), NULL, GL_DYNAMIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeInBytes(vectorOfPoints), &vectorOfPoints[0][0] );
  glBufferSubData( GL_ARRAY_BUFFER, sizeInBytes(vectorOfPoints), sizeInBytes(vectorOfColors), &vectorOfColors[0][0] );


  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  
  GLuint vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeInBytes(vectorOfPoints)) );

  //uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");
  }

  if(context==WIN_OPENGL){
  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER, sizeInBytes(vectorOf2Points) + sizeInBytes(vectorOf2Colors), NULL, GL_DYNAMIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeInBytes(vectorOf2Points), &vectorOf2Points[0][0] );
  glBufferSubData( GL_ARRAY_BUFFER, sizeInBytes(vectorOf2Points), sizeInBytes(vectorOf2Colors), &vectorOf2Colors[0][0] );


  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  
  GLuint vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeInBytes(vectorOf2Points)) );

  //uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");
  }
  glPointSize(1);
}




void renderGL(int context)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Draw
  if(context == WIN_BRESENHAM)
    glDrawArrays(GL_POINTS,0,vectorOfPoints.size()) ; 
  if(context == WIN_OPENGL)
    glDrawArrays(GL_LINES,0,vectorOf2Points.size()) ;  
}



int main(int argc, char** argv)
{
  //! The pointer to the GLFW window


  //! Setting up the GLFW Error callback
  glfwSetErrorCallback(csX75::error_callback);

  //! Initialize GLFW
  if (!glfwInit())
    return -1;

  //We want OpenGL 4.0
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //This is for MacOSX - can be omitted otherwise
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  //We don't want the old OpenGL 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 


  //! Create a windowed mode window and its OpenGL context
  window1 = glfwCreateWindow(winSizeX, winSizeY, "CS475/CS675 Demo: Bresenham's Line Algorithm", NULL, NULL);
  if (!window1)
    {
      glfwTerminate();
      return -1;
    }

  window2 = glfwCreateWindow(winSizeX, winSizeY, "CS475/CS675 Demo: OpenGL Line", NULL, NULL);
  if (!window2)
    {
      glfwTerminate();
      return -1;
    }

  int winXpos, winYpos;
  glfwGetWindowPos(window1, &winXpos, &winYpos);

  glfwSetWindowPos(window2, winXpos + winSizeX + 4, winYpos);

  //! Make the window's context current 
  glfwMakeContextCurrent(window1);

  //Initialize GLEW
  //Turn this on to get Shader based OpenGL
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
      //Problem: glewInit failed, something is seriously wrong.
      std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }

  //Print and see what context got enabled
  std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
  std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
  std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
  std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;


  //Keyboard Callback
  glfwSetKeyCallback(window1, csX75::key_callback);
  glfwSetKeyCallback(window2, csX75::key_callback);

  glfwSetMouseButtonCallback(window1, csX75::mouse_button_callback);
  //glfwSetMouseButtonCallback(window2, csX75::mouse_button_callback);

  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window1, csX75::framebuffer_size_callback);
  glfwSetFramebufferSizeCallback(window2, csX75::framebuffer_size_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window1, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSetInputMode(window2, GLFW_STICKY_KEYS, GL_TRUE);

  //Initialize GL state
  csX75::initGL();
  initBuffersGL(WIN_BRESENHAM);
  //initBuffersGL();

  glfwMakeContextCurrent(window2);
  csX75::initGL();
  initBuffersGL(WIN_OPENGL);

  std::cout<<"------------------------------" <<std::endl;
  std::cout<<"Starting with ONE_FLOAT mode ..." <<std::endl;
  std::cout<<"Press 1 for ONE_FLOAT mode ..." <<std::endl;
  std::cout<<"Press 2 for ALL_FLOAT mode ..." <<std::endl;
  std::cout<<"Press 3 for ONE_INT mode ..." <<std::endl;
  std::cout<<"Press 4 for ALL_INT mode ..." <<std::endl;
  std::cout<<"------------------------------" <<std::endl;

  // Loop until the user closes the window
  while (glfwWindowShouldClose(window1) == 0  && glfwWindowShouldClose(window2) == 0)
    {
      // Render here
      
      glfwMakeContextCurrent(window1);
      renderGL(WIN_BRESENHAM);

      // Swap front and back buffers
      glfwSwapBuffers(window1);
      
      glfwMakeContextCurrent(window2);
      renderGL(WIN_OPENGL);
      glfwSwapBuffers(window2);

      // Poll for and process events
      glfwPollEvents();
    }
  
  glfwTerminate();
  return 0;
}

//-------------------------------------------------------------------------

