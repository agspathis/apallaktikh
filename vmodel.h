#define VOXEL_RESOLUTION 30

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
	void draw();
	void debug();
};
