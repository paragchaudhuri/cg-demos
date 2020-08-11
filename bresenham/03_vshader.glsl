#version 440

in vec4 vPosition;
in vec4 vColor;
out vec4 color;


void main (void) 
{
  gl_Position =  vPosition;
  color = vColor;
}
