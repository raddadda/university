#include "glSetup.h"

#ifdef _WIN32
	#define _USE_MATH_DEFINES
#endif

#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
using namespace glm;

#include<vector>
#include<list>
#include <iostream>
#include<fstream>
using namespace std;

void init();
void quit();
void render(GLFWwindow* window);
void keyboard(GLFWwindow* window, int key, int code, int action, int mods);

bool readMesh(const char* filename);
void deleteMesh();

vec3 eye(3, 3, 3);
vec3 center(0, 0, 0);
vec3 up(0, 0, 1);

vec4 light(5.0, 5.0, 0.0, 1);

float AXIS_LENGTH = 1.75;
float AXIS_LINE_WIDTH = 2;

GLfloat bgColor[4] = { 1,1,1,1 };

int selection = 1;

GLUquadricObj* sphere = NULL;
GLUquadricObj* cylinder = NULL;
GLUquadricObj* disk = NULL;

bool pause = true;

float timeStep = 1.0f / 120;
float period = 8.0;
float theta = 0;
//Drawing parameters
bool depthTest = true;
bool polygonFill = true;

int main(int argc, char* argv[])
{
	vsync = 0;

	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL)return -1;

	glfwSetKeyCallback(window, keyboard);

	glEnable(GL_NORMALIZE);

	//키면 뒷면이 안보임
	glDisable(GL_CULL_FACE);

	reshape(window, windowW, windowH);

	init();

	float previous = (float)glfwGetTime();
	float elapsed = 0;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		float now= (float)glfwGetTime();
		float delta = now - previous;
		previous = now;

		elapsed += delta;

		if (elapsed > timeStep)
		{
			if (!pause)
			{
				theta += float(2.0 * M_PI) / period * elapsed;
			}
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

	readMesh("m01_bunny.off");

	cout << endl;
	cout << "keyboard Input: d for depth test on/off" << endl;
	cout << "keyboard Input: f for polygon fill on/off" << endl;
	cout << endl;
	cout << "Keyboard Input:1 for sphere" << endl;
	cout << "Keyboard Input:2 for cylinder" << endl;
	cout << "Keyboard Input:3 for Disk" <<  endl;
	cout << "Keyboard Input:4 for bunny" <<  endl;

}

void
quit()
{
	gluDeleteQuadric(sphere);
	gluDeleteQuadric(cylinder);
	gluDeleteQuadric(disk);

	deleteMesh();
}
//light
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

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE , GL_TRUE);

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

	gluSphere(sphere, radius, 36, 36);
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

	gluDisk(disk,inner_radius, outer_radius, 72, 5);
}

int nVertices = 0, nFaces = 0, nEdges = 0;
vec3* vertex = NULL;



int* face[3] = {NULL, NULL , NULL};

bool readMesh(const char* filename)
{
	ifstream is(filename);
	if (is.fail()) return false;

	
	char magicNumber[256];
	is >> magicNumber;

	is >> nVertices >> nFaces >> nEdges;
	cout << "# vertices = " << nVertices << endl;
	cout << "# faces = " << nFaces << endl;
	cout << "# edges = " << nEdges << endl;

	vertex = new vec3[nVertices];
	for (int i = 0; i < nVertices; i++) {
		is >> vertex[i].x >> vertex[i].y >> vertex[i].z;
		//a.push_back(i);
	}

	
	
	face[0] = new int[nFaces];
	face[1] = new int[nFaces];
	face[2] = new int[nFaces];
	

	list<int> edge1;
	list<int> edge2;

	int n;
	for (int i = 0; i < nFaces; i++)
	{
		is >> n >> face[0][i] >> face[1][i] >> face[2][i];

		//구현하지못하여 주석처리했습니다.
		//for (int i = 0; i < nFaces; i++)	
		//	edge1.push_back(face[0][i]);
		//	for (int j = 0; j < nFaces; j++) {
		//		edge2.push_back(face[2][j]);

		//	}
		
		//		for (int i = 0; i < nFaces; i++)
		//			for (int i = 0; i < nFaces; i++) {
		//				for (int j = 0; j < nFaces; j++)
		//					if ((face[0][i] == face[2][j]) && (face[0][j] == face[2][i]))
		//						edge2.remove(face[2][j]);
			
			
		if (n != 3) cout << "# vertices of the" << i << "-th faces = " << n << endl;
		
	}
	

	
	
	

	return true;
	
}

void
deleteMesh()
{
	if (vertex) { delete[] vertex; vertex = NULL; }
	if (face[0]) { delete[] face[0]; face[0] = NULL; }
	if (face[1]) { delete[] face[1]; face[1] = NULL; }
	if (face[2]) { delete[] face[2]; face[2] = NULL; }
}

void drawMesh()
{
	
	
		

	setDiffuseColor(vec3(0, 0, 0));

	glRotatef(90, 0, 0, 1);
	glRotatef(90, 1, 0, 0);

	glPointSize(3 * dpiScaling);
	
	glBegin(GL_POINTS);
	for (int i = 0; i < nVertices; i++) {
		glVertex3f(vertex[i].x, vertex[i].y, vertex[i].z);
		
		
	}
	glEnd();



}

void
render(GLFWwindow* window)
{
	if (depthTest) glEnable(GL_DEPTH_TEST);
	else           glDisable(GL_DEPTH_TEST);

	if (polygonFill) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else             glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	glDisable(GL_LIGHTING);
	drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH * dpiScaling);

	setupLight();

	setupMaterial();

	glRotatef(degrees(theta), 0, 1, 0);

	switch (selection)
	{
	case 1: drawSphere(1.0, vec3(0.95, 0.95, 0.95)); break;
	case 2: drawCylinder(1.0, 1.0, vec3(0.95, 0.95, 0.95)); break;
	case 3: drawDisk(0.5, 1.0, vec3(0.95, 0.95, 0.95));	break;
	case 4: drawMesh(); break;
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


		case GLFW_KEY_1: selection = 1; break;
		case GLFW_KEY_2: selection = 2; break;
		case GLFW_KEY_3: selection = 3; break;
		case GLFW_KEY_4: selection = 4; break;

		case GLFW_KEY_SPACE: pause = !pause; break;

		case GLFW_KEY_D: depthTest = !depthTest; break;
		
		case GLFW_KEY_F: polygonFill = !polygonFill; break;
		}
	}

}