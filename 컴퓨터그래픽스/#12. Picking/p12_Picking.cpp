#include "glSetup.h"

#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
using namespace glm;

#include <iostream>
#include <fstream>

using namespace std;

void init();
void quit();
void render(GLFWwindow* window);
void keyboard(GLFWwindow* window, int key, int code, int action, int mods);
void mouseButton(GLFWwindow* window, int button, int action, int mods);

vec3 eyeTopView(0, 10, 0);
vec3 upTopView(0, 0, -1);

vec3 eyeFrontView(0, 0, 10);
vec3 upFrontView(0, 1, 0);

vec3 eyeBirdView(0, -10, 4);
vec3 upBirdView(0, 1, 0);
vec3 center(0, 0, 0);

vec4 light(0.0, 0.0, 0.8, 1);

//Global coordinate frame
bool axes = true;

float AXIS_LENGTH = 1.25;
float AXIS_LINE_WIDTH = 2;

//colors
GLfloat bgColor[4] = { 1,1,1,1 };

int view = 1;

int picked = -1;

int 
main(int argc, char* argv[])
{
	fovy = 16.1f;

	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL) return -1;

	glfwSetKeyCallback(window, keyboard);
	glfwSetMouseButtonCallback(window, mouseButton);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);

	reshape(window, windowW, windowH);

	init();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		render(window);
		glfwSwapBuffers(window);
	}
	quit();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;

}

GLUquadricObj* sphere = NULL;

void
init()
{
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluQuadricOrientation(sphere, GLU_OUTSIDE);
	gluQuadricTexture(sphere, GL_FALSE);

	cout << endl;
	cout << "Keyboard input: x for axes on/off" << endl;
	cout << "Keyboard input: v for the top, front, bird-eye view" << endl;
	cout << "Mouse button down: Select an object" << endl;
}

void
quit() {
	gluDeleteQuadric(sphere);
}

void
setupColoredMaterial(const vec3& color)
{
	GLfloat mat_ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat mat_diffuse[4] = { color[0], color[1],color[2], 1.0f };
	GLfloat mat_specular[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat mat_shininess = 100.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void
drawSphere(float radius, int slices, int stacks)
{
	gluSphere(sphere, radius, slices, stacks);
}

void
setupLight(const vec4& p)
{
	GLfloat ambient[4] = { 0.1f,0.1f,0.1f,1.0f };
	GLfloat diffuse[4] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat specular[4] = { 1.0f,1.0f,1.0f,1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, value_ptr(p));
}

void
render(GLFWwindow* window)
{

	glInitNames();
	glPushName(-1);

	//background color
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	vec3 eye(0), up(0);
	switch (view)
	{
	case 0: eye = eyeTopView; up = upTopView; break;
	case 1: eye = eyeFrontView; up = upFrontView; break;
	case 2: eye = eyeBirdView; up = upBirdView; break;
	}

	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);


	if (axes) 
	{
		glDisable(GL_LIGHTING);
		drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH*dpiScaling);
	}


	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	setupLight(light);

	glScalef(0.4f, 0.4f, 0.4f);

	vec3 u[3];
	vec3 v[4];
	u[0] = vec3(0, 1, 0) * 2.0f; u[1] = vec3(0, 0, 0); u[2] = -u[0];
	v[0] = -vec3(1, 0, 0) * 4.0f; v[1] = vec3(0, 0, 0); v[2] = -v[0];

	for (int i = 0; i < 3; i++)
	{
		glPushMatrix();
		glTranslatef(u[i].x, u[i].y, u[i].z);

		for (int j = 0; j < 3; j++)
		{
			glPushMatrix();
			glTranslatef(v[j].x, v[j].y, v[j].z);

			if (picked == (3 * i + j))	setupColoredMaterial(vec3(0, 0, 1));
			else                        setupColoredMaterial(vec3(1, 1, 1));

			glLoadName(3 * i + j);

			drawSphere(0.7f, 64, 64);

			glPopMatrix();
		}
		glPopMatrix();
		
	}
}

void
keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GL_TRUE); break;

		case GLFW_KEY_X: axes = !axes; break;

		case GLFW_KEY_V: view = (view + 1) % 3; break;
		}
	}
}

int
findNearestHits(int hits, GLuint selectBuffer[64])
{
	bool diagnosis = true;

	if (diagnosis) cout << "hits = " << hits << endl;
	int name = -1;
	float nearest = 2.0;

	int index = 0;


	for (int i = 0; i < hits; i++)
	{

		int n = selectBuffer[index + 0];
		float z1 = (float)selectBuffer[index + 1] / 0xffffffff;
		float z2 = (float)selectBuffer[index + 2] / 0xffffffff;

		if (z1 < nearest) { nearest = z1; name = selectBuffer[index + 3]; }

		if (diagnosis)
		{
			cout << "\n# of names = " << n << endl;
			cout << "\nz1 = " << z1 << endl;
			cout << "\nz2 = " << z2 << endl;
			cout << "\tnames:";
			for (int j = 0; j < n; j++)
				cout << selectBuffer[index + 3 + j] << "";
			cout << endl;


		}

		index += (3 + n);

	}
	if (diagnosis) cout << "picked = " << name << endl;

	return name;
}


int selectObjects(GLFWwindow* window, double x, double y)
{
	double delX = 2;
	double delY = 2;

	GLuint selectBuffer[64];
	glSelectBuffer(64, selectBuffer);


	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glLoadIdentity();

	gluPickMatrix(x, viewport[3] - y, delX, delY, viewport);
	setupProjectionMatrix();

	glRenderMode(GL_SELECT);

	render(window);


	GLint hits = glRenderMode(GL_RENDER);
	int name = findNearestHits(hits, selectBuffer);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	return name;


}


void mouseButton(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
	{
		double xs, ys;
		glfwGetCursorPos(window, &xs, &ys);

		double x = xs * dpiScaling;
		double y = ys * dpiScaling;

		picked = selectObjects(window, x, y);


	}

}