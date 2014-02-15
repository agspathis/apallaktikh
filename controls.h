// drawing mode
#define WIRE 0
#define SOLID 1

struct rt {			// rotation-translation
    float rx, ry, rz;
    float tx, ty, tz;
};

extern struct rt rt;
extern int mode;
extern int view;
extern float distance;
extern int texture_enabled;
extern int vres;
extern int vres_step;

/* simulation */
extern int ff_sim;
extern int c_sim;
extern int sim_pause;
extern int frame_step;


void menu(int choice);
void mouse_motion(int x, int y);
void key_up(unsigned char key, int x, int y);
void key_down(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouse_wheel(int wheel, int direction, int x, int y);
