# Demo: Bresenham's Line Drawing Algorithm

<br>
<br>

## About

In this tutorial we draw a line by first figuring out the pixels to plot as GL_POINTS using Brensenham's Algorithm covered in the lectures. In a separate window we see how the same is done with GL_LINES provided by OpenGL. Additionallly we also implement the "All-integer optimization" of Bresenham's ALgorithm and extend both float and int versions to work in all octants.

The code uses OpenGL 3.3. 
<br>
<br>

## Running the code
Compilation of the code can be done using `make`. Type `./bresenham` to execute.


<br>
<br>


## Keyboard Input

In the `key_callback()` function in **gl_framework.cpp** we deal with various keyboard inputs. Mouse callbacks work in a similar fashion.

<br>
<br>


## Output

The window on the right displays the line corresponding to the points specified in the window on the left using GL_LINES. This GL_LINES version is colored purple. The left window is responsible for displaying the lines using bresenham's algorithm. Press the following keys to activate the modes corresponding to the specific implementaions/extensions of bresenham's algorithm:

* '1' : Float version, 1 octant only
* '2' : Float version, all 8 octants
* '3' : Integer version, 1 octant only
* '4' : Integer version, all 8 octants


