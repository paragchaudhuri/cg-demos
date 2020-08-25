#include "gl_framework.hpp"


extern GLfloat xrot,yrot,zrot;
extern double cursorXpos, cursorYpos;

extern void line1_F (int x0, int y0, int x1, int y1);
extern void line8_F (int x0, int y0, int x1, int y1);
extern void line1_I (int x0, int y0, int x1, int y1);
extern void line8_I (int x0, int y0, int x1, int y1);
extern void pointsForGL(int x0, int y0, int x1, int y1);
extern void reloadBuffers(CONTEXT context);


extern GLFWwindow* window1;
extern GLFWwindow*  window2;

extern int winSizeY;

namespace csX75
{

  bool numClicks =0;
  //cursor positions:
  int x0,y0;

#define ONE_FLOAT 1
#define ALL_FLOAT 2
#define ONE_INT 3
#define ALL_INT 4
  //spend only 1 byte to determine mode
  char mode = ONE_FLOAT;

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
    else if (key == GLFW_KEY_1 && action == GLFW_PRESS){
      mode = ONE_FLOAT;
      std::cerr<<"ONE_FLOAT mode."<<std::endl;
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS){
      mode = ALL_FLOAT;
      std::cerr<<"ALL_FLOAT mode."<<std::endl;
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS){
      mode = ONE_INT;
      std::cerr<<"ONE_INT mode."<<std::endl;
    }
    else if (key == GLFW_KEY_4 && action == GLFW_PRESS){
      mode = ALL_INT;
      std::cerr<<"ALL_INT mode."<<std::endl;
    }
  }



  void mouse_button_callback(GLFWwindow* currWindow, int button, int action, int mods)
  {
      if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        numClicks = (numClicks+1)%2;
        glfwGetCursorPos(currWindow, &cursorXpos, &cursorYpos);
        if(numClicks){
          //std::cerr<<"Odd  click: "<<cursorXpos<<" , "<<cursorYpos<<std::endl;
          x0 = cursorXpos;
          y0 = cursorYpos;
        }
        else{
          //std::cerr<<"Even click: "<<cursorXpos<<" , "<<cursorYpos<<std::endl;
          if (mode == ONE_FLOAT)
            line1_F(x0,winSizeY-y0,cursorXpos,winSizeY-cursorYpos);
          if (mode == ALL_FLOAT)
            line8_F(x0,y0,cursorXpos,cursorYpos);
          if (mode == ONE_INT)
            line1_I(x0,winSizeY-y0,cursorXpos,winSizeY-cursorYpos);
          pointsForGL(x0,y0, cursorXpos,cursorYpos);
          if (mode == ALL_INT)
            line8_I(x0,y0,cursorXpos,cursorYpos);
          // line2(x0,y0,cursorYpos,cursorXpos);
          glfwMakeContextCurrent(window1);
          reloadBuffers(WIN_BRESENHAM);
          glfwMakeContextCurrent(window2);
          
          reloadBuffers(WIN_OPENGL);
          glfwMakeContextCurrent(currWindow);
        }
      }
          
  }

};  
  


