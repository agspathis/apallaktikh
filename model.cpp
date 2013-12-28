#include <math.h>
#include <stdio.h>

#include <vector>

#include "model.h"
#include "vmodel.h"
#include "intersection.h"

#ifdef _WIN32
#include "gl/glut.h"
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

#define PI 3.14159


// vertex

vertex::vertex()
{
    x = 0;
    y = 0;
    z = 0;
}

// coordinate constructor
vertex::vertex(float cx, float cy, float cz)
{
    x = cx;
    y = cy;
    z = cz;
}


// vector

vector::vector()
{
    i = 0;
    j = 0;
    k = 0;
};

// coordinate constructor
vector::vector(float ci, float cj, float ck)
{
    i = ci;
    j = cj;
    k = ck;
}

vector::vector(vertex start, vertex end)
{
    i = end.x - start.x;
    j = end.y - start.y;
    k = end.z - start.z;
};

void vector::vector_print()
{
    printf("%f %f %f\n", i, j, k);
}

void vector::flip()
{
    i = -i;
    j = -j;
    k = -k;
}

void vector::normalize()
{
    float magnitude = sqrt(i*i + j*j + k*k);
    i /= magnitude;
    j /= magnitude;
    k /= magnitude;
}


// face

face::face()
{
    vi1 = 0;
    vi2 = 0;
    vi3 = 0;
}

// vertex inidices constructor

face::face(int vertex_i1, int vertex_i2, int vertex_i3)
{
    vi1 = vertex_i1;
    vi2 = vertex_i2;
    vi3 = vertex_i3;
}


// model

// .obj file constructor

model::model(const char* filename)
{
    vertex v;
	float x_min, y_min, z_min;
	float x_max, y_max, z_max;
    int vi1, vi2, vi3;
    char line[128];
    FILE *objfile;

    if (!(objfile = fopen(filename, "rt"))) return;

    while (fgets(line, 128, objfile)) {
	switch (line[0]) {
	case 'v':
	    sscanf(&line[1],"%f %f %f", &v.x, &v.y, &v.z);
	    vertices.push_back(v);
		if (v.x < x_min) x_min = v.x;
		if (v.y < y_min) y_min = v.y;
		if (v.z < z_min) z_min = v.z;

		if (v.x > x_max) x_max = v.x;
		if (v.y > y_max) y_max = v.y;
		if (v.z > z_max) z_max = v.z;
	    break;
	case 'f':
	    sscanf(&line[1],"%d %d %d", &vi1, &vi2, &vi3);
	    faces.push_back(face(--vi1, --vi3, --vi2));
	    break;
	default:
	    continue;
	};
    }
    fclose(objfile);
	
    vnormals =  std::vector<vector> (vertices.size(), vector());

	aabb_min = vertex(x_min, y_min, z_min);
	aabb_max = vertex(x_max, y_max, z_max);

	this->center();
	this->face_normals();
	this->vertex_normals();
}

// face normal calculation
void model::face_normals()
{
    face f;
    vector fn;
    for (int i = 0; i < faces.size(); i++) {
	f = faces[i];
	fn = vector(vertices[f.vi1], vertices[f.vi2])
	    % vector(vertices[f.vi1], vertices[f.vi3]);
	fn.normalize();
	fnormals.push_back(fn);
    }
}

// vertex normal calculation
void model::vertex_normals()
{
    face f;
    vector fn;
    for (int i = 0; i < faces.size(); i++) {
    	f = faces[i];
    	fn = fnormals[i];
    	vnormals[f.vi1] += fn;
    	vnormals[f.vi2] += fn;
    	vnormals[f.vi3] += fn;
    }

    for (int i = 0; i < vnormals.size(); i++) {
    	vnormals[i].normalize();
    }

}

// debugging method
void model::debug()
{
    printf("%d\n", vertices.size());
    printf("%f %f %f\n", vertices[4].x, vertices[4].y, vertices[4].z);
    printf("%d %d %d\n", faces[4].vi1, faces[4].vi2, faces[4].vi3);
    printf("%f %f %f\n", fnormals[4].i, fnormals[4].j, fnormals[4].k);
    printf("%f %f %f\n", vnormals[4].i, vnormals[4].j, vnormals[4].k);

    vector v1, v2, v3, v4;
    v1 = vector(1, 4, 3);
    v2 = vector(3, -2, 2);
    v3 = v1 % v2;
    v4 = v2 % v1;
    v1.vector_print();
    v2.vector_print();
    v3.vector_print();
    v4.vector_print();

    for (int i = 0; i < vnormals.size(); i++) {
    	vnormals[i].vector_print();
    }
}

// set material properties from coordinates of v
void set_coordinate_material(float x, float y, float z)
{
    float r = x*x + y*y + z*z;

    GLfloat specref[4];

    specref[0] = x/r; specref[1] = y/r;
    specref[2] = z/r; specref[3] = 0.2;
    glMaterialfv(GL_FRONT,GL_AMBIENT,specref);

    specref[0] = z/r; specref[1] = x/r;
    specref[2] = y/r; specref[3] = 0.5;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,specref);

    specref[0] = y/r; specref[1] = z/r;
    specref[2] = x/r; specref[3] = 0.5;
    glMaterialfv(GL_FRONT,GL_SPECULAR,specref);

    glMaterialf(GL_FRONT,GL_SHININESS, r);
}

// set texture coordinates from coordinates of v (spherical mapping)
void spherical_map_texture(float x, float y, float z)
{
    float theta, phi, r, u, v;
    int repeat_count = 2;
    theta = atan2(x, z);
    phi = atan2(y, sqrt(x*x+z*z));
    r = sqrt(x*x + y*y + z*z);
    u = (theta + PI) / (2 * PI);
    v = (phi + PI/2) / PI;
    glTexCoord2d(repeat_count * u, repeat_count * v);
}

void model::draw(int mode)
{
    face f = face(0, 0, 0);
    vertex v1, v2, v3;
    vector vn1, vn2, vn3;

    if (mode) glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    else glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    
    glBegin(GL_TRIANGLES);

    for (int i=0; i<faces.size(); i++) {
	f = faces[i];

	v1 = vertices[f.vi1];
	v2 = vertices[f.vi2];
	v3 = vertices[f.vi3];

	vn1 = vnormals[f.vi1];
	vn2 = vnormals[f.vi2];
	vn3 = vnormals[f.vi3];

	glNormal3f(vn1.i, vn1.j, vn1.k);
	set_coordinate_material(v1.x, v1.y, v1.z);
	spherical_map_texture(v1.x, v1.y, v1.z);
	glVertex3f(v1.x, v1.y, v1.z);

	glNormal3f(vn2.i, vn2.j, vn2.k);
	set_coordinate_material(v2.x, v2.y, v2.z);
	spherical_map_texture(v2.x, v2.y, v2.z);
	glVertex3f(v2.x, v2.y, v2.z);

	glNormal3f(vn3.i, vn3.j, vn3.k);
	set_coordinate_material(v3.x, v3.y, v3.z);
	spherical_map_texture(v3.x, v3.y, v3.z);
	glVertex3f(v3.x, v3.y, v3.z);

    }
    glEnd();

	// axes for debugging

	// glPushMatrix();
	// glTranslatef(aabb_min.x, aabb_min.y, aabb_min.z);
	
	// glBegin(GL_LINES);
	// glColor3f(1, 0, 0);
	// glVertex3f(0, 0, 0);
	// glVertex3f(4, 0, 0);
	// glEnd();

	// glBegin(GL_LINES);
	// glColor3f(0, 1, 0);
	// glVertex3f(0, 0, 0);
	// glVertex3f(0, 4, 0);
	// glEnd();

	// glBegin(GL_LINES);
	// glColor3f(0, 0, 1);
	// glVertex3f(0, 0, 0);
	// glVertex3f(0, 0, 4);
	// glEnd();

	// glPopMatrix();	
}

void model::center()
{
    vertex v;
    float cx, cy, cz;
    float dx, dy, dz;
    float sum_x, sum_y, sum_z;
    for (int i=0; i<vertices.size(); i++) {
	v = vertices[i];
	sum_x += v.x;
	sum_y += v.y;
	sum_z += v.z;
    }
    cx = sum_x / vertices.size();
    cy = sum_y / vertices.size();
    cz = sum_z / vertices.size();

    for (int i=0; i<vertices.size(); i++) {
	vertices[i].x -= cx;
	vertices[i].y -= cy;
	vertices[i].z -= cz;
    }

	aabb_min.x -= cx;
	aabb_min.y -= cy;
	aabb_min.z -= cz;

	aabb_max.x -= cx;
	aabb_max.y -= cy;
	aabb_max.z -= cz;
}
