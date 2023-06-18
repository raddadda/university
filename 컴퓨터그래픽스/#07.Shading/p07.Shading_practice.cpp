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

bool readMesh(const char* filename);
void deleteMesh();

//camera configuation
vec3 eye(3, 3, 3);
vec3 center(0, 0, 0);
vec3 up(0, 1, 0);

//Light configuration
vec4 light(5.0, 5.0, 0.0, 1);

//Global coordinate frame
bool axes = true;

float AXIS_LENGTH = 3;
float AXIS_LINE_WIDTH = 2;

//colors
GLfloat bgColor[4] = { 1,1,1,1 };

//control variable
bool smooth = true;

//Selected example
int selection = 1;

//sphere,cylinder
GLUquadricObj* sphere = NULL;


// paly configuration
bool pause = false;

float timeStep = 1.0f / 120;
float period = 4.0f;

//Drawing parameters
bool polygonFill = true;

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
	gluQuadricOrientation(sphere, GLU_OUTSIDE);
	gluQuadricTexture(sphere, GL_FALSE);

	readMesh("m01_bunny.off");

	cout << endl;
	cout << "keyboard Input: space for play/pause" << endl;
	cout << "keyboard Input: s for turn on/of smooth shading" << endl;
	cout << "keyboard Input: f for polygon fill on/off" << endl;
	cout << endl;

	cout << "Keyboard Input:1 for sphere with 16 slices and 16 stacks" << endl;
	cout << "Keyboard Input:2 for sphere with 32 slices and 32 stacks" << endl;
	cout << "Keyboard Input:3 for sphere with 64 slices and 64 stacks" << endl;
	cout << "Keyboard Input:4 for sphere with 128 slices and 128 stacks" << endl;
	cout << "keyboard Input:5 for flat approximation of a bunny" << endl;
	cout << "keyboard Input:6 for smooth approximation of a bunny" << endl;

}
void
quit()
{
	gluDeleteQuadric(sphere);

	deleteMesh();
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
drawSphere(float radius, int slices, int stacks, const vec3& color)
{
	setDiffuseColor(color);

	gluSphere(sphere, radius, slices, stacks);
}

int nVertices = 0, nFaces = 0, nEdges = 0;
glm::vec3* vertex = NULL;
glm::vec3* vnormal = NULL;
glm::vec3* fnormal = NULL;
int* face[3] = { NULL,NULL,NULL };

bool
readMesh(const char* filename)
{
	ifstream is(filename);
	if (is.fail()) return false;

	char magicNumber[256];
	is >> magicNumber;

	is >> nVertices >> nFaces >> nEdges;
	cout << "# vertices= " << nVertices << endl;
	cout << "# faces = " << nFaces << endl;

	vertex = new glm::vec3[nVertices];
	for (int i = 0; i < nVertices; i++)
		is >> vertex[i].x >> vertex[i].y >> vertex[i].z;

	vnormal = new glm::vec3[nVertices];
	for (int i = 0; i < nVertices; i++)
		vnormal[i] = glm::vec3(0, 0, 0);

	fnormal = new glm::vec3[nFaces];

	face[0] = new int[nFaces];
	face[1] = new int[nFaces];
	face[2] = new int[nFaces];


	int n;
	for (int i = 0; i < nFaces; i++)
	{
		is >> n >> face[0][i] >> face[1][i] >> face[2][i];


		glm::vec3 v1 = vertex[face[1][i]] - vertex[face[0][i]];
		glm::vec3 v2 = vertex[face[2][i]] - vertex[face[0][i]];
		glm::vec3 v = normalize(cross(v1, v2));

		fnormal[i] = v;

		vnormal[face[0][i]] += v;
		vnormal[face[1][i]] += v;
		vnormal[face[2][i]] += v;

	}

	for (int i = 0; i < nVertices; i++)
		vnormal[i] = normalize(vnormal[i]);

	return true;
}

void deleteMesh()
{

	if (vertex) { delete[] vertex; vertex = NULL; }
	if (vnormal) { delete[] vnormal; vnormal = NULL; }
	if (fnormal) { delete[] fnormal; fnormal = NULL; }
	if (face[0]) { delete[] face[0]; face[0] = NULL; }
	if (face[1]) { delete[] face[1]; face[1] = NULL; }
	if (face[2]) { delete[] face[2]; face[2] = NULL; }

}

void
drawFlatMesh()
{

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < nFaces; i++)
	{
		glNormal3fv(value_ptr(fnormal[i]));
		for (int j = 0; j < 3; j++)
			glVertex3fv(value_ptr(vertex[face[j][i]]));
	}
	glEnd();

}



void
drawSmoothMesh()
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < nFaces; i++)
		for (int j = 0; j < 3; j++)
		{
			glNormal3fv(value_ptr(vnormal[face[j][i]]));
			glVertex3fv(value_ptr(vertex[face[j][i]]));
		}
	glEnd();

}

void
render(GLFWwindow* window)
{
	if (polygonFill) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);


	if (axes)
	{

		glDisable(GL_LIGHTING);
		drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH * dpiScaling);

	}

	setupLight();

	if (smooth) glShadeModel(GL_SMOOTH);
	else glShadeModel(GL_FLAT);

	setupMaterial();

	float theta = frame * 4 / period;
	glm::vec3 axis(0, 1, 0);
	glRotatef(theta, axis.x, axis.y, axis.z);


	switch (selection)
	{
	case 1: drawSphere(1.0, 16, 16, vec3(0.95, 0.95, 0.95)); break;
	case 2: drawSphere(1.0, 32, 32, vec3(0.95, 0.95, 0.95)); break;
	case 3: drawSphere(1.0, 64, 64, vec3(0.95, 0.95, 0.95)); break;
	case 4: drawSphere(1.0, 128, 128, vec3(0.95, 0.95, 0.95)); break;
	case 5: drawFlatMesh();									 break;
	case 6: drawSmoothMesh();								 break;


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

		case GLFW_KEY_SPACE: pause = !pause; break;

		case GLFW_KEY_S: smooth = !smooth; break;

		case GLFW_KEY_F: polygonFill = !polygonFill; break;

		case GLFW_KEY_1: selection = 1; break;
		case GLFW_KEY_2: selection = 2; break;
		case GLFW_KEY_3: selection = 3; break;
		case GLFW_KEY_4: selection = 4; break;
		case GLFW_KEY_5: selection = 5; break;
		case GLFW_KEY_6: selection = 6; break;

		}

	}
}

