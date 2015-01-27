#ifndef SHADERUTILS_H
#define SHADERUTILS_H


#include <GL/glew.h>
#include <QGLWidget>
#include <cstdio>

using namespace std;

char* fileRead(const char* filename);
void glLog(GLuint object);
GLuint createShader(const char* filename, GLenum type);
#endif
