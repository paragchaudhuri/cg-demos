#include "gl_framework.hpp"

extern GLfloat xrot,yrot,zrot;
extern double cursorXpos, cursorYpos;

extern void line1 (int x0, int y0, int x1, int y1);
extern void line2 (int x0, int y0, int x1, int y1);
extern void reloadBuffers(void);

namespace csX75
{
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
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
      yrot -= 1.0;
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
      yrot += 1.0;
    else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
      xrot += 1.0;
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
      xrot += 1.0;
    else if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS)
      zrot += 1.0;
    else if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS)
      zrot += 1.0;
  }

  bool numClicks =0;
  int x0,y0;

  void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
  {
      if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        // double cursorXpos, cursorYpos;
        numClicks = (numClicks+1)%2;
        glfwGetCursorPos(window, &cursorXpos, &cursorYpos);
        if(numClicks){
          std::cerr<<"Odd  "<<cursorXpos<<" , "<<cursorYpos<<std::endl;
          x0 = cursorXpos;
          y0 = cursorYpos;
          // x0 = cursorXpos;
          // y0 = cursorYpos;
        }
        else{
          std::cerr<<"Even "<<cursorXpos<<" , "<<cursorYpos<<std::endl;
          line2(x0,y0,cursorXpos,cursorYpos);
          // line2(x0,y0,cursorYpos,cursorXpos);
          reloadBuffers();
        }
      }
          
  }

};  
  


