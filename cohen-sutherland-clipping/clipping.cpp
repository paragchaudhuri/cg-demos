/****************************************************************************************
 *				CS699 Individual Project				*
 *     Implementation of Cohen-Sutherland Clipping Algorithm with OpenGL Animation	*
 *					Developed By					*
 *				    Sarbartha Sengupta					*
 *					M.Tech 1 CSE					*
 *					10305903					*
 ****************************************************************************************/

#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <GL/glut.h>

float win_width = 512; // Width of the window
float win_height = 512; // height of the window
int xx0, yy0; //coordinates of the user click1
int xx1, yy1; //coordinates of the user click2
int numpoints=0; //point count (odd or even)
float points[100][2][2]; //storing all the points
float pointcolor[100][3]; // storing the colours of the points
int p=0; // no of points
float W = win_width; 
float H = win_height;
float L = win_width / 3; // Left bound of the viewport
float R = 2 * (win_width / 3);// Right bound of the viewport
float T = 2 * (win_height / 3);// Top bound of the viewport
float B = win_height / 3;// Bottom bound of the viewport
bool clipping = true; // clipping stould start or not
bool poly=false; // if it polygon clipping or not
float delay = 1.0; // delay in showing the output
void swap (int &x, int &y); 
void display(void); // display the screen
void reshape(int w, int h); //reshape the screen
void keyboard(unsigned char key, int x, int y); // take the keyboard input
void mouse(int button, int state, int x, int y); // take the mouse input




//Drawing Viwe Port


void viewport (void)
{


	
	glPointSize(4.0);
	glBegin(GL_LINES);
		glColor3f(0.0,0.0,1.0);
		glVertex2f(0.0f,B);
		glVertex2f(W,B);
	glEnd();
	glBegin(GL_LINES);
		glColor3f(0.0,0.0,1.0);
		glVertex2f(0.0f,T);
		glVertex2f(W,T);
	glEnd();
	glBegin(GL_LINES);
		glColor3f(0.0,0.0,1.0);
		glVertex2f(R,0.0f);
		glVertex2f(R,H);
	glEnd();
	glBegin(GL_LINES);
		glColor3f(0.0,0.0,1.0);
		glVertex2f(L,0.0f);
		glVertex2f(L,H);
	glEnd();
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(L,B);
		glVertex2f(R,B);
	glEnd();
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(L,T);
		glVertex2f(R,T);
	glEnd();
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(L,B);
		glVertex2f(L,T);
	glEnd();
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(R,B);
		glVertex2f(R,T);
	glEnd();


}
//Drawing the lines in the viewport
void drawlines()
{
	int i;	
	
	glBegin(GL_LINES);
	for(i=0;i<p;i++)
	{
		glColor3f(pointcolor[i][0],pointcolor[i][1],pointcolor[i][2]);
		glVertex2f(points[i][0][0],points[i][0][1]);
		glVertex2f(points[i][1][0],points[i][1][1]);
	}
	glEnd();
	if ((numpoints % 2) == 1)
	{
		glColor3f(pointcolor[p][0],pointcolor[p][1],pointcolor[p][2]);
		glBegin(GL_POINTS);
			glVertex2f(points[p][0][0],points[p][0][1]);
		glEnd();
	}
}


void swap (int &x, int &y)
{
  int temp = x;
  x = y; y = temp;
}


//Display everything on the window
void display(void)
{
 glClear( GL_COLOR_BUFFER_BIT );

	glLineWidth(2.0);
	viewport();
	glLineWidth(4.0);	
	drawlines();

  glutSwapBuffers();
}
//reshape the window
void reshape(int w, int h)
{
  if  ( h == 0 ) h = 1;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho( 0.0, (GLdouble)w, 0.0, (GLdouble)h, -1., 1. );
  glViewport( 0, 0, w, h );

  win_width = w;
  win_height = h;

  glutPostRedisplay();
}
// This function clips a polygon
void pclip()
{
	int i,j,jl,j1,p1=p,il,i1;
	float x,y,x1,y1,x2,y2;
	// Clipping the left side of the viewport
	for(i=0;i<p;i++)
	{
		i1=i;
		if(points[i][0][0]<L) // chking whether the full line is outside the viewport o not
		{
			if(points[i][1][0]<L || points[(i+p-1)%p][0][0] <L)
			{
				x=L; 
				p=p1;
				il=p;
				while(points[i][1][0]<L)
				{
						points[p][1][0]=points[i][1][0];
						points[p][1][1]=points[i][1][1];
						points[p][0][0]=points[i][0][0];
						points[p][0][1]=points[i][0][1];
						points[i][1][0]=L; //nullifying the unwanted lines
						points[i][1][1]=B;
						points[i][0][0]=L;
						points[i][0][1]=B;
						// changing the colour to red for unwanted line
						pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0; 
		
						p++;
						il++;	
						display();
						sleep(delay);
						points[p-1][1][0]=L; // deleting the unwanted red line
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
						if(i==il)
							i=0;
						else
							i++;
				}
				x1=points[i][0][0];
				y1=points[i][0][1];
				x2=points[i][1][0];
				y2=points[i][1][1];
				y=y1+((x-x1)*(y1-y2))/(x1-x2); // calculating the intersection point
				points[p][0][0]=points[i][0][0];
				points[p][0][1]=points[i][0][1];
				points[p][1][0]=x;
				points[p][1][1]=y;
				points[i][0][0]=x;
				points[i][0][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
				j= (i1+p-2)%(p-1);
				p++;
				display();
				sleep(delay);
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
				if(points[j][0][0]<L)
				{
					jl=p,j1=j;
					while(points[j][0][0]<L)
					{
						points[p][1][0]=points[j][1][0];
						points[p][1][1]=points[j][1][1];
						points[p][0][0]=points[j][0][0];
						points[p][0][1]=points[j][0][1];
						points[j][1][0]=L;
						points[j][1][1]=B;
						points[j][0][0]=L;
						points[j][0][1]=B;
						pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
						p++;	
						display();
						sleep(delay);
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
						jl++;
						if(j!=0)
							j--;
						else
							j=jl;
					}
				}
				x1=points[j][0][0];
				y1=points[j][0][1];
				x2=points[j][1][0];
				y2=points[j][1][1];
				y=y1+((x-x1)*(y1-y2))/(x1-x2);
				points[p][1][0]=points[j][1][0];
				points[p][1][1]=points[j][1][1];
				points[p][0][0]=x;
				points[p][0][1]=y;
				points[j][1][0]=x;
				points[j][1][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;

				p++;	
				display();
				sleep(delay);
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
				p=p1;
				points[j+1][0][0]=x;
				points[j+1][0][1]=y;
				points[j+1][1][0]=points[i][0][0];
				points[j+1][1][1]=points[i][0][1];

				pointcolor[p][0]=0.5;pointcolor[p][1]=0.5;pointcolor[p][2]=1.0;
				points[p][0][0]=points[p][1][0];
				points[p][0][1]=points[p][1][1];
				points[p][1][0]=points[j][1][0];
				points[p][1][1]=points[j][1][1];
				points[p][0][0]=x;
				points[p][0][1]=y;
				points[j][1][0]=x;
				points[j][1][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;

				p++;	
				display();
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
				//p++;
				points[p][0][0]=points[j][1][0];
				points[p][0][1]=points[j][1][1];
				points[p][1][0]=points[i][0][0];
				points[p][1][1]=points[i][0][1];
				pointcolor[p][0]=0.5;pointcolor[p][1]=0.5;pointcolor[p][2]=0.0;
				p++;
				p1++;
				display();
				sleep(delay);		
			}
			else
			{
				x=L; 
				x1=points[i][0][0];
				y1=points[i][0][1];
				x2=points[i][1][0];
				y2=points[i][1][1];
				y=y1+((x-x1)*(y1-y2))/(x1-x2);
				points[p][0][0]=points[i][0][0];
				points[p][0][1]=points[i][0][1];
				points[p][1][0]=x;
				points[p][1][1]=y;
				points[i][0][0]=x;
				points[i][0][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
				j= (i+p-1)%p;
				p++;
				display();
				sleep(delay);
				p--;
				x1=points[j][0][0];
				y1=points[j][0][1];
				x2=points[j][1][0];
				y2=points[j][1][1];
				y=y1+((x-x1)*(y1-y2))/(x1-x2);
				if(points[j][0][0]<L)
				{
					points[p][1][0]=points[j][1][0];
					points[p][1][1]=points[j][1][1];
					points[p][0][0]=points[j][0][0];
					points[p][0][1]=points[j][0][1];
						points[j][1][0]=L;
						points[j][1][1]=B;
						points[j][0][0]=L;
						points[j][0][1]=B;
					pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
	
					p++;	
					display();
					sleep(delay);
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
					points[j][0][0]=points[(j+p-1)%p][1][0];
					points[j][0][1]=points[(j+p-1)%p][1][1];
					points[j][1][0]=points[i][0][0];
					points[j][1][1]=points[i][0][1];
					pointcolor[p][0]=0.5;pointcolor[p][1]=0.5;pointcolor[p][2]=0.0;
					p++;					
				}
				else
				{
					points[p][1][0]=points[j][1][0];
					points[p][1][1]=points[j][1][1];
					points[p][0][0]=x;
					points[p][0][1]=y;
					points[j][1][0]=x;
					points[j][1][1]=y;
					pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
	
					p++;	
					display();
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
					sleep(delay);
					points[p][0][0]=points[j][1][0];
					points[p][0][1]=points[j][1][1];
					points[p][1][0]=points[i][0][0];
					points[p][1][1]=points[i][0][1];
					pointcolor[p][0]=0.5;pointcolor[p][1]=0.5;pointcolor[p][2]=0.0;
					p++;
					p1++;
				}
				display();
				sleep(delay);		
				p1++;
			}			
		}

	}
	p=p1;
	//Clipping the Bottom side of the viewport
	for(i=0;i<p;i++)
	{
		i1=i;
		if(points[i][0][1]<B)
		{
			if(points[i][1][1]<B || points[(i+p-1)%p][0][1] <B)
			{
				y=B; 
				p=p1;
				il=p;
				while(points[i][1][1]<B)
				{
						points[p][1][0]=points[i][1][0];
						points[p][1][1]=points[i][1][1];
						points[p][0][0]=points[i][0][0];
						points[p][0][1]=points[i][0][1];
						points[i][1][0]=L;
						points[i][1][1]=B;
						points[i][0][0]=L;
						points[i][0][1]=B;

						pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
		
						p++;
						il++;	
						display();
						sleep(delay);
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
						if(i==il)
							i=0;
						else
							i++;
				}
				x1=points[i][0][0];
				y1=points[i][0][1];
				x2=points[i][1][0];
				y2=points[i][1][1];
				x=x1+((y-y1)*(x1-x2))/(y1-y2);
				points[p][0][0]=points[i][0][0];
				points[p][0][1]=points[i][0][1];
				points[p][1][0]=x;
				points[p][1][1]=y;
				points[i][0][0]=x;
				points[i][0][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
				j= (i1+p-2)%(p-1);
				p++;
				display();
				sleep(delay);
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
				if(points[j][0][1]<B)
				{
					jl=p,j1=j;
					while(points[j][0][1]<B)
					{
						points[p][1][0]=points[j][1][0];
						points[p][1][1]=points[j][1][1];
						points[p][0][0]=points[j][0][0];
						points[p][0][1]=points[j][0][1];
						points[j][1][0]=L;
						points[j][1][1]=B;
						points[j][0][0]=L;
						points[j][0][1]=B;
						pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
						p++;	
						display();
						sleep(delay);
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
						jl++;
						if(j!=0)
							j--;
						else
							j=jl;
					}
				}
				x1=points[j][0][0];
				y1=points[j][0][1];
				x2=points[j][1][0];
				y2=points[j][1][1];
				x=x1+((y-y1)*(x1-x2))/(y1-y2);
				points[p][1][0]=points[j][1][0];
				points[p][1][1]=points[j][1][1];
				points[p][0][0]=x;
				points[p][0][1]=y;
				points[j][1][0]=x;
				points[j][1][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;

				p++;	
				display();
				sleep(delay);
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
				p=p1;
				points[j+1][0][0]=x;
				points[j+1][0][1]=y;
				points[j+1][1][0]=points[i][0][0];
				points[j+1][1][1]=points[i][0][1];

				pointcolor[p][0]=0.5;pointcolor[p][1]=0.5;pointcolor[p][2]=1.0;
				points[p][0][0]=points[p][1][0];
				points[p][0][1]=points[p][1][1];
				points[p][1][0]=points[j][1][0];
				points[p][1][1]=points[j][1][1];
				points[p][0][0]=x;
				points[p][0][1]=y;
				points[j][1][0]=x;
				points[j][1][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;

				p++;	
				display();
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
				//p++;
				points[p][0][0]=points[j][1][0];
				points[p][0][1]=points[j][1][1];
				points[p][1][0]=points[i][0][0];
				points[p][1][1]=points[i][0][1];
				pointcolor[p][0]=0.5;pointcolor[p][1]=0.5;pointcolor[p][2]=0.0;
				p++;
				p1++;
				display();
				sleep(delay);		
			}
			else
			{
				y=B; 
				x1=points[i][0][0];
				y1=points[i][0][1];
				x2=points[i][1][0];
				y2=points[i][1][1];
				x=x1+((y-y1)*(x1-x2))/(y1-y2);
				points[p][0][0]=points[i][0][0];
				points[p][0][1]=points[i][0][1];
				points[p][1][0]=x;
				points[p][1][1]=y;
				points[i][0][0]=x;
				points[i][0][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
				j= (i+p-1)%p;
				p++;
				display();
				sleep(delay);
				p--;
				x1=points[j][0][0];
				y1=points[j][0][1];
				x2=points[j][1][0];
				y2=points[j][1][1];
				x=x1+((y-y1)*(x1-x2))/(y1-y2);
				if(points[j][0][1]<B)
				{
					points[p][1][0]=points[j][1][0];
					points[p][1][1]=points[j][1][1];
					points[p][0][0]=points[j][0][0];
					points[p][0][1]=points[j][0][1];
						points[j][1][0]=L;
						points[j][1][1]=B;
						points[j][0][0]=L;
						points[j][0][1]=B;
					pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
	
					p++;	
					display();
					sleep(delay);
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
					points[j][0][0]=points[(j+p-1)%p][1][0];
					points[j][0][1]=points[(j+p-1)%p][1][1];
					points[j][1][0]=points[i][0][0];
					points[j][1][1]=points[i][0][1];
					pointcolor[p][0]=0.5;pointcolor[p][1]=0.5;pointcolor[p][2]=0.0;
					p++;					
				}
				else
				{
					points[p][1][0]=points[j][1][0];
					points[p][1][1]=points[j][1][1];
					points[p][0][0]=x;
					points[p][0][1]=y;
					points[j][1][0]=x;
					points[j][1][1]=y;
					pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
	
					p++;	
					display();
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
					sleep(delay);
					points[p][0][0]=points[j][1][0];
					points[p][0][1]=points[j][1][1];
					points[p][1][0]=points[i][0][0];
					points[p][1][1]=points[i][0][1];
					pointcolor[p][0]=0.5;pointcolor[p][1]=0.5;pointcolor[p][2]=0.0;
					p++;
					p1++;
				}
				display();
				sleep(delay);		
				p1++;
			}			
		}

	}
	p=p1;
	//Clipping the right side of the viewport
	for(i=0;i<p;i++)
	{
		i1=i;
		if(points[i][0][0]>R)
		{
			if(points[i][1][0]>R || points[(i+p-1)%p][0][0] >R)
			{
				x=R; 
				p=p1;
				il=p;
				while(points[i][1][0]>R)
				{
						points[p][1][0]=points[i][1][0];
						points[p][1][1]=points[i][1][1];
						points[p][0][0]=points[i][0][0];
						points[p][0][1]=points[i][0][1];
						points[i][1][0]=L;
						points[i][1][1]=B;
						points[i][0][0]=L;
						points[i][0][1]=B;

						pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
		
						p++;
						il++;	
						display();
						sleep(delay);
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
						if(i==il)
							i=0;
						else
							i++;
				}
				x1=points[i][0][0];
				y1=points[i][0][1];
				x2=points[i][1][0];
				y2=points[i][1][1];
				y=y1+((x-x1)*(y1-y2))/(x1-x2);
				points[p][0][0]=points[i][0][0];
				points[p][0][1]=points[i][0][1];
				points[p][1][0]=x;
				points[p][1][1]=y;
				points[i][0][0]=x;
				points[i][0][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
				j= (i1+p-2)%(p-1);
				p++;
				display();
				sleep(delay);
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
				if(points[j][0][0]>R)
				{
					jl=p,j1=j;
					while(points[j][0][0]>R)
					{
						points[p][1][0]=points[j][1][0];
						points[p][1][1]=points[j][1][1];
						points[p][0][0]=points[j][0][0];
						points[p][0][1]=points[j][0][1];
						points[j][1][0]=L;
						points[j][1][1]=B;
						points[j][0][0]=L;
						points[j][0][1]=B;
						pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
						p++;	
						display();
						sleep(delay);
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
						jl++;
						if(j!=0)
							j--;
						else
							j=jl;
					}
				}
				x1=points[j][0][0];
				y1=points[j][0][1];
				x2=points[j][1][0];
				y2=points[j][1][1];
				y=y1+((x-x1)*(y1-y2))/(x1-x2);
				points[p][1][0]=points[j][1][0];
				points[p][1][1]=points[j][1][1];
				points[p][0][0]=x;
				points[p][0][1]=y;
				points[j][1][0]=x;
				points[j][1][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;

				p++;	
				display();
				sleep(delay);
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
				p=p1;
				points[j+1][0][0]=x;
				points[j+1][0][1]=y;
				points[j+1][1][0]=points[i][0][0];
				points[j+1][1][1]=points[i][0][1];

				pointcolor[p][0]=0.5;pointcolor[p][1]=0.5;pointcolor[p][2]=1.0;
				points[p][0][0]=points[p][1][0];
				points[p][0][1]=points[p][1][1];
				points[p][1][0]=points[j][1][0];
				points[p][1][1]=points[j][1][1];
				points[p][0][0]=x;
				points[p][0][1]=y;
				points[j][1][0]=x;
				points[j][1][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;

				p++;	
				display();
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
				//p++;
				points[p][0][0]=points[j][1][0];
				points[p][0][1]=points[j][1][1];
				points[p][1][0]=points[i][0][0];
				points[p][1][1]=points[i][0][1];
				pointcolor[p][0]=0.5;pointcolor[p][1]=0.5;pointcolor[p][2]=0.0;
				p++;
				p1++;
				display();
				sleep(delay);		
			}
			else
			{
				x=R; 
				x1=points[i][0][0];
				y1=points[i][0][1];
				x2=points[i][1][0];
				y2=points[i][1][1];
				y=y1+((x-x1)*(y1-y2))/(x1-x2);
				points[p][0][0]=points[i][0][0];
				points[p][0][1]=points[i][0][1];
				points[p][1][0]=x;
				points[p][1][1]=y;
				points[i][0][0]=x;
				points[i][0][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
				j= (i+p-1)%p;
				p++;
				display();
				sleep(delay);
				p--;
				x1=points[j][0][0];
				y1=points[j][0][1];
				x2=points[j][1][0];
				y2=points[j][1][1];
				y=y1+((x-x1)*(y1-y2))/(x1-x2);
				if(points[j][0][0]>R)
				{
					points[p][1][0]=points[j][1][0];
					points[p][1][1]=points[j][1][1];
					points[p][0][0]=points[j][0][0];
					points[p][0][1]=points[j][0][1];
						points[j][1][0]=L;
						points[j][1][1]=B;
						points[j][0][0]=L;
						points[j][0][1]=B;
					pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
	
					p++;	
					display();
					sleep(delay);
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
					points[j][0][0]=points[(j+p-1)%p][1][0];
					points[j][0][1]=points[(j+p-1)%p][1][1];
					points[j][1][0]=points[i][0][0];
					points[j][1][1]=points[i][0][1];
					pointcolor[p][0]=0.5;pointcolor[p][1]=0.5;pointcolor[p][2]=0.0;
					p++;					
				}
				else
				{
					points[p][1][0]=points[j][1][0];
					points[p][1][1]=points[j][1][1];
					points[p][0][0]=x;
					points[p][0][1]=y;
					points[j][1][0]=x;
					points[j][1][1]=y;
					pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
	
					p++;	
					display();
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
					sleep(delay);
					points[p][0][0]=points[j][1][0];
					points[p][0][1]=points[j][1][1];
					points[p][1][0]=points[i][0][0];
					points[p][1][1]=points[i][0][1];
					pointcolor[p][0]=0.5;pointcolor[p][1]=0.5;pointcolor[p][2]=0.0;
					p++;
					p1++;
				}
				display();
				sleep(delay);		
				p1++;
			}			
		}

	}
	p=p1;
	//Clipping the top side of the viewport
	for(i=0;i<p;i++)
	{
		i1=i;
		if(points[i][0][1]>T)
		{
			if(points[i][1][1]>T || points[(i+p-1)%p][0][1] >T)
			{
				y=T; 
				p=p1;
				il=p;
				while(points[i][1][1]>T)
				{
						points[p][1][0]=points[i][1][0];
						points[p][1][1]=points[i][1][1];
						points[p][0][0]=points[i][0][0];
						points[p][0][1]=points[i][0][1];
						points[i][1][0]=L;
						points[i][1][1]=B;
						points[i][0][0]=L;
						points[i][0][1]=B;

						pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
		
						p++;
						il++;	
						display();
						sleep(delay);
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
						if(i==il)
							i=0;
						else
							i++;
				}
				x1=points[i][0][0];
				y1=points[i][0][1];
				x2=points[i][1][0];
				y2=points[i][1][1];
				x=x1+((y-y1)*(x1-x2))/(y1-y2);
				points[p][0][0]=points[i][0][0];
				points[p][0][1]=points[i][0][1];
				points[p][1][0]=x;
				points[p][1][1]=y;
				points[i][0][0]=x;
				points[i][0][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
				j= (i1+p-2)%(p-1);
				p++;
				display();
				sleep(delay);
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
				if(points[j][0][1]>T)
				{
					jl=p,j1=j;
					while(points[j][0][1]>T)
					{
						points[p][1][0]=points[j][1][0];
						points[p][1][1]=points[j][1][1];
						points[p][0][0]=points[j][0][0];
						points[p][0][1]=points[j][0][1];
						points[j][1][0]=L;
						points[j][1][1]=B;
						points[j][0][0]=L;
						points[j][0][1]=B;
						pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
						p++;	
						display();
						sleep(delay);
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
						jl++;
						if(j!=0)
							j--;
						else
							j=jl;
					}
				}
				x1=points[j][0][0];
				y1=points[j][0][1];
				x2=points[j][1][0];
				y2=points[j][1][1];
				x=x1+((y-y1)*(x1-x2))/(y1-y2);
				points[p][1][0]=points[j][1][0];
				points[p][1][1]=points[j][1][1];
				points[p][0][0]=x;
				points[p][0][1]=y;
				points[j][1][0]=x;
				points[j][1][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;

				p++;	
				display();
				sleep(delay);
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
				p=p1;
				points[j+1][0][0]=x;
				points[j+1][0][1]=y;
				points[j+1][1][0]=points[i][0][0];
				points[j+1][1][1]=points[i][0][1];

				pointcolor[p][0]=0.5;pointcolor[p][1]=0.5;pointcolor[p][2]=1.0;
				points[p][0][0]=points[p][1][0];
				points[p][0][1]=points[p][1][1];
				points[p][1][0]=points[j][1][0];
				points[p][1][1]=points[j][1][1];
				points[p][0][0]=x;
				points[p][0][1]=y;
				points[j][1][0]=x;
				points[j][1][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;

				p++;	
				display();
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
				//p++;
				points[p][0][0]=points[j][1][0];
				points[p][0][1]=points[j][1][1];
				points[p][1][0]=points[i][0][0];
				points[p][1][1]=points[i][0][1];
				pointcolor[p][0]=0.5;pointcolor[p][1]=0.5;pointcolor[p][2]=0.0;
				p++;
				p1++;
				display();
				sleep(delay);		
			}
			else
			{
				y=T; 
				x1=points[i][0][0];
				y1=points[i][0][1];
				x2=points[i][1][0];
				y2=points[i][1][1];
				x=x1+((y-y1)*(x1-x2))/(y1-y2);
				points[p][0][0]=points[i][0][0];
				points[p][0][1]=points[i][0][1];
				points[p][1][0]=x;
				points[p][1][1]=y;
				points[i][0][0]=x;
				points[i][0][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
				j= (i+p-1)%p;
				p++;
				display();
				sleep(delay);
				p--;
				x1=points[j][0][0];
				y1=points[j][0][1];
				x2=points[j][1][0];
				y2=points[j][1][1];
				x=x1+((y-y1)*(x1-x2))/(y1-y2);
				if(points[j][0][1]>T)
				{
					points[p][1][0]=points[j][1][0];
					points[p][1][1]=points[j][1][1];
					points[p][0][0]=points[j][0][0];
					points[p][0][1]=points[j][0][1];
						points[j][1][0]=L;
						points[j][1][1]=B;
						points[j][0][0]=L;
						points[j][0][1]=B;
					pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
	
					p++;	
					display();
					sleep(delay);
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
					points[j][0][0]=points[(j+p-1)%p][1][0];
					points[j][0][1]=points[(j+p-1)%p][1][1];
					points[j][1][0]=points[i][0][0];
					points[j][1][1]=points[i][0][1];
					pointcolor[p][0]=0.5;pointcolor[p][1]=0.5;pointcolor[p][2]=0.0;
					p++;					
				}
				else
				{
					points[p][1][0]=points[j][1][0];
					points[p][1][1]=points[j][1][1];
					points[p][0][0]=x;
					points[p][0][1]=y;
					points[j][1][0]=x;
					points[j][1][1]=y;
					pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
	
					p++;	
					display();
						points[p-1][1][0]=L;
						points[p-1][1][1]=B;
						points[p-1][0][0]=L;
						points[p-1][0][1]=B;
					sleep(delay);
					points[p][0][0]=points[j][1][0];
					points[p][0][1]=points[j][1][1];
					points[p][1][0]=points[i][0][0];
					points[p][1][1]=points[i][0][1];
					pointcolor[p][0]=0.5;pointcolor[p][1]=0.5;pointcolor[p][2]=0.0;
					p++;
					p1++;
				}
				display();
				sleep(delay);		
				p1++;
			}			
		}

	}

}
// This function is used for line clipping
void clip()
{
		int i,j,t=0;
	if(poly)
	{
		pclip();
		for(i=0;i<p;i++)
		{
			pointcolor[i][0]=0.0;pointcolor[i][1]=1.0;pointcolor[i][2]=0.0;
		}
		display();

		return;
	}

	float x,y,x1,y1,x2,y2;
	for(i=0;i<=p;i++)
	{
		if(t==2){i--;}
		t=0;
		pointcolor[i][0]=0.0;pointcolor[i][1]=0.0;pointcolor[i][2]=1.0;
		display();
		sleep(delay);
		if(points[i][0][0]>=L && points[i][0][1]>=B && points[i][0][0]<=R && points[i][0][1]<=T)
		{
			if(points[i][1][0]>=L && points[i][1][1]>=B && points[i][1][0]<=R && points[i][1][1]<=T)
			{
				pointcolor[i][0]=0.0;pointcolor[i][1]=1.0;pointcolor[i][2]=0.0;
				t=1;
			}
		}
		if((points[i][0][0]<=L && points[i][1][0]<=L) || (points[i][0][0]>=R && points[i][1][0]>=R))
		{
			pointcolor[i][0]=1.0;pointcolor[i][1]=0.0;pointcolor[i][2]=0.0;
			display();
			sleep(delay);
			for(j=0;j<2;j++)
			{
				points[i][0][j]=points[i][1][j];
				
			}
			t=1;
		}
		if((points[i][0][1]<=B && points[i][1][1]<=B) || (points[i][0][1]>=T && points[i][1][1]>=T))
		{
			pointcolor[i][0]=1.0;pointcolor[i][1]=0.0;pointcolor[i][2]=0.0;
			display();
			sleep(delay);
			for(j=0;j<2;j++)
			{
				points[i][0][j]=points[i][1][j];
			}
			t=1;
		}
		if(t==0)
		{

			if(points[i][0][0]<L && t==0)
			{
				x=L; 
				x1=points[i][0][0];
				y1=points[i][0][1];
				x2=points[i][1][0];
				y2=points[i][1][1];
				y=y1+((x-x1)*(y1-y2))/(x1-x2);
				points[p][0][0]=points[i][0][0];
				points[p][0][1]=points[i][0][1];
				points[p][1][0]=x;
				points[p][1][1]=y;
				points[i][0][0]=x;
				points[i][0][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
				p++;
				display();
				sleep(delay);
				p--;
				points[p][0][0]=x;
				points[p][0][1]=y;
				display();
				sleep(delay);
				t=2;
				continue;
			}
			if(points[i][1][0]<L && t==0)
			{
				x=L; 
				x1=points[i][0][0];
				y1=points[i][0][1];
				x2=points[i][1][0];
				y2=points[i][1][1];
				y=y1+((x-x1)*(y1-y2))/(x1-x2);
				points[p][1][0]=points[i][1][0];
				points[p][1][1]=points[i][1][1];
				points[p][0][0]=x;
				points[p][0][1]=y;
				points[i][1][0]=x;
				points[i][1][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
				p++;
				display();
				sleep(delay);
				p--;
				points[p][1][0]=x;
				points[p][1][1]=y;
				display();
				sleep(delay);
				t=2;
			}
			if(points[i][0][1]<B && t==0)
			{
				y=B; 
				x1=points[i][0][0];
				y1=points[i][0][1];
				x2=points[i][1][0];
				y2=points[i][1][1];
				x=x1+((y-y1)*(x1-x2))/(y1-y2);
				points[p][0][0]=points[i][0][0];
				points[p][0][1]=points[i][0][1];
				points[p][1][0]=x;
				points[p][1][1]=y;
				points[i][0][0]=x;
				points[i][0][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
				p++;
				display();
				sleep(delay);
				p--;
				points[p][0][0]=x;
				points[p][0][1]=y;
				display();
				sleep(delay);
				t=2;
			}
			if(points[i][1][1]<B && t==0)
			{
				y=B; 
				x1=points[i][0][0];
				y1=points[i][0][1];
				x2=points[i][1][0];
				y2=points[i][1][1];
				x=x1+((y-y1)*(x1-x2))/(y1-y2);
				points[p][1][0]=points[i][1][0];
				points[p][1][1]=points[i][1][1];
				points[p][0][0]=x;
				points[p][0][1]=y;
				points[i][1][0]=x;
				points[i][1][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
				p++;
				display();
				sleep(delay);
				p--;
				points[p][1][0]=x;
				points[p][1][1]=y;
				display();
				sleep(delay);
				t=2;
			}
			if(points[i][0][0]>R && t==0)
			{
				x=R; 
				x1=points[i][0][0];
				y1=points[i][0][1];
				x2=points[i][1][0];
				y2=points[i][1][1];
				y=y1+((x-x1)*(y1-y2))/(x1-x2);
				points[p][0][0]=points[i][0][0];
				points[p][0][1]=points[i][0][1];
				points[p][1][0]=x;
				points[p][1][1]=y;
				points[i][0][0]=x;
				points[i][0][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
				p++;
				display();
				sleep(delay);
				p--;
				points[p][0][0]=x;
				points[p][0][1]=y;
				display();
				sleep(delay);
				t=2;
				continue;
			}
			if(points[i][1][0]>R && t==0)
			{
				x=R; 
				x1=points[i][0][0];
				y1=points[i][0][1];
				x2=points[i][1][0];
				y2=points[i][1][1];
				y=y1+((x-x1)*(y1-y2))/(x1-x2);
				points[p][1][0]=points[i][1][0];
				points[p][1][1]=points[i][1][1];
				points[p][0][0]=x;
				points[p][0][1]=y;
				points[i][1][0]=x;
				points[i][1][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
				p++;
				display();
				sleep(delay);
				p--;
				points[p][1][0]=x;
				points[p][1][1]=y;
				display();
				sleep(delay);
				t=2;
			}
			if(points[i][0][1]>T && t==0)
			{
				y=T; 
				x1=points[i][0][0];
				y1=points[i][0][1];
				x2=points[i][1][0];
				y2=points[i][1][1];
				x=x1+((y-y1)*(x1-x2))/(y1-y2);
				points[p][0][0]=points[i][0][0];
				points[p][0][1]=points[i][0][1];
				points[p][1][0]=x;
				points[p][1][1]=y;
				points[i][0][0]=x;
				points[i][0][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
				p++;
				display();
				sleep(delay);
				p--;
				points[p][0][0]=x;
				points[p][0][1]=y;
				display();
				sleep(delay);
				t=2;
			}
			if(points[i][1][1]>T && t==0)
			{
				y=T; 
				x1=points[i][0][0];
				y1=points[i][0][1];
				x2=points[i][1][0];
				y2=points[i][1][1];
				x=x1+((y-y1)*(x1-x2))/(y1-y2);
				points[p][1][0]=points[i][1][0];
				points[p][1][1]=points[i][1][1];
				points[p][0][0]=x;
				points[p][0][1]=y;
				points[i][1][0]=x;
				points[i][1][1]=y;
				pointcolor[p][0]=1.0;pointcolor[p][1]=0.0;pointcolor[p][2]=0.0;
				p++;
				display();
				sleep(delay);
				p--;
				points[p][1][0]=x;
				points[p][1][1]=y;
				display();
				sleep(delay);
				t=2;
			}
		}
		display();
		sleep(delay);
		
	}
}
// Taking the keyboard input
void keyboard(unsigned char key, int x, int y) 
{
  switch(key) 
    {
    case 'c': clipping=false;clip();break;
    case 'p': poly=true;display();break;
    case 27: 
      exit(0);
      break;
    default:
      break;
    }
}
// Taking the mouse input
void mouse(int button, int state, int x, int y) 
{
  if(clipping)
  {
   if (state == GLUT_DOWN) 
     {
       if (button == GLUT_LEFT_BUTTON) 
	 {
	   if(poly)
	   {
		       numpoints = 0;
		       points[p][0][0] = x;
		       points[p][0][1] = win_height - y;
		       points[p][1][0] = points[0][0][0];
		       points[p][1][1] = points[0][0][1];
		       points[p-1][1][0] = x;
		       points[p-1][1][1] = win_height - y;
			pointcolor[p][0]=0.5;
			pointcolor[p][1]=0.5;
			pointcolor[p][2]=0.0;
			xx0 = x;
		       yy0 = win_height - y;
		       p++;	   	
	   }
	   else{
		   if ((numpoints % 2 )== 0)
		     {
		       numpoints = 0;
		       points[p][0][0] = x;
		       points[p][0][1] = win_height - y;
		       points[p][1][0] = x;
		       points[p][1][1] = win_height - y;
			pointcolor[p][0]=0.5;
			pointcolor[p][1]=0.5;
			pointcolor[p][2]=0.0;
			xx0 = x;
		       yy0 = win_height - y;
		       numpoints++;
		     }
		   else if ((numpoints % 2) == 1)
		     {
			points[p][1][0] = x;
		       	points[p][1][1] = win_height - y;
		       	numpoints++;
			p++;
			xx1 = x;
		        yy1 = win_height - y;
		     }
	    }
	 }
     }
  glutPostRedisplay();
   }
}       


int main (int argc, char *argv[]) 
{

  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
  glutInitWindowSize( win_width, win_height );

  glutCreateWindow( "cohen-sutherland Clipping Algorithm" );


  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
 
  glutMouseFunc(mouse);
 
  glutMainLoop();
}
