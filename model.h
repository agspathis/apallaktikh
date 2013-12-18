////////////
// vertex //
////////////

class vertex {
public:
    // position + vector
    float x, y, z;
public:
    vertex();
    vertex(float, float, float);
};


////////////
// vector //
////////////

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

//////////
// face //
//////////

class face {
public:
	int iv1, iv2, iv3;
	vector n;
public:
	face();
	face(int, int, int);
};

///////////
// model //
///////////

class model {
	std::vector<vertex> vertices;
	std::vector<face> faces;
	std::vector<vector> vnormals;
	std::vector<vector> fnormals;
public:
	model(const char*);
	void center();
	void face_normals();
	void vertex_normals();
	void draw(int);
	void debug();
};
