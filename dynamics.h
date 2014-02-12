#include <vector>

#include "model.h"
#include "vmodel.h"

// particle class

class particle {
public:
    vector pos;
    vector vel;
public:
    particle(float x, float y, float z);
    void update(float dt, vector g);
};

// particle system class

class particle_system {
public:
    vector g;
    float dt;
    int ready;
    float radius;
    int current_frame;
    int max_frame_index;
    std::vector<particle> particles;
    std::vector< std::vector <vector> > frames;
    float x_min, x_max, y_min, y_max, z_min, z_max;
public:
    particle_system();
    particle_system(vmodel);
    void store_frame();
    void floor_hit(particle*);
    void update(int with_collisions);
    void draw();
    void run(int iterations, int with_collisions);
};
