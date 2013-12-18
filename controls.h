// drawing mode
#define WIRE 0
#define SOLID 1

// both objects active
#define BOTH 2

struct rt {					// rotation-translation
	float rx, ry, rz;
	float tx, ty, tz;
};

extern struct rt rt;
extern int mode;
extern int phase;

void menu(int choice);
void mouse_motion(int x, int y);
void key_up(unsigned char key, int x, int y);
void key_down(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
