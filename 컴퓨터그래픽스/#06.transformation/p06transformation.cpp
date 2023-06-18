#include "glSetup.h"

#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif

#include<glm/glm.hpp>
#include<glm/gtx/string_cast.hpp>
#include<glm/gtc/type_ptr.hpp>
using namespace glm;


#include <iostream>
using namespace std;

void init();
void quit();
void render(GLFWwindow* window);
void keyboard(GLFWwindow* window, int key, int code, int action, int mods);


//camera configuation
vec3 eye(3.5, 3, 3.5);
vec3 center(0, 0, 0);
vec3 up(0, 1, 0);

//Light configuration
vec4 light(5.0, 5.0, 5.0, 1);

//Global coordinate frame
float AXIS_LENGTH = 3;
float AXIS_LINE_WIDTH = 2;

//colors
GLfloat bgColor[4] = { 1,1,1,1 };

//Selected example
int selection = 1;

//sphere,cylinder
GLUquadricObj* sphere = NULL;
GLUquadricObj* cylinder = NULL;
GLUquadricObj* disk = NULL;

// paly configuration
bool pause = false;

float timeStep = 1.0f / 120;
float period = 4.0f;

//Current frame
int frame = 0;

int main(int argc, char* argv[])
{
	vsync = 0;

	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL) return -1;

	glfwSetKeyCallback(window, keyboard);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);

	reshape(window, windowW, windowH);

	init();

	float previous = (float)glfwGetTime();
	float elapsed = 0;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		float now = (float)glfwGetTime();
		float delta = now - previous;
		previous = now;

		elapsed += delta;

		if (elapsed > timeStep)
		{
			if (!pause) frame += 1;

			elapsed = 0;
		}
		render(window);
		glfwSwapBuffers(window);
	}
	quit();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;


}

void
init()
{
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	//gluQuadricNormals(sphere, GLU_FLAT);
	gluQuadricOrientation(sphere, GLU_OUTSIDE);
	gluQuadricTexture(sphere, GL_FALSE);

	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricNormals(cylinder, GLU_SMOOTH);
	gluQuadricOrientation(cylinder, GLU_OUTSIDE);
	gluQuadricTexture(cylinder, GL_FALSE);

	disk = gluNewQuadric();
	gluQuadricDrawStyle(disk, GLU_FILL);
	gluQuadricNormals(disk, GLU_SMOOTH);
	gluQuadricOrientation(disk, GLU_OUTSIDE);
	gluQuadricTexture(disk, GL_FALSE);

	cout << endl;
	cout << "keyboard Input: space for play/pause" << endl;
	cout << endl;

	cout << "Keyboard Input:1 for ex1" << endl;
	cout << "Keyboard Input:2 for ex2" << endl;
	cout << "Keyboard Input:3 for ex3" << endl;
	cout << "Keyboard Input:4 for ex4" << endl;
	cout << "keyboard Input:5 for ex5" << endl;
	cout << "keyboard Input:6 for ex6" << endl;
	
}

void
quit()
{
	gluDeleteQuadric(sphere);
	gluDeleteQuadric(cylinder);
	gluDeleteQuadric(disk);


}

void
setupLight()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat ambient[4] = { 0.1f,0.1f,0.1f,1.0f };
	GLfloat diffuse[4] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat specular[4] = { 1.0f,1.0f,1.0f,1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, value_ptr(light));


}


void
setupMaterial()
{
	GLfloat mat_ambient[4] = { 0.1f,0.1f,0.1f,1.0f };
	GLfloat mat_specular[4] = { 0.5f,0.5f,0.5f,1.0f };
	GLfloat mat_shininess = 128;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}
void
setDiffuseColor(const vec3& color)
{
	GLfloat mat_diffuse[4] = { color[0],color[1],color[2],1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
}

void
drawSphere(float radius, const vec3& color)
{
	setDiffuseColor(color);

	gluSphere(sphere, radius, 72, 72);
}

void
drawCylinder(float radius, float height, const vec3& color)
{
	setDiffuseColor(color);
	gluCylinder(cylinder, radius, radius, height, 72, 5);
}

void drawDisk(float inner_radius, float outer_radius, const vec3& color)
{
	setDiffuseColor(color);

	gluDisk(disk, inner_radius, outer_radius, 72, 5);
}

void drawCube() {
	glBegin(GL_QUADS);

	glNormal3f(0, 0, 1);

	glVertex3f(0, 0, 1);
	glVertex3f(1, 0, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(0, 1, 1);

	//back
	glNormal3f(0, 0, -1);

	glVertex3f(0, 1, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(0, 0, 0);

	//Right
	glNormal3f(1, 0, 0);

	glVertex3f(1, 0, 1);
	glVertex3f(1, 0, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(1, 1, 1);

	//Left

	glNormal3f(-1, 0, 0);

	glVertex3f(0, 1, 1);
	glVertex3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);
	//Top

	glNormal3f(0, 1, 0);

	glVertex3f(0, 1, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(1, 1, 0);
	glVertex3f(0, 1, 0);

	//bottom
	glNormal3f(0, -1, 0);

	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(1, 0, 1);
	glVertex3f(0, 0, 1);

	glEnd();
}

void
ex1()
{
	float theta1 = frame * 4 / period;
	glm::vec3 axis(0, 0, 1);


	setDiffuseColor(glm::vec3(1, 1, 1));
	drawCube();
}

void
ex2()
{
	float s = 1.0f + 0.95f * sin(frame * 0.1f / period);

	glScalef(s, 1, 1);

	setDiffuseColor(glm::vec3(1, 1, 1));
	drawCube();
}

void
ex3() {
	float s = 1.0f + 0.95f * sin(frame * 0.1f / period);

	glScalef(s, s, 1);

	setDiffuseColor(glm::vec3(1, 1, 1));
	drawCube();
}

void ex4()
{
	float s = 1.0f + 0.95f * sin(frame * 0.1f / period);
	glm::vec3 pivot(0.5, 0.5, 0.5);


	glTranslatef(pivot.x, pivot.y, pivot.z);
	glScalef(s, 1, 1);
	glTranslatef(-pivot.x, -pivot.y, -pivot.z);

	setDiffuseColor(glm::vec3(1, 1, 1));
	drawCube();
	
}
void ex5()
{
	float s = 1.0f + 0.95f * sin(frame * 0.1f / period);
	glm::vec3 pivot(0.5, 0.5, 0.5);
	glm::vec3 direction(1, 1, 0);
	direction = normalize(direction);

	glm::vec3 axis = cross(glm::vec3(0, 0, 1), direction);
	float sinTheta = length(axis);
	float cosTheta = dot(glm::vec3(0, 0, 1), direction);
	float theta = (float)atan2(sinTheta, cosTheta) * float(180.0 / M_PI);
	axis /= sinTheta;

	glTranslatef(pivot.x, pivot.y, pivot.z);
	glRotatef(theta, axis.x, axis.y, axis.z);
	glScalef(s, 1, 1);
	glRotatef(-theta, axis.x, axis.y, axis.z);
	glTranslatef(-pivot.x, -pivot.y, -pivot.z);

	setDiffuseColor(glm::vec3(1, 1, 1));
	drawCube();


}
void ex6()
{
	float s = 1.0f + 0.95f * sin(frame * 0.1f / period);
	glm::vec3 pivot(0.5, 0.5, 0.5);


	//Identity matrix
	glm::mat4 M(1.0);

	M = glm::translate(M, pivot);

	M = glm::scale(M, glm::vec3(s, 1, 1));

	M = glm::translate(M, -pivot);



	glMultMatrixf(value_ptr(M));

	setDiffuseColor(glm::vec3(1, 1, 1));
	drawCube();


}





void
render(GLFWwindow* window)
{

	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	//Axes
	glDisable(GL_LIGHTING);
	drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH * dpiScaling);

	setupLight();

	setupMaterial();

	switch (selection)
	{
	case 1: ex1(); break;
	case 2: ex2(); break;
	case 3: ex3(); break;
	case 4: ex4(); break;
	case 5: ex5(); break;
	case 6: ex6(); break;
	

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

		case GLFW_KEY_SPACE: pause = !pause; break;

		case GLFW_KEY_1: selection = 1; break;
		case GLFW_KEY_2: selection = 2; break;
		case GLFW_KEY_3: selection = 3; break;
		case GLFW_KEY_4: selection = 4; break;
		case GLFW_KEY_5: selection = 5; break;
		case GLFW_KEY_6: selection = 6; break;
		case GLFW_KEY_7: selection = 7; break;
		case GLFW_KEY_8: selection = 8; break;
		case GLFW_KEY_9: selection = 9; break;
		}
	}

}

