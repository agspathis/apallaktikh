#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <vector>
#include <algorithm>

#include "model.h"
#include "vmodel.h"
#include "visuals.h"
#include "controls.h"
#include "texture.h"
#include "dynamics.h"

#ifdef _WIN32
#include "gl/glut.h"
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

std::vector<vmodel> vmodels;	// voxelized
std::vector<model> rmodels;	// reconstructed
particle_system ps;		// particle system simulation

model master_model;

void apply_rt(struct rt rt)
{
    glTranslatef(rt.tx, rt.ty, rt.tz);
    glRotatef(rt.rx, 1, 0, 0);
    glRotatef(rt.ry, 0, 1, 0);
    glRotatef(rt.rz, 0, 0, 1);
}

int current_model_index()
{
    if (vres/vres_step-1 > vmodels.size()-1)
	return vmodels.size()-1;
    else return vres/vres_step-1;
}

void render()
{
    // Clean up the colour of the window and the depth bufferp
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // glPolygonMode(GL_FRONT_AND_BACK,mode);

    // determine the index of the (v)model to render
    int render_index = current_model_index();

    switch (view) {
    case 0 :
	gluLookAt(0, 0, distance, 0, 0, 0, 0, 1, 0);
	apply_rt(rt);
	master_model.draw(mode);
	break;
    case 1 :
	gluLookAt(0, 0, distance, 0, 0, 0, 0, 1, 0);
	apply_rt(rt);
	glDisable(GL_TEXTURE_2D);
	vmodels[render_index].draw(VELEM_CUBE, mode);
	glDisable(GL_TEXTURE_2D);
	break;
    case 2 :
	gluLookAt(0, 0, distance, 0, 0, 0, 0, 1, 0);
	apply_rt(rt);
	rmodels[render_index].draw(mode);
	break;
    case 3 :
	gluLookAt(0, 0, 30, 0, 0, 0, 0, 1, 0);
	master_model.histogram();
	break;
    case 4 :
	gluLookAt(0, 0, 30, 0, 0, 0, 0, 1, 0);
	rmodels[render_index].histogram();
	break;
    case 5 :
	gluLookAt(0, 0, distance, 0, 0, 0, 0, 1, 0);
	apply_rt(rt);
	glDisable(GL_TEXTURE_2D);
	vmodels[render_index].draw(VELEM_SPHERE, mode);
	glEnable(GL_TEXTURE_2D);
	break;
    case 6 :
	gluLookAt(0, 0, distance, 0, 0, 0, 0, 1, 0);
	apply_rt(rt);
	glDisable(GL_TEXTURE_2D);
	ps.draw();
	glEnable(GL_TEXTURE_2D);
	break;

    default : break;
    }

    // All drawing commands applied to the hidden buffer, so now, bring
    // forward the hidden buffer and hide the visible one
    glutSwapBuffers();
}

void resize(int w, int h)
{
    // define the visible area of the window ( in pixels )
    if (h==0) h=1;
    glViewport(0,0,w,h);

    // Setup viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float window_ratio = ((float)w)/h;
    gluPerspective(30.0, window_ratio, 1.0, 1000);
}

void idle()
{
    while (vres > vres_step*vmodels.size()) {
	// while requested > largest available, generate one more

	vmodel new_vmodel =
	    vmodel(master_model, vres_step*(1+vmodels.size()));
	vmodels.push_back(new_vmodel);

	model new_rmodel = model(new_vmodel);
	new_rmodel.distance(master_model);
	rmodels.push_back(new_rmodel);

	printf("distance: %f\n\n", new_rmodel.dist);
    }

    if (ff_sim) {
	ps = particle_system(vmodels[current_model_index()]);
	ps.run(500, 0);
	ff_sim = 0;
    }
    if (c_sim) {
	ps = particle_system(vmodels[current_model_index()]);
	ps.run(1000, 1);
	c_sim = 0;
    }

    glutPostRedisplay();
}

void setup()
{
    // read models
    master_model = model("models/model.obj");

    // vmodels.push_back(vmodel(master_model, vres_step));
    // rmodels.push_back(model(vmodels[0]));

    vmodel new_vmodel =
	vmodel(master_model, vres_step*(1+vmodels.size()));
    vmodels.push_back(new_vmodel);

    model new_rmodel = model(new_vmodel);
    new_rmodel.distance(master_model);
    rmodels.push_back(new_rmodel);

    printf("distance: %f\n\n", new_rmodel.dist);

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
    GLfloat light0_position[] = { 20.0, 40.0, 150.0, 1.0 };
    GLfloat light1_position[] = { 20.0, 40.0, -150.0, 1.0 };
    GLfloat ambientLight[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat diffuseLight[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat specularLight[] = { 0.1, 0.1, 0.1, 1.0 };

    // init light0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    // init light1
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

    // enable default lights
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    //glEnable(GL_POINT_SMOOTH);

    load_rgb_texture("textures/texture.rgb", 512, 512, 1);

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    // Black background
    glClearColor(0.0f,0.0f,0.0f,1.0f);
}
