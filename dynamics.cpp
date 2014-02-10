#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "dynamics.h"
#include "controls.h"

#ifdef _WIN32
#include "gl/glut.h"
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

float jm = 0.02;		// jitter magnitude

int rand_sign()
{
    if (rand() % 2) return 1;
    else return -1;
}

particle::particle(float x, float y, float z)
{
    pos = vector(x, y, z);
    vel = vector(jm*rand_sign(), 0, jm*rand_sign());
}

void particle::update(float dt, vector g)
{
    pos += dt*vel;
    vel += dt*g;
}

particle_system::particle_system() {}

void particle_system::store_frame()
{
    std::vector<vector> current_positions;
    
    // store frame
    for(int i = 0; i < particles.size(); i++)
	current_positions.push_back(particles[i].pos);
    
    frames.push_back(current_positions);
    
}

particle_system::particle_system(vmodel vm)
{
    current_frame = 0;

    radius = vm.vsize/2;

    // convert voxels to particles
    for (int i = 0; i < vm.x; i++) {
	for (int j = 0; j < vm.y; j++) {
	    for (int k = 0; k < vm.z; k++) {
		if (vm.voxels[i][j][k])
		    particles.push_back(particle(vm.aabb_min.x + i*vm.vsize,
						 vm.aabb_min.y + j*vm.vsize,
						 vm.aabb_min.z + k*vm.vsize));
	    }
	}
    }

    this->store_frame();
    
    x_min = 1.1 * vm.aabb_min.x;
    y_min = 1.1 * vm.aabb_min.y;
    z_min = 1.1 * vm.aabb_min.z;

    x_max = 1.1 * vm.aabb_max.x;
    y_max = 1.1 * vm.aabb_max.y;
    z_max = 1.1 * vm.aabb_max.z;
    
    g = vector(0, -1, 0);
    dt = sqrt(pow(radius, 2)/(32*(x_max - x_min)));

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
	p2->pos += adjustment*axis;
    }
}

void particle_system::wall_hit(particle* p)
{
    vector pos = p->pos;

    if (pos.j < y_min + radius) {
	p->pos.j = y_min + radius;
	p->vel.j = - p->vel.j;
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
	for(int i = 0; i < particles.size()-1; i++) {
	    for(int j = i+1; j < particles.size(); j++) {
		collision(&particles[i], &particles[j], radius);
	    }
	}

	// wall hit handling
	for(int i = 0; i < particles.size(); i++)
	    wall_hit(&particles[i]);

    }

    this->store_frame();
}

void particle_system::draw_frame()
{
    std::vector<vector> frame = frames[current_frame];

    for (int i=0; i<frame.size(); i++) {
	glPushMatrix();
	glTranslatef(frame[i].i, frame[i].j, frame[i].k);
	glutSolidSphere(radius, 8, 5);
	glPopMatrix();
    }

    current_frame += frame_step;
    
    // clamp to range 0 .. frames.size()-1
    if (current_frame < 0)
	current_frame = 0;

    if (current_frame > (frames.size()-1))
	current_frame = frames.size()-1;
}


void particle_system::run(int iterations, int with_collisions)
{
    int last_progress_printed = 0;
    int current_progress;
    
    for (int i=0; i<iterations; i++) {
	this->update(with_collisions);
	
	current_progress = i*100/iterations;
	if (current_progress != last_progress_printed &&
	    ! (current_progress % 10)) {
	    printf("%d...", current_progress);
	    last_progress_printed = current_progress;
	    fflush(stdout);
	}
    }
    printf("done\n");
}
