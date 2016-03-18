#include <stdio.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

#define RIGHT 2 
#define LEFT 8
#define TOP 1   
#define BOTTOM 4

double xmin=200,ymin=20,xmax=400,ymax=200;
double xvmin=200,yvmin =300, xvmax=400,yvmax=480;


struct  point
{
    double x;
    double y;
};

vector<struct point *> polygon_vertices;

int find_code(struct point * p, int i)
{
    int bit_code =0;

    if(p->y < ymax && i== 0)   
        bit_code |= TOP;
    if(p->y > ymin && i== 2)    
        bit_code |= BOTTOM;
    if(p->x < xmax && i== 1)   
        bit_code |= RIGHT;
    if(p->x > xmin && i==3)  
      bit_code |= LEFT;

    return bit_code;
}


void read_vertex(int n){
    int x,y;
    for(int  i =0 ; i<n;i++){
        cin >> x >> y;
        point *p = new point();
        p->x = x;
        p->y = y;
        cout << p->x << " " << p->y << endl;
        polygon_vertices.push_back(p);
    }
}

void draw_polygon(){
	cout << "Drawing polygon" << endl;
    glBegin(GL_LINE_LOOP);
    for(int i =0 ;i<polygon_vertices.size();i++){	
    	struct point * p= polygon_vertices[i];
    	cout << p->x << " "  << p->y << endl;
    	glVertex2f(p->x,p->y);
    }
    glEnd();
    glFlush();
	cout << "Drawing ended" << endl;

}

void polygon_clipping(){
    cout << "Clipping Started" << endl;
    for(int i=0; i<4;i++){
        int curr = pow(2,i);
        cout << i << "  <-> " << curr << endl;
        cout << "size " <<  polygon_vertices.size()<< endl;
        if(polygon_vertices.size() > 1){

            vector<point *>::iterator itr_1 = polygon_vertices.begin();
            vector<point *>::iterator itr_2 = polygon_vertices.begin() + 1;

            vector<point *> new_vertex_list;
            while(itr_1 != polygon_vertices.end()){
                
                if(itr_2 == polygon_vertices.end())
                    itr_2 = polygon_vertices.begin();

                struct point *p_1 = *itr_1;
                struct point *p_2 = *itr_2;

                int code_1 = find_code(*itr_1,i);
                int code_2 = find_code(*itr_2,i);
                int result  = code_1 & code_2;
                int cod1_res = code_1 & curr;
                int cod2_res = code_2 & curr;


                cout << "POint p1 " << p_1->x << " " <<p_1->y << endl;
                cout << "POint p2 " << p_2->x << " " <<p_2->y << endl;
                cout << "Code 1 " << code_1 << endl;
                cout << "Code 2 " << code_2 << endl;
                cout << "result "<< result << endl;
                cout << "cod1_res "<< cod1_res << endl;
                cout << "cod2_res "<< cod2_res << endl;

                if(result){
                    // both are inside
                    new_vertex_list.push_back(*itr_2);
                      cout << "Inside" << endl;
                      struct point * p = *itr_2;
                       cout << "POint pushed " << p->x << " " <<p->y << endl;
                }
                else if(cod1_res){
                    // inside to out
                    // push the intersection point
                      cout << "inside to Outside" << endl;
                    struct point *p = new point();
                    p->x = 0, p->y = 0;
                    if(i== 0)
                    {
                        p->x =  p_1->x + (p_2->x - p_1->x)*(ymax-p_1->y)/(p_2->y - p_1->y);;
                        p->y = ymax;
                        cout << "Ymax is " << ymax << endl;
                    }
                    else if(i == 1){
                        p->x = xmax;
                        p->y = p_1->y + (p_2->y - p_1->y) * (xmax - p_1->x)/(p_2->x - p_1->x); ;
                    }
                    else if(i == 2){
                        p->x= p_1->x+(p_2->x - p_1->x)*(ymin- p_1->y)/(p_2->y - p_1->y);
                        p->y = ymin;
                    }
                    else if(i == 3){
                        p->x = xmin;
                        p->y = p_1->y + (p_2->y - p_1->y) * (xmin - p_1->x)/(p_2->x - p_1->x);
                    }
                    new_vertex_list.push_back(p);
                      cout << "POint pushed " << p->x << " " <<p->y << endl;
                }
                else if(cod2_res){
                    // out to in
                      cout << "Outside to inside" << endl;
                    struct point *p = new point();
                    
                    
                    if(i== 0)
                    {
                        p->x =p_1->x + (p_2->x - p_1->x)*(ymax-p_1->y)/(p_2->y - p_1->y);;
                        p->y = ymax;
                    }
                    else if(i == 1){
                        p->x = xmax;
                        p->y = p_1->y + (p_2->y - p_1->y) * (xmax - p_1->x)/(p_2->x - p_1->x); ;
                    }
                    else if(i == 2){
                        p->x= p_1->x+(p_2->x - p_1->x)*(ymin- p_1->y)/(p_2->y - p_1->y);
                        p->y = ymin;
                    }
                    else if(i == 3){
                        p->x = xmin;
                        p->y = p_1->y + (p_2->y - p_1->y) * (xmin - p_1->x)/(p_2->x - p_1->x);
                    }
              		new_vertex_list.push_back(p);
              		new_vertex_list.push_back(*itr_2);
              		  cout << "POint pushed " << p->x << " " <<p->y << endl;
              		  struct point * p2 = *itr_2;
                       cout << "POint pushed " << p2->x << " " <<p2->y << endl;
                }
                else
                {
                    // both outside
                    cout << "Outside" << endl;
                }
                
                itr_1++;
                itr_2++;
            }
            polygon_vertices = new_vertex_list;
            cout << "New point List " << endl;
			vector<point *>::iterator itr = polygon_vertices.begin();            
			while(itr != polygon_vertices.end()){
				cout << (*itr)->x<< " " << (*itr)->y<< endl;
				itr++;
			}
			//draw_polygon();
        }
    }

    cout << "Clipping Finished" << endl;
}

void display()
{
    

    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(0.0,0.5,1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(xmin,ymin);
	glVertex2f(xmax,ymin);
	glVertex2f(xmax,ymax);
	glVertex2f(xmin,ymax);
	glEnd();


	glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xvmin,yvmin);
    glVertex2f(xvmax,yvmin);
    glVertex2f(xvmax,yvmax);
    glVertex2f(xvmin,yvmax);
    glEnd();

    glFlush();
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
    int n;
    cin >> n;
    read_vertex(n);
    draw_polygon();
    polygon_clipping();
	vector<point *>::iterator itr = polygon_vertices.begin(); 
	double scale_x=(xvmax-xvmin)/(xmax-xmin);
    double scale_y=(yvmax-yvmin)/(ymax-ymin);
           
	while(itr != polygon_vertices.end()){
		(*itr)->x = xvmin + ((*itr)->x - xmin) * scale_x;
		(*itr)->y = yvmin + ((*itr)->y - ymin) * scale_y;;
		itr++;
	}
	glColor3f(0.0,0.5,1.0);
    draw_polygon();
    break;
  }
}

void init()
{
    glClearColor(1.0,1.0,1.0,1.0);
    glColor3f(1.0,0.0,0.0);
    glPointSize(1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,499.0,0.0,499.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Cohen-Sutherland  Polygon Clipping Algorithm");
    glutDisplayFunc(display);
    init();
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}