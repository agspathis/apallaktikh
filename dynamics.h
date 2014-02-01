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
    float radius;
    std::vector<particle> particles;
    std::vector< std::vector <vector> > frames;
    float x_min, x_max, y_min, y_max, z_min, z_max;
public:
    particle_system(vmodel);
    void wall_hit(particle*);
    void update(int with_collisions);
    void draw_frame(int frame_index);
    // void draw_container();
    void run(int iterations, int with_collisions);
};
