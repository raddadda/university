#ifndef __GL_SETUP_H_
#define __GL_SETUP_H_

#if defined(__APPLE__)&& defined(__MACH__)
	#include <OpenGL/glu.h>

#else
	#include <windows.h>
	#include <GL/glu.h>

#endif

#include <GLFW/glfw3.h>
//#pragma comment(lib, "glfw3.lib")


extern float screenScale;   //â�� ũ��
extern int screenW, screenH;// 
extern int windowW;
extern int windowH;
extern float aspect; //windowW/windowH
extern float dpiScaling;

extern int vsync;

extern bool perspectiveView;
extern float fovy;
extern float nearDist;
extern float farDist;

GLFWwindow* initializeOpenGL(int argc, char* argv[], GLfloat bg[4]);
void	reshape(GLFWwindow* window, int w, int h);
void	setupProjectionMatrix();

void drawAxes(float l, float w);

#endif