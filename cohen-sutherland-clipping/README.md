# Demo: Cohen-sutherland line clipping and Sutherland-Hodgman polygon clipping

<br>
<br>

## About

In this demo we clip lines and edges of polygons using Cohen-sutherland and Sutherland-Hodgman algorithms, respectively. The lines and edges are defined by the mouse-clicks on the screen. In case of polygon clipping, the edges of the polygon along the clipper edges are completed accordingly.

<br>
<br>

## Running the code
Same as Tutorial_01, compilation of the code can be done using `make`.
If you have a driver supporting OpenGL 4.1 then running `clipping_cs` will do the trick for you. But if your system supports OpenGL 3.2+. Then in order to run the code the following changes need to be made in the **cipping_cs.cpp**. In the main function,

```cpp
glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4);
glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1);
```

you need to change to

```cpp
glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3);
```

Also in the **clipping_cs_fshader.glsl** and **clipping_cs_vshader.glsl** you need to modify the first line to
```cpp
# version 330
```

Once you make the above changes you can compile and run the bresenham file.

<br>
<br>


## Keyboard Input

In the `key_callback()` function in **gl_framework.cpp** we deal with various keyboard inputs. Mouse callbacks work in a similar fashion.

<br>
<br>


## Output

The following key/mouse controls are used for interaction:

* '1': Left mouse click - To add points corresponding to line/polygon.
* '2': P - To change the clipping primitives between line and polygon. By default, it is line.
* '3': C - To start clipping of lines/polygon.

## Reference

* '1': https://www.geeksforgeeks.org/line-clipping-set-1-cohen-sutherland-algorithm/
* '2': https://www.geeksforgeeks.org/polygon-clipping-sutherland-hodgman-algorithm-please-change-bmp-images-jpeg-png/
* '3': https://github.com/paragchaudhuri/cs475-tutorials/tree/master/Tutorial_02

