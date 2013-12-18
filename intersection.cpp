#include "model.h"

#define EPSILON 0.000001

// ray-triangle intersection predicate (Möller–Trumbore algorithm)
 
int triangle_intersection(const vertex v1,   // triangle vertices
                          const vertex v2,
                          const vertex v3,
                          const vertex  o,   // ray origin
                          const vector  d,   // ray direction
						  float*        out)
{
	vector e1, e2, p, q, t;
	float det, inv_det, u, v, tf;
 
	// vectors of edges sharing V1
	e1 = vector(v1, v2);
	e2 = vector(v1, v3);

	// determinant calculation - also used for u parameter
	p = d % e2;

	// if det ~= zero, ray lies in plane of triangle
	det = e1 * p;

	// not culling
	if(det > -EPSILON && det < EPSILON) return 0;
	inv_det = 1.f / det;
 
	// distance from V1 to ray origin
	t = vector(v1, o);
 
	// u parameter calculation and bound test
	u = (t * p) * inv_det;
	// intersection lies outside of the triangle
	if(u < 0.f || u > 1.f) return 0;
 
	// v parameter test preparation
	q = t % e1;
 
	// v parameter calculation and bound test
	v = (d * q) * inv_det;

	// intersection outside of the triangle
	if(v < 0.f || u + v  > 1.f) return 0;
 
	tf = (e2 * q) * inv_det;
 
	if(tf > EPSILON) {
		*out = tf;
		return 1;
	}

	return 0;
}
