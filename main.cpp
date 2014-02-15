#include <stdio.h>     // - Just for some ASCII messages

#include "visuals.h"   // Header file for our OpenGL functions
#include "controls.h"

#ifdef _WIN32
#include "gl/glut.h"   // - An interface and windows management library
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

////////////////// State Variables ////////////////////////
// float t=0.0; ///Time

/////////////// Main Program ///////////////////////////

int main(int argc, char* argv[])
{
    // initialize GLUT library state
    glutInit(&argc, argv);

    // Set up the display using the GLUT functions to
    // get rid of the window setup details:
    // - Use true RGB colour mode ( and transparency )
    // - Enable double buffering for faster window update
    // - Allocate a Depth-Buffer in the system memory or
    //   in the video memory if 3D acceleration available
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);


    // Define the main window size and initial position
    // (upper left corner, boundaries included)
    glutInitWindowSize(1600,1000);
    glutInitWindowPosition(0,0);

    // Create and label the main window
    glutCreateWindow("Voxelization-Simulation");

    // Configure various properties of the OpenGL rendering context
    setup();

    // Callbacks for the GL and GLUT events:

    // The rendering function
    glutDisplayFunc(render);
    glutReshapeFunc(resize);
    glutIdleFunc(idle);
    glutKeyboardFunc(key_down);
    glutKeyboardUpFunc(key_up);
    glutMouseFunc(mouse);
    glutMotionFunc(mouse_motion);

    glutCreateMenu(menu);
    glutAddMenuEntry("wire", WIRE);
    glutAddMenuEntry("solid", SOLID);

    // attach the menu to the right button
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //Enter main event handling loop
    glutMainLoop();
    return 0;
}
