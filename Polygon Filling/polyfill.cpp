#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <stdlib.h>  
#include <GL/glut.h> 
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <map>
#define COORD 20
#define ABS(x) (x>0? x : -1 *x)
#define MAX(x,y) (x>=y?x:y)
#define MIN(x,y) (x<=y?x:y)
#define ALL(vec) vec.begin(),vec.end()

using namespace std;


int ya_min = 99999,ya_max= -99999;
int num_points=0;
map<int,int> high_y;
struct edge_struct
{	
	int y_max ;
	int x_min ;
	int d_x ;
	int d_y ;
	struct edge_struct * next ;
	bool incr ;

	edge_struct(){
		y_max = -1;
		x_min = -1;
		d_x = -1;
		d_y = -1;
		next = NULL;
		incr = true;
	}
};

bool compareByLength(const edge_struct *a, const edge_struct *b)
{
    return a->x_min < b->x_min;
}

typedef struct edge_struct ae_record;

struct  point
{
	int x;
	int y;
	bool incr;
	
	point(){
		incr = true;
	}
};


vector<edge_struct *> buc;

vector<point *> vertex_list;
void initialise_BUC(int y_max){
	for (int i = 0; i < y_max; ++i)	
	{
			buc.push_back(new ae_record());
	}
}

void draw_point(int x, int y){
	glBegin(GL_POLYGON);
		glVertex3i(x,y,0);
		glVertex3i(x+1,y,0);
		glVertex3i(x+1,y+1,0);
		glVertex3i(x,y+1,0);
	glEnd();
	glFlush();
}


void read_vertex(int n){
	int x,y;
	for(int  i =0 ; i<n;i++){
		cin >> x >> y;
		point *p = new point();
		p->x = x;
		p->y = y;
		ya_min = MIN(ya_min, y);
		ya_max = MAX(ya_max, y);
		vertex_list.push_back(p);
		if(high_y.find(x) == high_y.end()){
			high_y[x] = y;
		}
		else{
			high_y[x] = MAX(high_y[x],y);
		}
	}
}

void process_edge(point *p1, point *p2){
	int curr_min = MIN(p1->y,p2->y);
	struct edge_struct *e_s = new edge_struct();
	e_s->y_max = MAX(p1->y,p2->y);
	e_s->x_min = curr_min == p1->y ? p1->x : p2->x;
	e_s->d_x = p2->x - p1->x;
	e_s->d_y = p2->y - p1->y;
	
	cout << curr_min << endl;
	if(buc[curr_min]->y_max == -1){
		buc[curr_min] = e_s;
	}
	else{
		struct edge_struct *e_start = buc[curr_min];
		while(e_start->next!= NULL){
			e_start = e_start->next;
		}
		e_start->next= e_s;
	}
}

bool scanline_vertex_test(int i){
	if(buc[i]->y_max == -1)
		return false;
	return true;
}

void process_vertex(){
	vector<point *>::iterator itr_1;
	vector<point *>::iterator itr_2;
	vector<point *>::iterator itr_3;
	itr_1 = (vertex_list.begin());
	itr_2 = (vertex_list.begin());
	itr_3 = (vertex_list.begin());
	itr_2 += 1;
	itr_3 += 2;
	bool flag = false;
	while(itr_1 !=  vertex_list.end()){
		if(itr_3 == vertex_list.end())
			itr_3 = vertex_list.begin() + 1,flag= true;
		if(itr_2 == vertex_list.end())
			itr_2 = vertex_list.begin();
		if(itr_1 == vertex_list.end()-1)
			break;
	 	point *p1 = *itr_1;
	 	point *p2 = *itr_2;
	 	point *p3 = *itr_3;

	 	int d_21 = p2->y - p1->y;
 		int d_32 = p3->y - p2->y;

 		// cout << p1->x << "-1-" << p1->y << endl;
 		// cout << p2->x << "-2-" << p2->y << endl;
 		// cout << p3->x << "-3-" << p3->y << endl;

 		// cout << d_21 << "-diff-" << d_32 << endl;

	 	if((d_21 > 0 && d_32 > 0 ) || (d_21 < 0 && d_32 < 0 ) || d_21 == 0|| d_32 == 0)
	 		(*itr_2)->incr = false;	

	 	itr_1++;
	 	itr_2++;
	 	if(!flag)
	 	itr_3++;
	}
	itr_1 = (vertex_list.begin());
	itr_2 = (vertex_list.end() -1);
	(*itr_1)->incr = (*itr_2)->incr; 
}

void printPoints(){
	vector<point *>::iterator itr = vertex_list.begin();
	while(itr != vertex_list.end()){
		cout << (*itr)->x << " " << (*itr)->y << " " << (*itr)->incr << endl;
		itr++;
	}
}

void start_process(){
	
	cin >> num_points;
	read_vertex(num_points);
	// cout <<"ya_max "<< ya_max << endl;
	// cout <<"ya_min "<< ya_min << endl;
	initialise_BUC(ya_max);
	process_vertex();
	//printPoints();

	vector<point *>::iterator itr_1;
	vector<point *>::iterator itr_2;
	itr_1 = (vertex_list.begin());
	itr_2 = (vertex_list.begin()+1);

 	while(itr_2 !=  vertex_list.end()){
	 	process_edge(*itr_1,*itr_2);
	 	itr_1++;
	 	itr_2++;
	}
}



void fill_bw(int x1, int x2, int y){
	for(int i=x1; i<=x2;i++){
		draw_point(i,y);
		cout << "Filling " << i << " "  << y << endl;
	}
}

void print_vector(vector<struct edge_struct*> vec){
	for (int i = 0; i < vec.size(); ++i)
	{
	 	int count = 1;
	 	struct edge_struct *e_start = vec[i];
	 	while(e_start!= NULL){
		 	cout << "Iteration "<<i <<" . " << count << endl;
		 	cout << "y_max " << e_start->y_max << endl;
		 	cout << "x_min " << e_start->x_min << endl;
		 	cout << "d_x " << e_start->d_x << endl;
		 	cout << "d_y " << e_start->d_y << endl;
		 	e_start = e_start->next;
	 		count++;
	 	}
	 } 
}

void print_vector_1(vector<struct edge_struct*> vec){
	for (int i = 0; i < vec.size(); ++i)
	{
	
	 	struct edge_struct *e_start = vec[i];
		 	cout << "Iteration "<<i <<" . " << endl;
		 	cout << "y_max " << e_start->y_max << endl;
		 	cout << "x_min " << e_start->x_min << endl;
		 	cout << "d_x " << e_start->d_x << endl;
		 	cout << "d_y " << e_start->d_y << endl;
		 	e_start = e_start->next;
	 } 
}



void scan_fill(){
	// construct the edge table
	int Y_min = ya_min;
	vector<edge_struct *> active_edge_list;
	vector<edge_struct *>::iterator itr;
	
	 for(int y = ya_min ; y <= ya_max;){

	 	cout << "SCANLINE " << y << endl;
		struct edge_struct *e_start = buc[y];
		while(e_start != NULL && e_start->y_max != -1 ){
			active_edge_list.push_back(e_start);
			cout << "inserted one" << endl;
			e_start = e_start->next;
		}

		cout << "ACTIVE EDGE LIST" << endl;

		sort(active_edge_list.begin(),active_edge_list.end(),compareByLength);
		print_vector_1(active_edge_list);
		
		
		//form pairs to fill in between
		if(active_edge_list.size() >= 2){
			int i=0,j=1;
			int size  = active_edge_list.size();
			while(i < size && j < size){
				int x_1 = (active_edge_list[i])->x_min;
				int x_2 = (active_edge_list[j])->x_min;
				fill_bw(x_1,x_2,y);
				i+=2;
				j+=2;
			}
		}	

		y++;		
		vector<edge_struct *> new_active_edge_list;
		for(itr = active_edge_list.begin(); itr!=active_edge_list.end();itr++){
			if((*itr)->y_max == y);
			else{
			 	(*itr)->x_min = (*itr)->x_min + ((*itr)->d_x)/((*itr)->d_y); 
			 	new_active_edge_list.push_back(*itr);
			}
		}

		active_edge_list = new_active_edge_list;
		sort(ALL(active_edge_list),compareByLength);

		cout << endl << endl;
	 }


}
void init(void)
{
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glShadeModel(GL_FLAT);
}


void display(void)
{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor3f(1.0, 1.0, 1.0);
        glLoadIdentity();  /*clear the matrix */
                /*viewing transformation*/
        gluLookAt(0.0,0.0,20.0,0.0,0.0,0.0,0.0,1.0,0.0);

        int i=0;
        for(i=-COORD ; i<=COORD;i++){
	        glBegin(GL_LINES);

	        	if(i !=0 ){
	                glColor3f(6.0f,6.0f,6.0f); //y is red
	                glVertex3i(i,COORD,0);
	                glVertex3i(i,-COORD,0);
	                glColor3f(4.0f,4.0f,4.0f); //x is green
	                glVertex3i(COORD,i,0);
	                glVertex3i(-COORD,i,0);
	                glColor3f(5.0f,5.0f,5.0f); //z is blue
	                glVertex3i(0,0,COORD);
	                glVertex3i(0,0,-COORD);
	            }
	            else{
	            	glColor3f(1.0f,0.0f,0.0f); //y is red
	                glVertex3i(i,COORD,0);
	                glVertex3i(i,-COORD,0);
	
	                glColor3f(0.0f,1.0f,0.0f); //x is green

	                glVertex3i(COORD,i,0);
	                glVertex3i(-COORD,i,0);
	                glColor3f(0.0f,0.0f,1.0f); //z is blue
	                glVertex3i(0,0,COORD);
	                glVertex3i(0,0,-COORD);
	            }
	        glEnd();
        }
        glFlush();   
}
void plot_remain(){
	map<int,int>::iterator itr = high_y.begin();
	while(itr != high_y.end()){
		draw_point(itr->first,itr->second);
		itr++;
	}

}


void keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {
   case 'p':     
   case 'P':
      cout << "P pressed" << endl;
       start_process();
       //print_vector(buc);
       	scan_fill();
       	plot_remain();
      break;
   
  }
}

void reshape(int w, int h)
{
        glViewport(0, 0, (GLsizei) w, (GLsizei) h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
        glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char** argv)
{
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
        glutInitWindowSize(500,500);
        glutInitWindowPosition(100,100);
        glutCreateWindow(argv[0]);
        init();
        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutKeyboardFunc(keyboard);
        glutMainLoop();
       
       
        return 0;
}