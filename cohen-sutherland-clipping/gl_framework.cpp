#include "gl_framework.hpp"

extern bool clipPrim_mode, startClip, animDisplay;
extern double cursorPosX, cursorPosY;

extern void drawLines(int x1, int y1, int x2, int y2, bool prim_mode);
extern void reloadBuffers();
extern void clearBuffer();
extern void clipPolygon();

namespace csX75
{
  //Variable to store current number of mouse clicks
  int numClicks = 0;
  //Variables to remember first and last mouse click
  float prevX, prevY, firstX, firstY;
  
  //! Initialize GL State
  void initGL(void)
  {
    //Set framebuffer clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //Set depth buffer furthest depth
    glClearDepth(1.0);
    //Set depth test to less-than
    glDepthFunc(GL_LESS);
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);
  }
  
  //!GLFW Error Callback
  void error_callback(int error, const char* description)
  {
    std::cerr<<description<<std::endl;
  }
  
  //!GLFW framebuffer resize callback
  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    //!Resize the viewport to fit the window size - draw to entire window
    glViewport(0, 0, width, height);
  }
  
  //!GLFW keyboard callback
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    //!Close the window if the ESC key was pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
   else if (key == GLFW_KEY_P && action == GLFW_PRESS)
   {    
   	if(!startClip)
   	{
           numClicks = 0;
	   clearBuffer();
	   reloadBuffers();
   	   clipPrim_mode = !clipPrim_mode;
	   if(!clipPrim_mode)
	     std::cout<<"Clipping lines"<<std::endl;
   	   else
	     std::cout<<"Clipping polygons"<<std::endl;
   	}
   	else
           std::cout<<"Cannot switch primitive in between ongoing clipping"<<std::endl;
   }
   else if (key == GLFW_KEY_C && action == GLFW_PRESS)
   {
        startClip = true;
        if(startClip)
        {
           std::cout<<"Clipping started"<<std::endl;
           animDisplay = true;
           
           startClip = false;
        }
        
        reloadBuffers();
   }
    
  }
  
  void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
  {
      if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
      {
          numClicks = numClicks+1;
          glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
          
          if(clipPrim_mode)
          {
             if(numClicks == 1)
             {
               firstX = cursorPosX;
               firstY = cursorPosY;
             }
          }
          
          if(!clipPrim_mode)
          {
             if((numClicks+1)%2 == 0)
             {
               prevX = cursorPosX;
               prevY = cursorPosY;
             }
             else
             {
                drawLines(prevX, prevY, cursorPosX, cursorPosY, clipPrim_mode);
             }
          }
          else
          {
             if(numClicks == 1)
             {
               prevX = cursorPosX;
               prevY = cursorPosY;
             }
             else if (numClicks == 2)
             { 
                drawLines(prevX, prevY, cursorPosX, cursorPosY, !clipPrim_mode);
                prevX = cursorPosX;
                prevY = cursorPosY;
             }
             else
             {
                drawLines(prevX, prevY, cursorPosX, cursorPosY, clipPrim_mode);
                prevX = cursorPosX;
                prevY = cursorPosY;
             }
          }
          
          reloadBuffers();
       }
   }
};  
  


