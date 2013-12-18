#include <stdio.h>

#ifdef _WIN32
#include "gl/glut.h"
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

// load an RGB file as a texture
void load_rgb_texture(const char * filename, int width, int height, int wrap)
{
    GLuint texture;
    GLbyte* data;
    FILE* file;

    // open texture data
    file = fopen(filename, "rb");
    if (file == NULL) return;

    // allocate buffer
    width = 512;
    height = 512;
    data = (GLbyte*) malloc(width * height * 3);

    // read texture
    fread(data, width * height * 3, sizeof(GLbyte), file);
    fclose(file);

    // allocate and bind texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // linear inter- and extrapolation
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);
    
    // preprocess mipmaps
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

    // free buffer
    free(data);
}
