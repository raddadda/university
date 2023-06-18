#include "glSetup.h"

#ifdef _WIN32
	#define _USE_MATH_DEFINES
#endif

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

vec3 eyeTopView(0, 10, 0);
vec3 upTopView(0, 0, -1);

vec3 eyeFrontView(0, 0, 10);
vec3 upFrontView(0, 1, 0);

vec3 eyeBirdView(0, -10, 4);
vec3 upBirdView(0, 1, 0);
vec3 center(0, 0, 0);

vec4 lightInitialP(0.0, 0.0, 0.8, 1);

//Global coordinate frame
bool axes = true;

float AXIS_LENGTH = 1.25;
float AXIS_LINE_WIDTH = 2;

//colors
GLfloat bgColor[4] = { 1,1,1,1 };

int view = 1;
int selection = 1;

bool rotationObject = false;
bool rotationLight = false;

float thetaModel = 0;
float thetaLight[3];

bool lightOn[3];
bool attenuation = false;

bool exponent = false;
float exponentInitial = 0.0;
float exponentValue = exponentInitial;
float exponentNorm = exponentValue / 128.0f;

bool cutoff = false;
float cutoffMax = 60;
float cutoffInitial = 30.0;
float cutoffValue = cutoffInitial;
float cutoffNorm = cutoffValue / cutoffMax;

int material = 0;

bool pause = true;

float timeStep = 1.0f / 120;
float period = 4.0f;

int frame = 0;

void
reinitialize() {
	frame = 0;

	lightOn[0] = false;
	lightOn[1] = true;
	lightOn[2] = false;
	 
	thetaModel = 0;
	for (int i = 0; i < 3; i++)
		thetaLight[i] = 0;

	exponentValue = exponentInitial;
	exponentNorm = exponentValue / 128.0f;

	cutoffValue = cutoffInitial;
	cutoffNorm = cutoffValue / cutoffMax;
	
}

void
animate()
{

	frame += 1;


	if (rotationLight)
		{
		for(int i =0; i<3; i++)
			if(lightOn[i]) thetaLight[i] += 4/period;
		}

	if (rotationObject) thetaModel += 4 / period;

	if (lightOn[2] && exponent)
	{
		exponentNorm += float(radians(4.0 / period) / M_PI);
		exponentValue = float(128.0f * (acos(cos(exponentNorm * M_PI)) / M_PI));

	}
	if(lightOn[2]&&cutoff)
	{
		cutoffNorm += float(radians(4.0 / period) / M_PI);
		cutoffValue = float(cutoffMax * (acos(cos(cutoffNorm * M_PI)) / M_PI));
	}

}

int main(int argc, char* argv[])
{
	vsync = 0;

	fovy = 16.1f;

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
			if (!pause) animate();

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

GLUquadricObj* sphere = NULL;
GLUquadricObj* cylinder = NULL;
GLUquadricObj* cone = NULL;



void
init()
{

	reinitialize();

	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluQuadricOrientation(sphere, GLU_OUTSIDE);
	gluQuadricTexture(sphere, GL_FALSE);

	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricNormals(cylinder, GLU_SMOOTH);
	gluQuadricOrientation(cylinder, GLU_OUTSIDE);
	gluQuadricTexture(cylinder, GL_FALSE);

	cone = gluNewQuadric();
	gluQuadricDrawStyle(cone, GLU_FILL);
	gluQuadricNormals(cone, GLU_SMOOTH);
	gluQuadricOrientation(cone, GLU_OUTSIDE);
	gluQuadricTexture(cone, GL_FALSE);



	readMesh("m01_bunny.off");


	cout << endl;
	cout << "keyboard Input: space for play/pause" << endl;
	cout << "keyboard Input: i for reinitialization" << endl;
	cout << "keyboard Input: up for increasing period" << endl;
	cout << "keyboard Input: down for decreasing period" << endl;
	cout << "keyboard Input: v for the top, front, bird-eye view" << endl;
	cout << endl;

	cout << "Keyboard Input:p for a point light" << endl;
	cout << "Keyboard Input:d for a distant light" << endl;
	cout << "Keyboard Input:s for a spot light" << endl;
	cout << "Keyboard Input:a for light attenuation" << endl;
	cout << "keyboard Input:e for time dependent exponent of a spot light" << endl;
	cout << "keyboard Input:c for time dependent cutoff of a spot light" << endl;
	cout << "keyboard Input:m for predefined material parameters" << endl;
	cout << "keyboard Input:l for rotation of lights" << endl;
	

	cout << "keyboard Input:o for rotation of objects " << endl;
	cout << "keyboard Input:1 for 3x3 spheres " << endl;
	cout << "keyboard Input:2 for 3x3 flat bunnies" << endl;
	cout << "keyboard Input:3 for 3x3 smooth bunnies" << endl;
	cout << "keyboard Input:4 for a plane with many small squres" << endl;

}


void
quit()
{
	gluDeleteQuadric(sphere);
	gluDeleteQuadric(cylinder);
	gluDeleteQuadric(cone);


	deleteMesh();
}

void
setupColoredMaterial(const vec3& color)
{
	GLfloat mat_ambient[4] = { 0.1f,0.1f,0.1f,1.0f };
	GLfloat mat_diffuse[4] = { color[0],color[1],color[2],1.0f };
	GLfloat mat_specular[4] = { 0.5f,0.5f,0.5f,1.0f };
	GLfloat mat_shininess = 100;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

}

void
setupWhiteShinyMaterial()
{
	GLfloat mat_ambient[4] = { 0.1f,0.1f,0.1f,1.0f };
	GLfloat mat_diffuse[4] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat mat_specular[4] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat mat_shininess = 100;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void
setupBrassMaterial()
{
	GLfloat mat_ambient[4] = { 0.33f,0.22f,0.33f,1.0f };
	GLfloat mat_diffuse[4] = { 0.78f,0.57f,0.11f,1.0f };
	GLfloat mat_specular[4] = { 0.99f,0.91f,0.81f,1.0f };
	GLfloat mat_shininess = 27.8f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void
setupRedPlasticMaterial()
{
	GLfloat mat_ambient[4] = { 0.3f,0.0f,0.0f,1.0f };
	GLfloat mat_diffuse[4] = { 0.6f,0.0f,0.0f,1.0f };
	GLfloat mat_specular[4] = { 0.8f,0.6f,0.6f,1.0f };
	GLfloat mat_shininess = 32.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void
drawSquare()
{
	glBegin(GL_QUADS);

	glNormal3f(0, 0, 1);
	glVertex3f(0, 0, 1);
	glVertex3f(1, 0, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(0, 1, 1);

	glEnd();

}

void
drawSphere(float radius, int slices, int stacks)
{
	gluSphere(sphere, radius, slices, stacks);

}

void
drawCylinder(float radius, float height, int slices, int stacks)
{
	gluCylinder(cylinder, radius, radius, height, slices, stacks);

}

void
drawCone(float radius, float height, int slices, int stacks)
{
	gluCylinder(cone, 0, radius, height, slices, stacks);
}

int nVertices = 0, nFaces = 0, nEdges = 0;
vec3* vertex = NULL;
vec3* vnormal = NULL;
vec3* fnormal = NULL;
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
		if (n != 3) cout << "#vertices of the" << i << "-th faces= " << n << endl;

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
computeRotation(const vec3& a, const vec3& b, float& theta, vec3& axis)
{
	axis = cross(a, b);
	float sinTheta = length(axis);
	float cosTheta = dot(a,b);
	theta = float(atan2(sinTheta, cosTheta) * 180.0 / M_PI);

}

void 
setupLight(const vec4& p ,int i)
{
	GLfloat ambient[4] = { 0.1f,0.1f,0.1f,1.0f };
	GLfloat diffuse[4] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat specular[4] = { 0.1f,0.1f, 0.1f, 1.0f };
	GLfloat mat_shininess = 25;
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	glLightfv(GL_LIGHT0 + i, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0 + i, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0 + i, GL_POSITION, value_ptr(p));

	if ((i == 0 || i == 2) && attenuation)
	{
		glLightf(GL_LIGHT0 + i, GL_CONSTANT_ATTENUATION, 1.0f);
		glLightf(GL_LIGHT0 + i, GL_LINEAR_ATTENUATION, 0.1f);
		glLightf(GL_LIGHT0 + i, GL_QUADRATIC_ATTENUATION, 0.05f);
	}
	else {
		glLightf(GL_LIGHT0 + i, GL_CONSTANT_ATTENUATION, 1.0f);
		glLightf(GL_LIGHT0 + i, GL_LINEAR_ATTENUATION, 0.0f);
		glLightf(GL_LIGHT0 + i, GL_QUADRATIC_ATTENUATION, 0.0f);
	}
	if (i == 2)
	{
		vec3 spotDirection = -vec3(p);
		glLightfv(GL_LIGHT0 + i, GL_SPOT_DIRECTION, value_ptr(spotDirection));
		glLightf(GL_LIGHT0 + i, GL_SPOT_CUTOFF, cutoffValue);
		glLightf(GL_LIGHT0 + i, GL_SPOT_EXPONENT, exponentValue);
	}
	else {
		glLightf(GL_LIGHT0 + i, GL_SPOT_CUTOFF, 180);
	}
}

void
drawArrow(const vec3& p, bool tailOnly)
{
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	GLfloat mat_specular[4] = { 1,1,1,1 };
	GLfloat mat_shininess = 25;
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,mat_shininess);

	glPushMatrix();

	glTranslatef(p.x, p.y, p.z);

	if (!tailOnly)
	{
		float theta;
		vec3 axis;
		computeRotation(vec3(0, 0, 1), vec3(0, 0, 0) - vec3(p), theta, axis);
		glRotatef(theta, axis.x, axis.y, axis.z);

	}
	float arrowTailRadius = 0.05f;
	glColor3f(1, 0, 0);
	drawSphere(arrowTailRadius, 16, 16);

	if(!tailOnly)
	{ 
		float arrowShaftRadius = 0.02f;
		float arrowShaftLength = 0.2f;
		glColor3f(0, 1, 0);
		drawCylinder(arrowShaftRadius, arrowShaftLength, 16, 5);

		float arrowheadHeight = 0.09f;
		float arrowheadRadius = 0.06f;
		glTranslatef(0, 0, arrowShaftLength + arrowheadHeight);
		glRotatef(180, 1, 0, 0);
		glColor3f(0, 0, 1);
		drawCone(arrowheadRadius, arrowheadHeight, 16, 5);

	}

	glPopMatrix();

	glDisable(GL_COLOR_MATERIAL);
}

void
drawSpotLight(const vec3& p, float cutoff)
{
	glPushMatrix();

	glTranslatef(p.x, p.y, p.z);
	float theta;
	vec3 axis;
	computeRotation(vec3(0, 0, 1), vec3(0, 0, 0) - vec3(p), theta, axis);
	glRotatef(theta, axis.x, axis.y, axis.z);

	setupColoredMaterial(vec3(0, 0, 1));

	float h = 0.15f;
	float r = h * tan(radians(cutoff));
	drawCone(r, h, 16, 5);
	setupColoredMaterial(vec3(1, 0, 0));

	float apexRadius = 0.06f * (0.5f + exponentValue / 128.0f);
	drawSphere(apexRadius,16,16);
	glPopMatrix();
}

void
render(GLFWwindow* window)
{
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


	if (axes) {
		glDisable(GL_LIGHTING);
		drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH*dpiScaling);

	}


	glShadeModel(GL_SMOOTH);

	vec3 axis(0, 1, 0);



	glEnable(GL_LIGHTING);


	vec4 lightP[3];

	for (int i = 0; i < 3; i++)
	{

		if (!lightOn[i]) { glDisable(GL_LIGHT0 + i); continue; }


		glEnable(GL_LIGHT0 + i);



		lightP[i] = lightInitialP;
		if (i == 1) lightP[i].w = 0;

		mat4 R = rotate(mat4(1.0), radians(thetaLight[i]), axis);;

		lightP[i] = R * lightP[i];


		setupLight(lightP[i], i);
	}


	for (int i = 0; i < 3; i++)
	{
		if (!lightOn[i]) continue;

		if (i == 2) drawSpotLight(lightP[i], cutoffValue);
		else		drawArrow(lightP[i], i == 0);

	}



	switch (material)
	{
	case 0: setupWhiteShinyMaterial(); break;
	case 1: setupBrassMaterial(); break;
	case 2: setupRedPlasticMaterial(); break;

	}

	if (selection == 4)
	{
		float width = 0.05f;
		float height = 0.05f;
		float depth = 0.1f;
		for (float i = -2.2f; i <= 2.2f; i += width)
			for (float j = -1.3f; j <= 1.3f; j += height)
			{
				glPushMatrix();

				glTranslatef(i, j, -1.0);
				glScalef(width, height, depth);
				glTranslatef(-0.5, -0.5, -0.5);
				drawSquare();
				glPopMatrix();
			}
	}
	else {
		glScalef(0.4f, 0.4f, 0.4f);

		vec3 u[3];
		vec3 v[3];
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
					glRotatef(thetaModel, axis.x, axis.y, axis.z);

					switch (selection)
					{
					case 1: drawSphere(0.7f, 64, 64); break;
					case 2: drawFlatMesh(); break;
					case 3: drawSmoothMesh(); break;
					}

					glPopMatrix();
			}
				glPopMatrix();
		}
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

		case GLFW_KEY_I: reinitialize(); break;

		case GLFW_KEY_SPACE: pause = !pause; break;

		case GLFW_KEY_UP: period += 0.1f; break;
		case GLFW_KEY_DOWN:period = (float)std::max(period - 0.1, 0.1); break;

		case GLFW_KEY_V: view = (view + 1) % 3; break;

		case GLFW_KEY_P: lightOn[0] = !lightOn[0]; break;

		case GLFW_KEY_D: lightOn[1] = !lightOn[1]; break;

		case GLFW_KEY_S: lightOn[2] = !lightOn[2]; break;



		case GLFW_KEY_A: attenuation = !attenuation; break;

		case GLFW_KEY_L: rotationLight = !rotationLight; break;

		case GLFW_KEY_O: rotationObject = !rotationObject; break;

		case GLFW_KEY_E: exponent = !exponent; break;

		case GLFW_KEY_C: cutoff = !cutoff; break;

		case GLFW_KEY_M: material = (material + 1) % 3; break;

		case GLFW_KEY_1: selection = 1; break;
		case GLFW_KEY_2: selection = 2; break;
		case GLFW_KEY_3: selection = 3; break;
		case GLFW_KEY_4: selection = 4; break;
		}
	}

}
