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

// voxel model

vmodel::vmodel(model m, int sampling_steps)
{
	float ld; // longest dimension of aabb
	aabb_min = m.aabb_min;
	aabb_max = m.aabb_max;

	vector aabb_diagonal = vector(aabb_min, aabb_max);

	// successive comparisons to find the ld
	if (aabb_diagonal.i > aabb_diagonal.j) ld = aabb_diagonal.i;
	else ld = aabb_diagonal.j;
	if (ld < aabb_diagonal.k) ld = aabb_diagonal.k;

	// voxel size determination
	vsize = ld / sampling_steps;

	x = (int) ceil(aabb_diagonal.i / vsize) + 1;
	y = (int) ceil(aabb_diagonal.j / vsize) + 1;
	z = (int) ceil(aabb_diagonal.k / vsize) + 1;

	printf("vmodel x:%d y:%d z:%d, total:%d \n", x, y, z, x*y*z);

	// allocate and clear voxels
	voxels = (char***) malloc (x*sizeof(char**));
	for (int i = 0; i < x; i++) {
		voxels[i] = (char**) malloc (y*sizeof(char*));
		for (int j = 0; j < y; j++) {
			voxels[i][j] = (char*) malloc (z*sizeof(char));
		}
	}

	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			for (int k = 0; k < z; k++) {
				voxels[i][j][k] = 0;
			}
		}
	}

	std::vector<face> faces = m.faces;
	std::vector<vertex> vertices = m.vertices;

	// origin of ray and triangle vertices
	vertex origin, v1, v2, v3;
	// direction of ray
	vector dir = vector(1, 1, 1);
	// temporary triangle storage
	face triangle;
	// placeholder for each intersection coordinates
	float tf;

	for (int f = 0; f < faces.size(); f++) {
		triangle = faces[f];
		v1 = vertices[triangle.vi1];
		v2 = vertices[triangle.vi2];
		v3 = vertices[triangle.vi3];

		for (int i = 0; i < x; i++) {
			origin.x = aabb_min.x + i*vsize;
			for (int j = 0; j < y; j++) {
				origin.y = aabb_min.y + j*vsize;
				for (int k = 0; k < z; k++) {
					origin.z = aabb_min.z + k*vsize;

					if (ray_triangle (v1, v2, v3, origin, dir, &tf))
						if (voxels[i][j][k]) voxels[i][j][k] = 0;
						else voxels[i][j][k] = 1;
				}
			}
		}
	}
}

void vmodel::draw()
{
	glPushMatrix();
	glTranslatef(aabb_min.x, aabb_min.y, aabb_min.z);
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			for (int k = 0; k < z; k++) {
				if (voxels[i][j][k] == 1) {
					glPushMatrix();
					glTranslatef(i*vsize, j*vsize, k*vsize);
					glutSolidCube(vsize);
					glPopMatrix();
				}
			}
		}
	}
	glPopMatrix();
}
