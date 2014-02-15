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

struct rt rt = {0, 0, 0, 0, 0, 0};

static float r_step = 5;
static float t_step = 0.5;

static int last_x = 0;
static int last_y = 0;

float distance = 30;
int texture_enabled = 0;
int vres_step = 5;
int vres = vres_step;
int view = 0;

int c_sim = 0;
int ff_sim = 0;
int sim_pause = 0;
int sim_ready = 0;
int frame_step = 1;

void key_down(unsigned char key,int x,int y)
{
    switch(key)
    {
    case 27 : exit(0);		// escape
	break;

    case '-' : distance += 1;
	break;
    case '=' : distance -= 1;
	break;

    case 't':
	if (texture_enabled) {
	    glDisable(GL_TEXTURE_2D);
	    texture_enabled = 0;
	}
	else {
	    glEnable(GL_TEXTURE_2D);
	    texture_enabled = 1;
	}
	break;
    case '0': view = 0;		// master model
	break;
    case '1': view = 1;		// voxel model (cubes)
	break;
    case '2': view = 2;		// reconstructed model
	break;
    case '3': view = 3;		// angle histogram (master)
	break;
    case '4': view = 4;		// angle histogram (reconstructed)
	break;
    case '5': view = 5;		// voxel model (spheres)
	break;
    case '6': view = 6;		// simulation
	break;

    case 'i': vres += vres_step;
	break;
    case 'd':
	if (vres>vres_step) vres -= vres_step;
	break;

    case 'f':
	ff_sim = 1;		// free fall
	break;
    case 'c':			// collision
	c_sim = 1;
	break;
    case ',':			// rewind
	frame_step -= 1;
	break;
    case '.':			// fast forward
	frame_step += 1;
	break;

    default : break;
    }

    glutPostRedisplay();
}

void key_up(unsigned char key, int x, int y)
{
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    switch (button) {
    case GLUT_LEFT_BUTTON :
	if(state == GLUT_DOWN) {
	    last_x = x;
	    last_y = y;
	    glutPostRedisplay();
	}
	break;
    case 3 : distance -= 1;
	break;
    case 4 : distance += 1;
	break;
    default : break;
    }
}

void mouse_motion(int x, int y)
{
    float rot_x = 0.5 * (y - last_y);
    float rot_y = 0.5 * (x - last_x);
    last_x = x;
    last_y = y;
    rt.rx += rot_x;
    rt.ry += rot_y;
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
