/*
CSX75 Demo:
  A program to test effects of various transformations on a standard 3D object.
  Modified from:
  {
    CSX75 Tutorial 3

    A program which opens 1 windows and draws a "color cube."

    Modified from An Introduction to OpenGL Programming, 
    Ed Angel and Dave Shreiner, SIGGRAPH 2013

    Written by Parag Chaudhuri, 2015
  }
  teapot obj sourced from: https://graphics.stanford.edu/courses/cs148-10-summer/as3/code/as3/teapot.obj
  2020
*/

#include <iostream>
#include <fstream>
#include <string>

#include "modeling-transforms.hpp"


#include "../common/imgui/imgui.h"
#include "../common/imgui/imgui_impl_glfw.h"
#include "../common/imgui/imgui_impl_opengl3.h"

#define PI 3.141592

GLuint shaderProgram;
GLuint vbo[1], vao[1];

int winSizeX=1280,winSizeY=720;


glm::mat4 modelviewproject_matrix;

glm::mat4 projection_matrix;
glm::mat4 ortho_matrix;
glm::mat4 perspective_matrix;
int projectionType=1;

glm::mat4 view_matrix;
glm::mat4 model_matrix;


glm::mat4 rotation_matrix;
glm::mat4 scaling_matrix;
glm::mat4 translation_matrix;

GLuint uModelViewMatrix;


//-----------------------------------------------------------------
const char* assetPath = "../common/assets/teapot.obj";
//-----------------------------------------------------------------


glm::vec4 startPt,endPt,deltaPos;


std::vector<glm::vec4> vectorOfPoints,vectorOfColors,vectorOfNormals,vectorOfTex;
std::vector<glm::vec3> objVertices, objNormals;
std::vector<glm::vec2> objTex;




std::size_t sizeInBytes(std::vector<glm::vec4> arbitraryVector){ //can and should be overloaded
  return arbitraryVector.size() * sizeof(arbitraryVector[0]) ;
}


void fetchObj(const char* objPath){
  std::fstream objFile;
  objFile.open(objPath,std::ios::in);
   if (objFile.is_open()){ 
      std::string fileLine;
      while(getline(objFile, fileLine)){

        //--------------------------------------
        std::string word ="";
        std::vector<std::string> wordlist;
        for(auto c :fileLine){
          if (c == ' '){
            wordlist.push_back(word);
            word="";
          }
          else{
            word = word+c;
          }

        }
        wordlist.push_back(word);
        //---------------------------------------

        if(fileLine[0]=='v' && fileLine[1]==' '){
          glm::vec3 vertex = glm::vec3(std::stof(wordlist[1]), std::stof(wordlist[2]) , std::stof(wordlist[3]));
          objVertices.push_back( vertex );
        } 
        if(fileLine[0]=='f' && fileLine[1]==' '){
          for(int i =1; i<4; i++){
            int vIdx = std::stoi(wordlist[i])-1;
            vectorOfPoints.push_back(glm::vec4( objVertices[vIdx] ,1.0f) );
            vectorOfColors.push_back(glm::vec4( (objVertices[vIdx]/4.0f +0.5f)  ,1.0) );
          }

        }

      }
      objFile.close(); //close the file object.
   }
   else{
    std::cerr<<"Could not open obj file."<<std::endl;
   }
}


void initMatrices(){
  view_matrix = glm::lookAt(glm::vec3(0.0f, 1.2f, -8.0f), //camerapos
                            glm::vec3(0.0f, 1.2f, 0.0f),  //targetpos
                            glm::vec3(0.0f, 1.0f, 0.0f)); //up
  float aspect = (float)winSizeX/(float)winSizeY;
  perspective_matrix =  glm::perspective(glm::radians(45.0f), aspect , 0.1f, 100.0f);
  glm::mat4 fixAR = glm::mat4();
  fixAR[1][1] = aspect;
  ortho_matrix = fixAR * glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
}

void initBuffersGL(CONTEXT context)
{
  assert(context == WIN_TRANSFORM);
  if( context == WIN_TRANSFORM )
    fetchObj(assetPath);


  //Ask GL for a Vertex Attribute Object (vao)
  glGenVertexArrays (1, &vao[0]);
  //Set it as the current array to be used by binding it
  glBindVertexArray (vao[0]);

  //Ask GL for a Vertex Buffer Object (vbo)
  glGenBuffers (1, &vbo[0]);
  //Set it as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[0]);
  //Copy the points into the current buffer
  if( context == WIN_TRANSFORM ){
  glBufferData (GL_ARRAY_BUFFER, sizeInBytes(vectorOfPoints) + sizeInBytes(vectorOfColors), NULL, GL_DYNAMIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeInBytes(vectorOfPoints), &vectorOfPoints[0][0] );
  glBufferSubData( GL_ARRAY_BUFFER, sizeInBytes(vectorOfPoints), sizeInBytes(vectorOfColors), &vectorOfColors[0][0] );
  }


  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("mtr_vshader.glsl");
  std::string fragment_shader_file("mtr_fshader.glsl");

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
  if( context == WIN_TRANSFORM )
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeInBytes(vectorOfPoints)) );


  
  uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");
  //glPointSize(1);
}


void reloadBuffers(CONTEXT context)
{
  assert(context == WIN_TRANSFORM);

  if(context==WIN_TRANSFORM){
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

    
  }

  //glPointSize(1);
}




void renderGL(int context)
{
  modelviewproject_matrix = projection_matrix*view_matrix*model_matrix;

  glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelviewproject_matrix));

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Draw
  if(context == WIN_TRANSFORM){
    glDrawArrays(GL_TRIANGLES,0,vectorOfPoints.size()) ;  
  }
}





void rotate(float Rx, float Ry,float Rz){
        //assuming x then y then z
        //Deg to rad
        Rx *=PI/180.0; 
        Ry *=PI/180.0; 
        Rz *=PI/180.0;
        //Column major!
        glm::mat4 rotx{
        1, 0, 0, 0,
        0, cos(Rx), sin(Rx), 0,
        0, -sin(Rx), cos(Rx), 0,
        0, 0, 0, 1
        };

        glm::mat4 roty{
        cos(Ry), 0, -sin(Ry), 0,
        0, 1, 0, 0,
        sin(Ry), 0, cos(Ry), 0,
        0, 0, 0, 1
        };

        glm::mat4 rotz{
        cos(Rz), sin(Rz), 0, 0,
        -sin(Rz), cos(Rz), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
        };

        rotation_matrix = rotz*roty*rotx;
}

void translate(float Tx, float Ty,float Tz){     
        //Column major!
        glm::mat4 transMat{
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        Tx, Ty, Tz, 1
        };

        translation_matrix = transMat;
}

void scale(float Sx, float Sy,float Sz){     
        //Column major!
        translation_matrix[0][0] = Sx;
        translation_matrix[1][1] = Sy;
        translation_matrix[2][2] = Sz;
}


void printMat4(const char* name, glm::mat4 matval){

  ImGui::Begin(name); 
  ImGui::Text("\n");
  for(int i =0; i<4; i++){
    for(int j =0; j<4; j++){
      ImGui::Text("%.2f  ",matval[j][i]); ImGui::SameLine();
    }
     ImGui::Text(" ");
  }


  ImGui::End();

}


void window_size_callback(GLFWwindow* window, int width, int height)
{
    winSizeX = width;
    winSizeY = height;
    //fixAR[0][0] = std::max(winSizeX,winSizeY)/float(winSizeX);
    //fixAR[1][1] = std::max(winSizeX,winSizeY)/float(winSizeY);
    initMatrices();
    
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
  window1 = glfwCreateWindow(winSizeX, winSizeY, "CS475/CS675 Demo: Matrix Transformations", NULL, NULL);
  if (!window1)
    {
      glfwTerminate();
      return -1;
    }

  int winXpos, winYpos;
  glfwGetWindowPos(window1, &winXpos, &winYpos);

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

  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window1, csX75::framebuffer_size_callback);

  //Window resize callback
  glfwSetWindowSizeCallback(window1, window_size_callback);
  

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window1, GLFW_STICKY_KEYS, GL_TRUE);
  

  //Initialize GL state
  csX75::initGL();
  initBuffersGL(WIN_TRANSFORM);
  initMatrices();

  std::cout<<"------------------------------" <<std::endl;
  

   // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;


  // Setup Dear ImGui style
  //ImGui::StyleColorsDark();
  ImGui::StyleColorsClassic();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window1, true);
  //ImGui_ImplOpenGL3_Init(glsl_version);
  //std::cerr<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;
  //TODO figure out GLSL version automatically
  ImGui_ImplOpenGL3_Init("#version 130");


  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


  // Loop until the user closes the window
  while (glfwWindowShouldClose(window1) == 0 )// && glfwWindowShouldClose(window2) == 0)
    {
      // Render here
      
      glfwMakeContextCurrent(window1);

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      static float rx = 0.0f;
      static float ry = 0.0f;
      static float rz = 0.0f;
      static int counter = 0;

      ImGui::Begin("Transformation Customization");

      ImGui::Text("Rotation Construction:");   
      
      //ImGui::Checkbox("Another Window", &show_another_window);

      ImGui::SliderFloat("X-Rotation", &rx, -180.0f, 180.0f); // Edit 1 float using a slider from -180.0f to 180.0f
      ImGui::SliderFloat("Y-Rotation", &ry, -180.0f, 180.0f); // Edit 1 float using a slider from -180.0f to 180.0f
      ImGui::SliderFloat("Z-Rotation", &rz, -180.0f, 180.0f); // Edit 1 float using a slider from -180.0f to 180.0f
      //ImGui::Text("float val is : %f",f);
      rotate(rx,ry,rz);
      //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
      if (ImGui::Button("Reset Rotaion"))   {// Buttons return true when clicked (most widgets return true when edited/activated)
        rx=0.0f;
        ry=0.0f;
        rz=0.0f;
      }                    
        

    ImGui::Text("Translation Construction:");
    static float tx = 0.0f;
    static float ty = 0.0f;
    static float tz = 0.0f;

    ImGui::SliderFloat("X-Translation", &tx, -10.0f, 10.0f); 
    ImGui::SliderFloat("Y-Translation", &ty, -10.0f, 10.0f); 
    ImGui::SliderFloat("Z-Translation", &tz, -10.0f, 10.0f); 
    //ImGui::Text("float val is : %f",f);
    translate(tx,ty,tz);
    //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
    if (ImGui::Button("Reset Translation"))   {// Buttons return true when clicked (most widgets return true when edited/activated)
      tx=0.0f;
      ty=0.0f;
      tz=0.0f;
    }     

    ImGui::Text("Scaling Construction:");
    static float sx = 1.0f;
    static float sy = 1.0f;
    static float sz = 1.0f;

    ImGui::SliderFloat("X-Scaling", &sx, 0.2f, 2.0f); 
    ImGui::SliderFloat("Y-Scaling", &sy, 0.2f, 2.0f); 
    ImGui::SliderFloat("Z-Scaling", &sz, 0.2f, 2.0f); 
    //ImGui::Text("float val is : %f",f);
    scale(sx,sy,sz);
    //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
    if (ImGui::Button("Reset Scaling"))   {// Buttons return true when clicked (most widgets return true when edited/activated)
      sx=1.0f;
      sy=1.0f;
      sz=1.0f;
    }

    ImGui::Text("Transformation Order:");
    static int tord = 0; //transformOrder
    ImGui::RadioButton("S then R then T", &tord, 0); ImGui::SameLine();
    ImGui::RadioButton("S then T then R", &tord, 1); //ImGui::SameLine();
    ImGui::RadioButton("R then T then S", &tord, 2); ImGui::SameLine();
    ImGui::RadioButton("T then R then S", &tord, 3);
    switch(tord){
      case 0: model_matrix = translation_matrix*rotation_matrix*scaling_matrix; //SRT
      break;
      case 1: model_matrix = rotation_matrix*translation_matrix*scaling_matrix; //STR
      break;
      case 2: model_matrix = scaling_matrix*translation_matrix*rotation_matrix; //RTS
      break;
      case 3: model_matrix = scaling_matrix*rotation_matrix*translation_matrix;  //TRS

    }

    ImGui::Text("Projection:");
    //static int projectType = 1;
    ImGui::RadioButton("Orthographic", &projectionType, 0); ImGui::SameLine();
    ImGui::RadioButton("Perspective", &projectionType, 1);
    switch(projectionType){
      case 0: projection_matrix = ortho_matrix;
      break;
      case 1: projection_matrix = perspective_matrix; //STR

    }

 
    ImGui::End();

    printMat4("Rotation Matrix",rotation_matrix);
    printMat4("Translation Matrix",translation_matrix);
    printMat4("Scaling Matrix",scaling_matrix);
    printMat4("Model Matrix", model_matrix);







      renderGL(WIN_TRANSFORM);

      // Swap front and back buffers
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      glfwSwapBuffers(window1);
      

      // Poll for and process events
      glfwPollEvents();
    }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  
  glfwTerminate();
  return 0;
}

//-------------------------------------------------------------------------

