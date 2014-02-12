#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <vector>

#include "model.h"
#include "vmodel.h"
#include "visuals.h"
#include "controls.h"
#include "texture.h"
#include "mcubes.h"
#include "mclut.h"

#ifdef _WIN32
#include "gl/glut.h"
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

// Global vertex indexing is based upon voxel addressing. Each voxel
// provides address to the 3 neighboring vertices towards each
// positive axis direction. Thus, in total there are 3x vertices as
// voxels. If e is 0, 8 or 3, then the current voxel (at base index)
// provides the base for addressing, else the function is called
// recursively with the appropriate voxel as base, and the
// corresponding edge index with respect to the new base

int vmodel::global_vi(int i, int j, int k, int e)
{
    int base = 3*(i*y*z + j*z + k);
	
    switch(e) {
		
    case 8: return base;
    case 3: return base+1;
    case 0: return base+2;
		
    case 9: return global_vi(i, j, k+1, 8);
    case 1: return global_vi(i, j, k+1, 3);
		
    case 10: return global_vi(i, j+1, k+1, 8);

    case 2: return global_vi(i, j+1, k, 0);
    case 11: return global_vi(i, j+1, k, 8);
		
    case 4: return global_vi(i+1, j, k, 0);
    case 7: return global_vi(i+1, j, k, 3);
		
    case 5: return global_vi(i+1, j, k+1, 3);
		
    case 6: return global_vi(i+1, j+1, k, 0);
    };
}

model::model(vmodel vm)
{
    int x, y, z;
    x = vm.x; y = vm.y; z = vm.z;
    float vsize;
    vsize = vm.vsize;

    float x_min = vm.aabb_min.x;
    float y_min = vm.aabb_min.y;
    float z_min = vm.aabb_min.z;

    // populate temporary vertices (on voxel edges)
    float vx, vy, vz;
    for (int i = 0; i < x; i++) {
	for (int j = 0; j < y; j++) {
	    for (int k = 0; k < z; k++) {
		vx = i*vsize; vy = j*vsize; vz = k*vsize;
		vertices.push_back(vertex(x_min + vx + 0.5*vsize,
					  y_min + vy,
					  z_min + vz));
		vertices.push_back(vertex(x_min + vx,
					  y_min + vy + 0.5*vsize,
					  z_min + vz));
		vertices.push_back(vertex(x_min + vx,
					  y_min + vy,
					  z_min + vz + 0.5*vsize));
	    }
	}
    }

    char*** voxels = vm.voxels;

    face f;

    // current cube vertices
    unsigned char cube;

    // current cube has min = voxels[i][j][k] and max =
    // voxels[i+1][j+1][k+1]
    for (int i = 0; i < x-1; i++) {
	for (int j = 0; j < y-1; j++) {
	    for (int k = 0; k < z-1; k++) {
		cube = 0;
		if (voxels[i  ][j  ][k  ]) cube |= 1;
		if (voxels[i  ][j  ][k+1]) cube |= 2;
		if (voxels[i  ][j+1][k+1]) cube |= 4;
		if (voxels[i  ][j+1][k  ]) cube |= 8;
		if (voxels[i+1][j  ][k  ]) cube |= 16;
		if (voxels[i+1][j  ][k+1]) cube |= 32;
		if (voxels[i+1][j+1][k+1]) cube |= 64;
		if (voxels[i+1][j+1][k  ]) cube |= 128;

		for (int t=0; face_lut[cube][t]!=-1; t+=3) {
		    f = face(vm.global_vi(i, j, k, face_lut[cube][t]),
			     vm.global_vi(i, j, k, face_lut[cube][t+1]),
			     vm.global_vi(i, j, k, face_lut[cube][t+2]));
		    faces.push_back(f);
		}
	    }
	}
    }

    this->compact();
    this->face_normals();
    this->vertex_normals();
    this->compute_angles();
    this->center();
}

