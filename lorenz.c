#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//  OpenGL with prototypes for glext
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif



//globals
int numPts = 200000;
float pts[200000][3];
int view_rotx = 145, view_roty = 215, view_rotz = 0;
 
/*  Lorenz Parameters  */
double s  = 10;
double b  = 2.6666;
double r  = 28;

int iter = 0;

#define LEN 8192  // Maximum length of text string

void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPushMatrix();
	glLoadIdentity();
	//set view angle	
	glRotatef(view_rotx, 1.0, 0.0, 0.0);
	glRotatef(view_roty, 0.0, 1.0, 0.0);
	glRotatef(view_rotz, 0.0, 0.0, 1.0);

	glBegin(GL_LINE_STRIP);
	
	//color Lorenz plot
	int i = 0;
	while( i < iter && i < numPts ) {
		glColor3fv(pts[i]);
		glVertex3fv(pts[i++]);
	}

	glEnd();
	

	//  Draw axes in white
   	glColor3f(1,1,1);
   	glBegin(GL_LINES);
   	glVertex3d(0,0,0);
   	glVertex3d(70,0,0);
   	glVertex3d(0,0,0);
   	glVertex3d(0,70,0);
   	glVertex3d(0,0,0);
   	glVertex3d(0,0,70);
   	glEnd();
   
   //  Label axes
   glRasterPos3d(70,0,0);
   Print("X");
   glRasterPos3d(0,70,0);
   Print("Y");
   glRasterPos3d(0,0,70);
   Print("Z");

   //  Display parameters
   glWindowPos2i(5,5);
   Print("View Angle=%d, %d, %d",view_rotx,view_roty,view_rotz);
   

	glFlush();
	glutSwapBuffers();
	
	glPopMatrix();
	
}

static void reshape(int width, int height) {
  
  glViewport(0, 0, (GLint) width, (GLint) height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();				//undo previous transformations
  
  //No perspective viewpoint | 2D projection
  glOrtho(100.0, -100.0, 100.0, -100.0, 100.0, -100.0);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  

}

static void special(int k, int x, int y) {
	switch (k) {
		case GLUT_KEY_UP:
			view_rotx += 5.0;
			break;
		case GLUT_KEY_DOWN:
			view_rotx -= 5.0;
			break;
		case GLUT_KEY_LEFT:
			view_roty -= 5.0;
			break;
		case GLUT_KEY_RIGHT:
			view_roty += 5.0;
			break;
		default:
			return;
	}
  glutPostRedisplay();
}

static void key(unsigned char k, int x, int y) {
	
	//  Exit on ESC
    if (k == 27)
      exit(0);

	switch (k) {
		case 's':
			view_rotz += 5.0;
			break;
		case 'a':
			view_rotz -= 5.0;
			break;
		case 'r':
			iter = 0;
			break;
		case 'f':
			iter = numPts;
			break;		
		default:
			return;
	}
	glutPostRedisplay();
}

static void idle(void) {
	iter+= 200;
	glutPostRedisplay();
}

static void lorenzGen(void) {
	int i;
	/*  Coordinates  */
	float x = pts[0][0] = 1;
	float y = pts[0][1] = 1;
	float z = pts[0][2] = 1;
	/*  Time step  */
	float dt = 0.001;

	for (i=0;i<numPts-1;i++)
	{	
		float dx = s*(y-x);
		float dy = x*(r-z)-y;
		float dz = x*y - b*z;
		x += dt*dx;
		y += dt*dy;
		z += dt*dz;
		
		pts[i+1][0] = x;
		pts[i+1][1] = y;
		pts[i+1][2] = z;
		
	}
}

int main(int argc,char* argv[]) {
	lorenzGen();
	
	glutInit(&argc,argv);
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);
	
	glutCreateWindow("Lorenz Attractor");
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);		//progress background while receiveing information
	
	glutMainLoop();
	
	return 0;
}
