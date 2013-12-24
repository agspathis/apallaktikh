#include <vector>


// vertex

class vertex {
public:
    // position + vector
    float x, y, z;
public:
    vertex();
    vertex(float, float, float);
	inline vertex operator -= (vertex v)
    {
	x -= v.x;
	y -= v.y;
	z -= v.z;
    };
};



// vector

class vector {
public:
    // vectorized unit vector coefficients
    float i, j, k;

public:
    vector();
    vector(float, float, float);
    vector(vertex, vertex);
    void flip();
    void normalize();
    void vector_print();
    inline vector operator += (vector v)
    {
	i += v.i;
	j += v.j;
	k += v.k;
    };

};

// operator overloading

// addition

inline vector operator + (vector lv, const vector& rv)
{
    lv += rv;
    return lv;
}

// dot product

inline float operator * (const vector& lv, const vector& rv)
{
    float ip;
    ip = lv.i * rv.i + lv.j * rv.j + lv.k * rv.k;
    return ip;
}

// cross product

inline vector operator % (const vector& u, const vector& v)
{
    vector cp;
    cp.i = u.j * v.k - u.k * v.j;
    cp.j = u.k * v.i - u.i * v.k;
    cp.k = u.i * v.j - u.j * v.i;
    return cp;
};


// face

class face {
public:
	int iv1, iv2, iv3;
	vector n;
public:
	face();
	face(int, int, int);
};


// model

class model {
public:
	std::vector<vertex> vertices;
	std::vector<face> faces;
	std::vector<vector> vnormals;
	std::vector<vector> fnormals;
	vertex aabb_min, aabb_max;
public:
	model(const char*);
	void center();
	void face_normals();
	void vertex_normals();
	void draw(int);
	void debug();
};

// voxel array

/* class varray { */
/* public: */
/* 	std::vector<char> array; */
/* 	int x, y, z; // dimensions */
/* public: */
/* 	varray(); */
/* 	varray(int, int, int); */
/* 	int index(int, int, int); */
/* 	char get(int, int, int); */
/* 	void flip(int, int, int); */
/* }; */

