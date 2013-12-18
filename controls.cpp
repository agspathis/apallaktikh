#include <stdio.h>
#include <math.h>

#include "visuals.h"
#include "controls.h"

#ifdef _WIN32
#include "gl/glut.h"
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

int mode = SOLID;

int object = BOTH;				// active object = 0, 1 or BOTH (2)
struct rt rt;

static float r_step = 5;
static float t_step = 0.5;

static int last_x = 0;
static int last_y = 0;

void key_down(unsigned char key,int x,int y)
{
	switch(key)
	{
	case 27 : exit(0);
		break;

		// translations
	case 'q':
		rt.tx += t_step;
		break;
	case 'w':
		rt.tx -= t_step;
		break;
	case 'a':
		rt.ty += t_step;
		break;
	case 's':
		rt.ty -= t_step;
		break;
	case 'z':
		rt.tz += t_step;
		break;
	case 'x':
		rt.tz -= t_step;
		break;
		
		// rotations
	case 'o':
		rt.rx -= r_step;
		break;
	case 'p':
		rt.rx += r_step;
		break;
	case 'k':
		rt.ry -= r_step;
		break;
	case 'l':
		rt.ry += r_step;
		break;
	case 'n':
		rt.rz -= r_step;
		break;
	case 'm':
		rt.rz += r_step;
		break;
		
	default : break;
	}

	glutPostRedisplay();
}

void key_up(unsigned char key,int x,int y)
{
	glutPostRedisplay();
}

void mouse(int button,int state,int x,int y)
{
	if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		last_x = x;
		last_y = y;
		glutPostRedisplay();
	}
}

void mouse_motion(int x, int y)
{
	float dist = 0.001 * (x - last_x + y - last_y);
 	last_x = x;
 	last_y = y;
	glutPostRedisplay();
}

void menu(int choice)
{
	switch (choice) {
	case WIRE :
		mode = WIRE;
		break;
	case SOLID :
		mode = SOLID;
		break;
	default : break;
	}
	glutPostRedisplay();
}
