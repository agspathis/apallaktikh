#ifndef VMODEL_H
#define VMODEL_H

#define VOXEL_RESOLUTION 20
#define VELEM_SPHERE 0
#define VELEM_CUBE 1

// voxel model

class vmodel {
public:
    int x, y, z; // voxels per dimension
    float vsize;
    char*** voxels;
    vertex aabb_min, aabb_max;
public:
    vmodel(model, int);
    /* computation of global vertex index for marching cubes */
    int global_vi(int, int, int, int);
    void draw(int, int);
    void debug();
};

#endif
