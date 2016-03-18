#include <stdio.h> 
#include <GL/glut.h> 
#include <iostream>
using namespace std;
#define RIGHT 8 
#define LEFT 2
#define TOP 4   
#define BOTTOM 1

double xmin=200,ymin=20,xmax=400,ymax=210;
double xvmin=200,yvmin =300, xvmax=400,yvmax=480;

struct  point
{
	double x;
	double y;
};


int find_code(struct point * p)
{
    int bit_code =0;
    if(p->y > ymax)   
    	bit_code |= TOP;
    if(p->y < ymin)    
    	bit_code |= BOTTOM;
    if(p->x > xmax)   
    	bit_code |= RIGHT;
    if(p->x < xmin)  
      bit_code |= TOP;

    return bit_code;
}
void CohenSutherlandLineAlgorithm(struct point *p_1, struct point *p_2)
{
    int code_p_1,code_p_2,intOut;
    int done =0,draw=0;
    code_p_1= find_code(p_1);
    code_p_2= find_code(p_2);
    do
    {
        
        if(code_p_1 & code_p_2)    
        	done=1;
        else if(!(code_p_1 | code_p_2))
            draw= 1,done=1;
        else
        {
            struct point *p = new point();

            intOut= code_p_1?code_p_1:code_p_2;
            
            if(intOut & TOP)
            {
                p->x= p_1->x + (p_2->x - p_1->x)*(ymax-p_1->y)/(p_2->y - p_1->y);
                p->y=ymax;
            }
            else if(intOut & BOTTOM)
            {
                p->x= p_1->x+(p_2->x - p_1->x)*(ymin- p_1->y)/(p_2->y - p_1->y);
                p->y=ymin;
            }
            else if(intOut & RIGHT)
            {
            	p->y = p_1->y + (p_2->y - p_1->y) * (xmax - p_1->x)/(p_2->x - p_1->x);    
                p->x=xmax;
            }
            else
            {
            	p->y = p_1->y + (p_2->y - p_1->y) * (xmin - p_1->x)/(p_2->x - p_1->x); 
                p->x=xmin;
            }

            if(intOut == code_p_1)
            {
                p_1->x = p->x;
                p_1->y = p->y;
                code_p_1 = find_code(p_1);
            }
            else
            {
                p_2->x = p->x;
                p_2->y = p->y;
                code_p_2 = find_code(p_2);
            }
        }
    }
    while(!done);
    
    if(draw){
	    double scale_x=(xvmax-xvmin)/(xmax-xmin);
	    double scale_y=(yvmax-yvmin)/(ymax-ymin);
	    double vx0 = xvmin + (p_1->x - xmin) * scale_x;
	    double vy0 = yvmin + (p_1->y - ymin) * scale_y;
	    double vx1 = xvmin + (p_2->x - xmin) * scale_x;
	    double vy1 = yvmin + (p_2->y - ymin) * scale_y;

	    glColor3f(1.0,0.0,0.0);
	    glBegin(GL_LINE_LOOP);
	    glVertex2f(xvmin,yvmin);
	    glVertex2f(xvmax,yvmin);
	    glVertex2f(xvmax,yvmax);
	    glVertex2f(xvmin,yvmax);
	    glEnd();

	    glColor3f(0.0,0.0,1.0);
	    glBegin(GL_LINES);
	    glVertex2d(vx0,vy0);
	    glVertex2d(vx1,vy1);
	    glEnd();
    }
}

void display()
{
    

    glClear(GL_COLOR_BUFFER_BIT);
    
   
    

    glFlush();
}
void myinit()
{
    glClearColor(1.0,1.0,1.0,1.0);
    glColor3f(1.0,0.0,0.0);
    glPointSize(1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,499.0,0.0,499.0);
}

void input(struct point *p_1, struct point *p_2){
	double x0=220,y0=200,x1=440,y1=30;
    p_1-> x = x0, p_1->y = y0;
    p_2-> x = x1, p_2->y = y1;
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {
   case 'p':     
   case 'P':
	cout << "P pressed" << endl;
	struct point *p_1,*p_2;
	p_1 = new point();
	p_2 = new point();
	input(p_1,p_2);

    glColor3f(1.0,0.0,1.0);
    glBegin(GL_LINES);
    glVertex2d(p_1->x,p_1->y);
    glVertex2d(p_2->x,p_2->y);
    glEnd();
	
    
    
    CohenSutherlandLineAlgorithm(p_1,p_2);
	
	glColor3f(0.0,0.5,1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xmin,ymin);
    glVertex2f(xmax,ymin);
    glVertex2f(xmax,ymax);
    glVertex2f(xmin,ymax);
    glEnd();
    glFlush();
	
    break;
  }
}

int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Cohen-Sutherland Line Clipping Algorithm");
    glutDisplayFunc(display);
    myinit();
 	glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}