# Demo: Bresenham's Line Drawing Algorithm

<br>
<br>

## About

In this tutorial we draw a line by first figuring out the pixels to plot as GL_POINTS using Brensenham's Algorithm covered in the lectures. In a separate window we see how the same is done with GL_LINES provided by OpenGL. Additionallly we also implement the "All-integer optimization" of Bresenham's ALgorithm and extend both float and int versions to work in all octants.

<br>
<br>

## Running the code
Same as Tutorial_01, compilation of the code can be done using `make`.
If you have a driver supporting OpenGL 4.1 then running `bresenham` will do the trick for you. But if your system supports OpenGL 3.2+. Then in order to run the code the following changes need to be made in the **bresenham.cpp**. In the main function,

```cpp
glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4);
glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1);
```

you need to change to

```cpp
glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3);
```

Also in the **bh_fshader.glsl** and **bh_vshader.glsl** you need to modify the first line to
```cpp
# version 330
```

Once you make the above changes you can compile and run the bresenham file.

<br>
<br>

## Shaders

The next piece of code void *initBuffersGL(void)*, is essentially what were the *initShadersGL()* and *initVertexBufferGL()* in the Tutorial_01.

```cpp
void initBuffersGL(void)
{
  colorcube();

  //Ask GL for a Vertex Attribute Object (vao)
  glGenVertexArrays (1, &vao);
  //Set it as the current array to be used by binding it
  glBindVertexArray (vao);

  //Ask GL for a Vertex Buffer Object (vbo)
  glGenBuffers (1, &vbo);
  //Set it as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER, sizeof (v_positions) + sizeof(v_colors), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(v_positions), v_positions );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(v_positions), sizeof(v_colors), v_colors );

  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("02_vshader.glsl");
  std::string fragment_shader_file("02_fshader.glsl");

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
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions)) );
}
```

We initially call the colorcube() method to populate the two vec4 arrays
*v_positions* and *v_colors*. After generating vao and vbo, we create a buffer of size *(v_positions + v_colors)*. (Since our buffer is going to hold the color data as well as the position data for each vertex). In the next two lines,

```cpp
glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(v_positions), v_positions );
glBufferSubData( GL_ARRAY_BUFFER, sizeof(v_positions), sizeof(v_colors), v_colors );
```

We fill up the buffer using the two types of sub data, position and color. The syntax and semantics of these functions are already explained in Tutorial_01.

After this, we load the shaders and push them back into shaderlist vector, for the reference to the shaders.

Then we specify how position and color data are stored in *VBO* using the `vao`, now as you remember, vertex attrib pointer, *VAO*, describes how vertex attributes are stored in the vertex buffer object, *VBO*. But, in this case we have two things that are to be stored in *VBO*, colors and positions. So, first we get the location of the parameter(*vPosition* and *vColor*), using `glGetAttribLocation()`. As, you can see that we have defined the same variables in the shader as well. If you remember, we mentioned in the tutorial 0 that the declaration of *vPosition* and *vColor*, in shader code may also be done using `layout(location = 0) in vec4 vp;`, the return value of `glGetAttribLocation()`, is the same location value from the shader program. Now, we can use *glVertexAttribPointer*, using the location for color and position pointers.

### Vertex Shader

```cpp
attribute vec4 vPosition;
attribute vec4 vColor;
varying vec4 color;
```

Unlike the first Tutorial, here instead of only specifying *gl_Postion*, we are also providing the color using the attributes *vPosition* and *vColor*, these attributes are the ones referred by `glGetAttribLocation()`.

### Fragment Shader

The fragment shader is quite similar to the code previous tutorial and you can easily see that we are assigning the color of the fragment, from our input from vertex shader.


<br>
<br>

## Keyboard Input

In the `key_callback()` function in **gl_framework.cpp** we deal with various keyboard inputs. You can see that here we increment/decrement our rotation parameters whenever their respective key is pressed.
