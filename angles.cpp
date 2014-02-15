#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <vector>
// #include <algorithm>

#include "model.h"
#include "controls.h"

#ifdef _WIN32
#include "gl/glut.h"
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

#define PI 3.14159

float angle(vector v0, vector v1)
{
    return (180 * acos(v0 * v1 / (v0.magn() * v1.magn()))) / PI;
}

void model::compute_angles()
{
    face f;
    float a0, a1, a2;
    vertex v0, v1, v2;
    for (int i=0; i<faces.size(); i++) {
	f = faces[i];
	v0 = vertices[f.vi0];
	v1 = vertices[f.vi1];
	v2 = vertices[f.vi2];

	a0 = angle(vector(v0, v1), vector(v0, v2));
	a1 = angle(vector(v1, v0), vector(v1, v2));
	a2 = 180 - a0 - a1;

	angles.push_back(a0);
	angles.push_back(a1);
	angles.push_back(a2);
    }

    // std::sort(angles.begin(), angles.end());
}

void text(const char *str,float size)
{
    glPushMatrix();
    glScalef(size,size,size);
    for (int i=0;i<strlen(str);i++)
	glutStrokeCharacter(GLUT_STROKE_ROMAN,str[i]);
    glPopMatrix();
}

void prepare_angle_histogram(float w, float h)
{
    // axes
    glBegin(GL_LINES);
    glVertex2f(-1.1*w,-h);
    glVertex2f(w, -h);
    glVertex2f(-1.1*w, -h);
    glVertex2f(-1.1*w, h);
    glEnd();


    // horizontal axis labeling
    const char* hlabels[10] =
	{"0", "20", "40", "60", "80", "100", "120", "140", "160", "180"};

    // 10 subdivisions on horizontal axis (9*20 = 180 deg)
    glPushMatrix();
    glTranslatef(-w, -1.1*h, 0);
    for (int i=0; i<10; i++) {
	text(hlabels[i], 0.002);
	glTranslatef(2*w/9.0, 0, 0);
    }
    glPopMatrix();

    // marks on horizontal axis
    glPushMatrix();
    glTranslatef(-w, -h, 0);
    for (int i=0; i<10; i++) {
	glBegin(GL_LINES);
	glVertex2f(0, -0.02*h);
	glVertex2f(0, 0.02*h);
	glEnd();
	glTranslatef(2*w/9.0, 0, 0);
    }
    glPopMatrix();

    // vertical axis labeling
    const char* vlabels[6] = {"0.0", "0.2", "0.4", "0.6", "0.8", "1.0"};

    // 6 subdivisions on vertical axis (5*0.2 = 1.0 freq)
    glPushMatrix();
    glTranslatef(-1.2*w, -h, 0);
    for (int i=0; i<6; i++) {
	text(vlabels[i], 0.002);
	glTranslatef(0, 2*h/5.0, 0);
    }
    glPopMatrix();

    // marks on vertical axis
    glPushMatrix();
    glTranslatef(-1.1*w, -h, 0);
    for (int i=0; i<6; i++) {
	glBegin(GL_LINES);
	glVertex2f(-0.02*h, 0);
	glVertex2f(0.02*h, 0);
	glEnd();
	glTranslatef(0, 2*h/5.0, 0);
    }
    glPopMatrix();
}

void model::histogram()
{
    // prevent external influence
    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);


    float w = 10; float h = 5;

    // store number of angles (rounded)
    std::vector<float> angle_count = std::vector<float> (181, 0);

    for (int i=0; i<angles.size(); i++) 
	angle_count[(int) (angles[i]+0.5)]++;

    float max_count = 0;
    for (int i=0; i<181; i++)
	if (angle_count[i] > max_count) max_count = angle_count[i];
	
    // draw histogram
    prepare_angle_histogram(w, h);

    glPushMatrix();
    glTranslatef(-w, -h, 0);
    for (int i=0; i<181; i++) {
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, 2*h*angle_count[i]/max_count);
	glEnd();
	glTranslatef(w/180.0, 0, 0);
    }
    glPopMatrix();
        
    if (texture_enabled) glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopAttrib();
}
