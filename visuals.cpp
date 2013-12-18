#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <vector>

#include "model.h"
#include "visuals.h"
#include "controls.h"
#include "texture.h"

#ifdef _WIN32
#include "gl/glut.h"
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

std::vector<model> models;

void apply_rt(struct rt rt)
{
	glTranslatef(rt.tx, rt.ty, rt.tz);
	glRotatef(rt.rx, 1, 0, 0);
	glRotatef(rt.ry, 0, 1, 0);
	glRotatef(rt.rz, 0, 0, 1);
}

void render()
{
	// Clean up the colour of the window and the depth bufferp
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	apply_rt(rt);

	models[0].draw(mode);

	glutSwapBuffers(); // All drawing commands applied to the hidden
					   // buffer, so now, bring forward the hidden
					   // buffer and hide the visible one
}

void resize(int w, int h)
{
	// define the visible area of the window ( in pixels )
	if (h==0) h=1;
	glViewport(0,0,w,h);

	// Setup viewing volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//         L	   R	  B	T	 N	   F
	//glOrtho(-100.0, 100.0, -100.0, 100.0, 100.0, -100.0);
	//glFrustum(-100.0, 100.0, -100.0, 100.0, 1, 40.0);
	gluPerspective(30.0, (float(w)/h), 1.0, 1000);

	//gluLookAt(20, 100, 0, 0, 0, 0, 0, 0, 1);
	gluLookAt(0, 0, -50, 0, 0, 0, 0, 1, 0);
	// gluLookAt(0, 0, -200, 0, 0, 0, 0, 1, 0);
}

void idle()
{
	glutPostRedisplay();
}

void setup()
{

	models.push_back(model("models/b406_low.obj"));

	for (int i = 0; i < models.size(); i++) {
		models[i].face_normals();
		models[i].vertex_normals();
	}

	models[0].center();

	//Parameter handling
	glShadeModel (GL_SMOOTH);

	// glFrontFace(GL_CW);
	// glCullFace(GL_FRONT);
	// glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1);

	// default material properties
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE || GL_SPECULAR);

	GLfloat specref[4];
	specref[0] = 0.19225; specref[1] = 0.19225;
	specref[2] = 0.19225; specref[3] = 0.19225;
	glMaterialfv(GL_FRONT,GL_AMBIENT,specref);

	specref[0] = 0.50754; specref[1] = 0.50754;
	specref[2] = 0.50754; specref[3] = 0.50754;
	glMaterialfv(GL_FRONT,GL_DIFFUSE,specref);

	specref[0] = 0.508273; specref[1] = 0.508273;
	specref[2] = 0.508273; specref[3] = 0.508273;
	glMaterialfv(GL_FRONT,GL_SPECULAR,specref);

	glMaterialf(GL_FRONT,GL_SHININESS, 51.2);

	// default light properties
	GLfloat light_position[] = { 20.0, 40.0, -150.0, 1.0 };
	GLfloat ambientLight[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };

	// init light0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	// enable default lights
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//glEnable(GL_POINT_SMOOTH);

	glEnable(GL_TEXTURE_2D);
	load_rgb_texture("textures/bricks.rgb", 512, 512, 1);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	// Black background
	glClearColor(0.0f,0.0f,0.0f,1.0f);
}
