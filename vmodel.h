#define VOXEL_RESOLUTION 20

// voxel model

class vmodel {
	int x, y, z; // voxels per dimension
	float vsize;
	char*** voxels;
	vertex aabb_min, aabb_max;
public:
	vmodel(model, int);
	void draw();
	void debug();
};
