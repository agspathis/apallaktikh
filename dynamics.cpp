#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "dynamics.h"

#ifdef _WIN32
#include "gl/glut.h"
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

particle::particle(float x, float y, float z)
{
    pos = vector(x, y, z);
    vel = vector();
}

void particle::update(float dt, vector g)
{
    pos += dt*vel;
    vel += dt*g;
}

particle_system::particle_system(vmodel vm)
{
    radius = vm.vsize;

    // convert voxels to particles
    for (int i = 0; i < vm.x; i++) {
	for (int j = 0; j < vm.y; j++) {
	    for (int k = 0; k < vm.z; k++) {
		if (vm.voxels[i][j][k])
		    particles.push_back(particle(i*vm.vsize, j*vm.vsize, k*vm.vsize));
	    }
	}
    }

    x_min = 1.1 * vm.aabb_min.x;
    y_min = 1.1 * vm.aabb_min.y;
    z_min = 1.1 * vm.aabb_min.z;

    x_max = 1.1 * vm.aabb_max.x;
    y_max = 1.1 * vm.aabb_max.y;
    z_max = 1.1 * vm.aabb_max.z;
    
    g = vector(0, -1, 0);
    dt = sqrt(pow(radius, 2)/(16*(x_max - x_min)));

}

void collision(particle* p1, particle* p2, float r)
{
    vector axis = p2->pos - p1->pos; // collision axis
    float dist = axis.magn();

    if (dist < 2*r) {
	axis.normalize();
	vector v1 = p1->vel;
	vector v2 = p2->vel;

	vector v1a, v1n, v2a, v2n;

	v1a = (v1 * axis) * axis;
	v2a = (v2 * axis) * axis;

	// normal part remains the same
	v1n = v1 - v1a;
	v2n = v2 - v2a;

	// assume equal masses for p1, p2 => exchange of velocities
	p1->vel = v1n + v2a;
	p2->vel = v2n + v1a;

	// position adjustment
	float adjustment = (2*r - dist)/2;
	p1->pos -= adjustment*axis;
	p1->pos += adjustment*axis;
    }
}

void particle_system::wall_hit(particle* p)
{
    vector pos = p->pos;

    if (pos.i > x_max) {
	p->pos.i = x_max - radius;
	p->vel.i = - p->vel.i;
    }
    if (pos.j > y_max) {
	p->pos.j = y_max - radius;
	p->vel.j = - p->vel.j;
    }
    if (pos.k > z_max) {
	p->pos.k = z_max - radius;
	p->vel.k = - p->vel.k;
    }

    if (pos.i < x_min) {
	p->pos.i = x_min + radius;
	p->vel.i = - p->vel.i;
    }
    if (pos.j < y_min) {
	p->pos.j = y_min + radius;
	p->vel.j = - p->vel.j;
    }
    if (pos.k < z_min) {
	p->pos.k = z_min + radius;
	p->vel.k = - p->vel.k;
    }
}

void particle_system::update(int with_collisions)
{
    // move for timestep
    for(int i = 0; i < particles.size(); i++)
	particles[i].update(dt, g);

    // wall hit handling
    for(int i = 0; i < particles.size(); i++)
	wall_hit(&particles[i]);

    if (with_collisions) {

	// particle-particle collision handling
	for(int i = 0; i < particles.size(); i++) {
	    for(int j = i; j < particles.size(); j++) {
		collision(&particles[i], &particles[j], radius);
	    }
	}

	// wall hit handling
	for(int i = 0; i < particles.size(); i++)
	    wall_hit(&particles[i]);

    }

    std::vector<vector> current_positions;

    // store frame
    for(int i = 0; i < particles.size(); i++)
	current_positions.push_back(particles[i].pos);

    frames.push_back(current_positions);
}

void particle_system::draw_frame(int frame_index)
{
    std::vector<vector> frame = frames[frame_index];

    for (int i=0; i<frame.size(); i++) {
	glPushMatrix();
	glTranslatef(frame[i].i, frame[i].j, frame[i].k);
	glutSolidSphere(radius, 8, 5);
	glPopMatrix();
    }
}


void particle_system::run(int iterations, int with_collisions)
{
    for (int i=0; i<iterations; i++)
	this->update(with_collisions);
}
