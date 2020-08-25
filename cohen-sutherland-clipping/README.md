# Demo: Cohen-sutherland line clipping and Sutherland-Hodgman polygon clipping

<br>
<br>

## About

In this demo we clip lines and edges of polygons using Cohen-sutherland and Sutherland-Hodgman algorithms, respectively. The lines and edges are defined by the mouse-clicks on the screen. In case of polygon clipping, the edges of the polygon along the clipper edges are completed accordingly.

This code uses OpenGL 3.3
<br>
<br>

## Running the code
Compilation of the code can be done using `make`. Type `./clipping_cs` to run the code.


<br>
<br>


## Keyboard Input

In the `key_callback()` function in **gl_framework.cpp** we deal with various keyboard inputs. Mouse callbacks work in a similar fashion.

<br>
<br>


## Output

The following key/mouse controls are used for interaction:

* Left mouse click - To add points corresponding to line/polygon.
* P - To change the clipping primitives between line and polygon. By default, it is line.
* C - To start clipping of lines/polygon.

## Reference

* https://www.geeksforgeeks.org/line-clipping-set-1-cohen-sutherland-algorithm/
* https://www.geeksforgeeks.org/polygon-clipping-sutherland-hodgman-algorithm-please-change-bmp-images-jpeg-png/
* https://github.com/paragchaudhuri/cs475-tutorials/tree/master/Tutorial_02

