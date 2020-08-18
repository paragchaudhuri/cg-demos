/*
  CSX75: Demo - Clipping: Cohen Sutherland
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

#include "clipping_cs.hpp"


GLuint shaderProgram;
GLuint vbo, vao;

std::size_t sizeInBytes(std::vector<glm::vec4> arbitraryVector)
{ //can and should be overloaded
  return arbitraryVector.size() * sizeof(arbitraryVector[0]) ;
}
void renderGL();

//-----------------------------------------------------------------
int winSizeX = 512; // Window width
int winSizeY = 512; // Window height 
float W = 1; // Width of the window
float H = 1; // height of the window
float L = -0.5; // Left bound of the viewport
float R = 0.5;// Right bound of the viewport
float T = 0.5;// Top bound of the viewport
float B = -0.5;// Bottom bound of the viewport

//24 points in homogenous coordinates
glm::vec4 view_positions[24] = {
  glm::vec4(L, T, 0, 1.0),
  glm::vec4(-W, T, 0, 1.0),
  glm::vec4(R, T, 0, 1.0),
  glm::vec4(W, T, 0, 1.0),
  
  glm::vec4(L, B, 0, 1.0),
  glm::vec4(-W, B, 0, 1.0),
  glm::vec4(R, B, 0, 1.0),
  glm::vec4(W, B, 0, 1.0),
  
  glm::vec4(L, T, 0, 1.0),
  glm::vec4(L, H, 0, 1.0),
  glm::vec4(R, T, 0, 1.0),
  glm::vec4(R, H, 0, 1.0),
  
  glm::vec4(L, B, 0, 1.0),
  glm::vec4(L, -H, 0, 1.0),
  glm::vec4(R, B, 0, 1.0),
  glm::vec4(R, -H, 0, 1.0),
  
  glm::vec4(L, B, 0, 1.0),
  glm::vec4(R, B, 0, 1.0),
  glm::vec4(L, T, 0, 1.0),
  glm::vec4(R, T, 0, 1.0),
  
  glm::vec4(L, T, 0, 1.0),
  glm::vec4(L, B, 0, 1.0),
  glm::vec4(R, T, 0, 1.0),
  glm::vec4(R, B, 0, 1.0),
};

//RGBA colors for each point
glm::vec4 view_colors[24] = {
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  
  glm::vec4(1.0, 0.0, 0.0, 1.0),
  glm::vec4(1.0, 0.0, 0.0, 1.0),
  glm::vec4(1.0, 0.0, 0.0, 1.0),
  glm::vec4(1.0, 0.0, 0.0, 1.0),
  
  glm::vec4(1.0, 0.0, 0.0, 1.0),
  glm::vec4(1.0, 0.0, 0.0, 1.0),
  glm::vec4(1.0, 0.0, 0.0, 1.0),
  glm::vec4(1.0, 0.0, 0.0, 1.0)
};

//color of mouse click based lines
glm::vec4 lineColor = glm::vec4(0.0, 0.0, 1.0, 1.0);
//color of selected lines
glm::vec4 selectedColor = glm::vec4(0.0, 1.0, 0.0, 1.0);
//color of clipped(removed) lines
glm::vec4 clipColor = glm::vec4(1.0, 1.0, 0.0, 1.0);
//color of accepted lines
glm::vec4 acceptColor = glm::vec4(0.0, 1.0, 1.0, 1.0);
// arrays to store vertices and colors
std::vector<glm::vec4> v_positions, v_colors;
//Number of viewport lines
int num_view_points = sizeof(view_positions)/sizeof(view_positions[0]);
  
// generates 12 lines: 24 vertices and 24 colors
void viewportLines(void)
{
  int p_num = 0;
  while(p_num < num_view_points)
  {
     v_colors.push_back(view_colors[p_num]); v_positions.push_back(view_positions[p_num]);
     v_colors.push_back(view_colors[p_num+1]); v_positions.push_back(view_positions[p_num+1]);
     p_num+=2;
   }
}

//draws lines and polygons from mouse clicks
void drawLines(int x1, int y1, int x2, int y2, bool prim_mode)
{
   if(prim_mode && v_positions.size() > num_view_points+3)
   {
      v_colors.pop_back(); v_positions.pop_back();
      v_colors.pop_back(); v_positions.pop_back();
   }
   v_colors.push_back(lineColor); v_positions.push_back(glm::vec4(2*float(x1)/winSizeX - 1 , 1 - 2*float(y1)/winSizeY, 0, 1.0));
   v_colors.push_back(lineColor); v_positions.push_back(glm::vec4(2*float(x2)/winSizeX - 1 , 1 - 2*float(y2)/winSizeY, 0, 1.0));
   
   if(prim_mode)
   {
      v_colors.push_back(lineColor); v_positions.push_back(glm::vec4(2*float(x2)/winSizeX - 1 , 1 - 2*float(y2)/winSizeY, 0, 1.0));
      v_colors.push_back(lineColor); v_positions.push_back(v_positions.at(num_view_points));	
   }
}

/* Cohen Sutherland algorithm snippet modified from:
 https://www.geeksforgeeks.org/line-clipping-set-1-cohen-sutherland-algorithm/
*/
//Helper function for clipping functions
// Defining region codes 
const int INSIDE = 0; // 0000 
const int LEFT = 1; // 0001 
const int RIGHT = 2; // 0010 
const int BOTTOM = 4; // 0100 
const int TOP = 8; // 1000 

int computeCode(double x, double y) 
{ 
    // initialized as being inside 
    int code = INSIDE;
  
    if (x < L) // to the left of rectangle 
        code |= LEFT; 
    else if (x > R) // to the right of rectangle 
        code |= RIGHT; 
    if (y < B) // below the rectangle 
        code |= BOTTOM; 
    else if (y > T) // above the rectangle 
        code |= TOP; 
  
    return code; 
}

//Clips an input line using Cohen Sutherland Algorithm
std::vector<float> cohenSutherlandLine(float x1, float y1, float x2, float y2)
{
    // Accepted end points for a line
    std::vector<float> resPoints;
    // Compute region codes for P1, P2 
    int code1 = computeCode(x1, y1); 
    int code2 = computeCode(x2, y2); 
  
    // Initialize line as outside the rectangular window 
    bool accept = false;
  
    while (true) 
    { 
        if ((code1 == 0) && (code2 == 0)) 
        { 
            // If both endpoints lie within rectangle
            accept = true;
	    resPoints.push_back(x1);
	    resPoints.push_back(y1);
	    resPoints.push_back(x2);
	    resPoints.push_back(y2);
	    
            return resPoints; 
        } 
        else if (code1 & code2)
        { 
            // If both endpoints are outside rectangle in same region 
            resPoints.push_back(-1000);
            
            return resPoints; 
        } 
        else
        { 
            // Some segment of line lies within the rectangle 
            int code_out; 
            double x, y; 
  
            // At least one endpoint is outside the rectangle, pick it. 
            if (code1 != 0) 
                code_out = code1; 
            else
                code_out = code2; 
  
            // Find intersection point; using formulas
            // y = y1 + slope * (x - x1), 
            //x = x1 + (1 / slope) * (y - y1) 
            if (code_out & TOP)
            { 
                // point is above the clip rectangle 
                x = x1 + (x2 - x1) * (T - y1) / (y2 - y1); 
                y = T; 
            } 
            else if (code_out & BOTTOM)
            { 
                // point is below the rectangle 
                x = x1 + (x2 - x1) * (B - y1) / (y2 - y1); 
                y = B;
            } 
            else if (code_out & RIGHT)
            { 
                // point is to the right of rectangle 
                y = y1 + (y2 - y1) * (R - x1) / (x2 - x1); 
                x = R; 
            } 
            else if (code_out & LEFT) 
            { 
                // point is to the left of rectangle 
                y = y1 + (y2 - y1) * (L - x1) / (x2 - x1); 
                x = L; 
            } 
  
            // Now intersection point x, y is found 
            // We replace point outside rectangle 
            // by intersection point 
            if (code_out == code1) 
            { 
                x1 = x; 
                y1 = y;
                code1 = computeCode(x1, y1); 
            } 
            else 
            { 
                x2 = x; 
                y2 = y; 
                code2 = computeCode(x2, y2); 
            } 
        } 
    }
}

int currIdx = num_view_points; // counter for the line being clipped
int currCase = -1; // maintain current case of clipping
int counter = 0; // auxiliary counter

//Select line to clip
void selectLines()
{
     if(currIdx+1 < v_positions.size())
     {
     	v_colors.at(currIdx) = selectedColor;
        v_colors.at(currIdx+1) = selectedColor;  
     }
     else
     	animDisplay = false; 
     	
     if(currIdx > num_view_points)
     {
	 v_colors.at(currIdx-1) = acceptColor;
	 v_colors.at(currIdx-2) = acceptColor;
     } 
}

//Clips the input line by the rectangular region
void clipLines()
{
	if(currIdx+1 > v_positions.size())
	  return;

    	float x1 = v_positions.at(currIdx)[0];
	float y1 = v_positions.at(currIdx)[1];
	float x2 = v_positions.at(currIdx+1)[0];
	float y2 = v_positions.at(currIdx+1)[1];
	
	std::vector<float> acceptedPoints = cohenSutherlandLine(x1, y1, x2, y2);
    
	if(acceptedPoints.size() == 4)
	{
	   v_positions.at(currIdx)[0] = acceptedPoints.at(0);
	   v_positions.at(currIdx)[1] = acceptedPoints.at(1);
	   v_positions.at(currIdx+1)[0] = acceptedPoints.at(2);
	   v_positions.at(currIdx+1)[1] = acceptedPoints.at(3);
	   
	   if(  x1 == acceptedPoints.at(0) && y1 == acceptedPoints.at(1)
	     && x2 == acceptedPoints.at(2) && y2 == acceptedPoints.at(3))
	   {
	   	currCase = 1;
	   	//currIdx = currIdx + 2;
	   }
     	   else if(  x1 != acceptedPoints.at(0) && y1 != acceptedPoints.at(1)
	          && x2 != acceptedPoints.at(2) && y2 != acceptedPoints.at(3))
	   {
	        v_positions.insert(v_positions.begin() + currIdx + 2, glm::vec4(x1, y1, 0, 1.0));
	        v_positions.insert(v_positions.begin() + currIdx + 3, glm::vec4(acceptedPoints.at(0), acceptedPoints.at(1), 0, 1.0));
	        
	        v_colors.insert(v_colors.begin() + currIdx + 2, clipColor);
	        v_colors.insert(v_colors.begin() + currIdx + 3, clipColor);
	        
	        v_positions.insert(v_positions.begin() + currIdx + 4, glm::vec4(acceptedPoints.at(2), acceptedPoints.at(3), 0, 1.0));
	        v_positions.insert(v_positions.begin() + currIdx + 5, glm::vec4(x2, y2, 0, 1.0));
	        
	        v_colors.insert(v_colors.begin() + currIdx + 4, clipColor);
	        v_colors.insert(v_colors.begin() + currIdx + 5, clipColor);
	        
	        currCase = 2;
	        //currIdx = currIdx + 6;
	   }
	   else if(  x1 == acceptedPoints.at(0) && y1 == acceptedPoints.at(1)
	          && x2 != acceptedPoints.at(2) && y2 != acceptedPoints.at(3))
	   {
	   	v_positions.insert(v_positions.begin() + currIdx + 2, glm::vec4(acceptedPoints.at(2), acceptedPoints.at(3), 0, 1.0));
	        v_positions.insert(v_positions.begin() + currIdx + 3, glm::vec4(x2, y2, 0, 1.0));
	        
	        v_colors.insert(v_colors.begin() + currIdx + 2, clipColor);
	        v_colors.insert(v_colors.begin() + currIdx + 3, clipColor);
	        
	        currCase = 3;
	        //currIdx = currIdx + 4;
	   }
	   else if(  x1 != acceptedPoints.at(0) && y1 != acceptedPoints.at(1)
	          && x2 == acceptedPoints.at(2) && y2 == acceptedPoints.at(3))
	   {
	   	v_positions.insert(v_positions.begin() + currIdx + 2, glm::vec4(x1, y1, 0, 1.0));
	   	v_positions.insert(v_positions.begin() + currIdx + 3, glm::vec4(acceptedPoints.at(0), acceptedPoints.at(1), 0, 1.0));
	        
	        v_colors.insert(v_colors.begin() + currIdx + 2, clipColor);
	        v_colors.insert(v_colors.begin() + currIdx + 3, clipColor);
	        
	        currCase = 4;
	        //currIdx = currIdx + 4;
	   }	   
	}
	else
	{
           v_colors.at(currIdx) = clipColor;
           v_colors.at(currIdx+1) = clipColor;
           
           currCase = 5;
	}
}

//Update the lines according to clipping
void updateLines()
{
    if (currCase == -1)
     	return;  	
     
     if(currCase == 1)
     	currIdx = currIdx + 2;	
     else if(currCase == 2)
     {	
	for(int i = 0; i < 4; i++)
	{
	    v_positions.erase(v_positions.begin() + currIdx + 2);
	    v_colors.erase(v_colors.begin() + currIdx + 2);
	}
	
	currIdx = currIdx + 2;	
     }
     else if (currCase == 3 || currCase == 4)
     {	
	for(int i = 0; i < 2; i++)
	{
	    v_positions.erase(v_positions.begin() + currIdx + 2);
	    v_colors.erase(v_colors.begin() + currIdx + 2);
	}
	
	currIdx = currIdx + 2;
     }
     else if (currCase == 5)
     {
	for(int i = 0; i < 2; i++)
	{
	    v_positions.erase(v_positions.begin() + currIdx);
	    v_colors.erase(v_colors.begin() + currIdx);
	}
     }

     currCase = -1;
}

//Clips the input polygon by the rectangular region
void clipPolygon()
{

}
//-----------------------------------------------------------------

void initBuffersGL(void)
{
  viewportLines();
  
  //Ask GL for a Vertex Attribute Object (vao)
  glGenVertexArrays (1, &vao);
  //Set it as the current array to be used by binding it
  glBindVertexArray (vao);

  //Ask GL for a Vertex Buffer Object (vbo)
  glGenBuffers (1, &vbo);
  //Set it as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER, sizeInBytes(v_positions) + sizeInBytes(v_colors), NULL, GL_DYNAMIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeInBytes(v_positions), &v_positions[0][0] );
  glBufferSubData( GL_ARRAY_BUFFER, sizeInBytes(v_positions), sizeInBytes(v_colors), &v_colors[0][0] );

  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("clipping_cs_vshader.glsl");
  std::string fragment_shader_file("clipping_cs_fshader.glsl");

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
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeInBytes(v_positions)) );
}

void reloadBuffers()
{
  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER, sizeInBytes(v_positions) + sizeInBytes(v_colors), NULL, GL_DYNAMIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeInBytes(v_positions), &v_positions[0][0] );
  glBufferSubData( GL_ARRAY_BUFFER, sizeInBytes(v_positions), sizeInBytes(v_colors), &v_colors[0][0] );

  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  
  GLuint vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeInBytes(v_positions)) );
}

void clearBuffer()
{
  int len = v_positions.size()-num_view_points;
  for(int i = 0; i < len; i++)
  {
     v_positions.pop_back();
     v_colors.pop_back();
  }
}

void renderGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw 
  glDrawArrays(GL_LINES, 0, v_positions.size());
}

int main(int argc, char** argv)
{
  //! The pointer to the GLFW window
  GLFWwindow* window;

  //! Setting up the GLFW Error callback
  glfwSetErrorCallback(csX75::error_callback);

  //! Initialize GLFW
  if (!glfwInit())
    return -1;

  //We want OpenGL 4.0
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  //This is for MacOSX - can be omitted otherwise
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  //We don't want the old OpenGL 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  //! Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(winSizeX, winSizeY, "CS475/CS675 Demo: Clipping - Cohen Sutherland", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }
  
  //! Make the window's context current 
  glfwMakeContextCurrent(window);

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
  glfwSetKeyCallback(window, csX75::key_callback);
  //Mouse Callback
  glfwSetMouseButtonCallback(window, csX75::mouse_button_callback);
  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  //Initialize GL state
  csX75::initGL();
  initBuffersGL();
  
  // delay in showing the output
  float delay = 2;
  
  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0)
  {       
      // Render here
      if(animDisplay)
      {
  	  if(!clipPrim_mode)
  	  {
  	     selectLines();
             reloadBuffers();
             sleep(delay);
       
             clipLines();
             reloadBuffers();
             renderGL();
             sleep(delay);
         
             updateLines();
             reloadBuffers();
  	  }
  	  else
  	  {
  	      clipPolygon();
  	  }
      }
      else
          renderGL();
      // Swap front and back buffers
      glfwSwapBuffers(window);
      
      // Poll for and process events
      glfwPollEvents();

    }
  
  glfwTerminate();
  return 0;
}

//-------------------------------------------------------------------------

