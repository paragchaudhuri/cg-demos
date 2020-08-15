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


## Keyboard Input

In the `key_callback()` function in **gl_framework.cpp** we deal with various keyboard inputs. You can see that here we increment/decrement our rotation parameters whenever their respective key is pressed. Mouse callbacks work in a similar fashion.

<br>
<br>


## Output

The window on the right displays the line corresponding to the points specified in the window on the left using GL_LINES. This GL_LINES version is colored purple. The left window is responsible for displaying the lines using bresenham's algorithm. Press the following keys to activate the modes corresponding to the specific implementaions/extensions of bresenham's algorithm:

* '1' : Float version, 1 octant only
* '2' : Float version, all 8 octants
* '3' : Integer version, 1 octant only
* '4' : Integer version, all 8 octants


