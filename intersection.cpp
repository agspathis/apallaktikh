#include "model.h"

#define EPSILON 0.0001

// ray-triangle intersection predicate (Möller–Trumbore algorithm)
 
int ray_triangle(const vertex v0,   // triangle vertices
				 const vertex v1,
				 const vertex v2,
				 const vertex  o,   // ray origin
				 const vector  d,   // ray direction
				 float*        out)
{
	vector e0, e1, p, q, t;
	float det, inv_det, u, v, tf;
 
	// vectors of edges sharing V0
	e0 = vector(v0, v1);
	e1 = vector(v0, v2);

	// determinant calculation - also used for u parameter
	p = d % e1;

	// if det ~= zero, ray lies in plane of triangle
	det = e0 * p;

	// not culling
	if(det > -EPSILON && det < EPSILON) return 0;
	inv_det = 1.f / det;
 
	// distance from V0 to ray origin
	t = vector(v0, o);
 
	// u parameter calculation and bound test
	u = (t * p) * inv_det;
	// intersection lies outside of the triangle
	if(u < 0.f || u > 1.f) return 0;
 
	// v parameter test preparation
	q = t % e0;
 
	// v parameter calculation and bound test
	v = (d * q) * inv_det;

	// intersection outside of the triangle
	if(v < 0.f || u + v  > 1.f) return 0;
 
	tf = (e1 * q) * inv_det;
 
	if(tf > EPSILON) {
		*out = tf;
		return 1;
	}

	return 0;
}
