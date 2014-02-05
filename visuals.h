//-------- Types ------------------------------------

struct rts;

//-------- Variables --------------------------------

// particle_system ps;

//-------- Functions --------------------------------

void render();
// The function responsible for drawing everything in the 
// OpenGL context associated to a window. 

void resize(int w, int h);
// Handle the window size changes and define the world coordinate 
// system and projection type

void setup();
// Set up the OpenGL state machine and create a light source

void idle();
// Idle function

void voxelize_reconstruct();

