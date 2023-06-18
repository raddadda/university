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

int main(int argc,char* argv[])
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

	cout << "Keyboard Input:1 for rotation example" << endl;
	cout << "Keyboard Input:2 for rotation wrt a pivot example" << endl;
	cout << "Keyboard Input:3 for ratation wrt a pivor example using GLM" << endl;
	cout << "Keyboard Input:4 for scaling example" << endl;
	cout << "keyboard Input:5 for scaling wrt a pivot example" << endl;
	cout << "keyboard Input:6 for scaling wrt a direction example" << endl;
	cout << "keyboard Input:7 for a solar system example" << endl;
	cout << "keyboard Input:8 for scaling wrt a pivot example using GLM" << endl;
	cout << "keyboard Input:9 for scaling wrt a direction example using GLM" << endl;
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
rotationExample()
{
	float theta1 = frame * 4 / period;
	glm::vec3 axis(0, 0, 1);

	glRotatef(theta1, axis.x, axis.y, axis.z);

	setDiffuseColor(glm::vec3(1, 1, 1));
	drawCube();
}

void
rotationPivotExample()
{
	float theta1 = frame * 4 / period;
	glm::vec3 axis(1, 0, 1);
	glm::vec3 pivot(0, 0, 0);

	//if (selection >= 3)
	glTranslatef(pivot.x, pivot.y, pivot.z);
	glRotatef(theta1, axis.x, axis.y, axis.z);
	glTranslatef(-pivot.x, -pivot.y, -pivot.z);

	setDiffuseColor(glm::vec3(11, 1, 1));
	drawCube();
}

void
rotationPivotExampleGLM(){
	
	glm::vec3 axis(0, 1, 0);
	glm::vec3 pivot(0, 0, 0);
	//Identity matrix
	glm::mat4 M(1.0);
	for (int j = 0; j < 36; ++j) {
		float theta1 = j;
		//M=M*translate(pivot)
		M = glm::translate(M, pivot);
		//M=M*rotate(pivot)
		M = glm::rotate(M, glm::radians(theta1), axis);
		//M=M*translate(-pivot)
		M = glm::translate(M, -pivot);

		double rad = 0.5;

		glMultMatrixf(value_ptr(M));

		setDiffuseColor(glm::vec3(1, 1, 1));
		glBegin(GL_LINE_LOOP);

		for (int i = 0; i < 18; ++i) {

			double angle, x, y;
			angle = i * 20 * 3.141592 / 180;
			x = rad * cos(angle) + 1.0f;
			y = rad * sin(angle) + 1.0f;
			glVertex3f(x, y, 0);


		}

		glEnd();
	}
	//drawCube();
}

void scalingExample()
{
	float s = 1.0f + 0.9f * sin(frame * 0.1f / period);

	glScalef(s, 1, 1);

	setDiffuseColor(glm::vec3(1, 1, 1));
	drawCube();
}

void scalingPivotExample()
{
	float s = 1.0f + 0.95f * sin(frame * 0.1f / period);
	glm::vec3 pivot(0.5, 0.5, 0.5);
	

	glTranslatef(pivot.x, pivot. y, pivot.z);
	glScalef(s, 1, 1);
	glTranslatef(-pivot.x, -pivot.y, -pivot.z);

	setDiffuseColor(glm::vec3(1, 1, 1));
	drawCube();

}
void scalingPivotExampleGLM()
{
	float s = 1.0f + 0.95f * sin(frame * 0.1f / period);
	glm::vec3 pivot(0.5, 0.5, 0.5);
	

	//Identity matrix
	glm::mat4 M(1.0);
	
	M = glm::translate(M, pivot);
	
	M = glm::scale(M, glm::vec3(s,1,1));
	
	M = glm::translate(M, -pivot);



	glMultMatrixf(value_ptr(M));

	setDiffuseColor(glm::vec3(1, 1, 1));
	drawCube();


}


void scalingDirectionExample()
{
	float s = 1.0f + 0.95f * sin(frame * 0.1f / period);
	glm::vec3 direction(1, 1, 0);
	direction = normalize(direction);

	glm::vec3 axis = cross(glm::vec3(1, 0, 0), direction);
	float sinTheta = length(axis);
	float cosTheta = dot(glm::vec3(1, 0, 0), direction);
	float theta = (float)atan2(sinTheta, cosTheta) * float(180.0 / M_PI);
	axis /= sinTheta;


	glRotatef(theta, axis.x, axis.y, axis.z);
	glScalef(s, 1, 1);
	glRotatef(-theta, axis.x, axis.y, axis.z);

	setDiffuseColor(glm::vec3(1, 1, 1));
	drawCube();
}
void scalingDirectionExampleGLM()
{
	float s = 1.0f + 0.95f * sin(frame * 0.1f / period);
	glm::vec3 direction(1, 1, 0);
	direction = normalize(direction);

	glm::vec3 axis = cross(glm::vec3(1, 0, 0), direction);
	float sinTheta = length(axis);
	float cosTheta = dot(glm::vec3(1, 0, 0), direction);
	float theta = (float)atan2(sinTheta, cosTheta) * float(180.0 / M_PI);
	axis /= sinTheta;


	glm::mat4 M(1.0);
	
	M = glm::rotate(M, glm::radians(theta), axis);
	M = glm::scale(M, glm::vec3(s, 1, 1));
	M = glm::rotate(M, glm::radians(-theta), axis);
	

	glMultMatrixf(value_ptr(M));

	setDiffuseColor(glm::vec3(1, 1, 1));
	drawCube();
}

void
solarSystemExample()
{
	//Planet1
	float theta1 = frame * 4.0f / period + 10;//공전속도
	float orbit1 = 1.2f; //공전크기
	float radius1 = 0.2f; //위성크기
	//Planet2
	float theta2 = frame * 3.0f / period + 120;
	float orbit2 = 2.0f;
	float radius2 = 0.2f;
	//Satellite of Planet2
	float theta21 = frame * 9.0f / period + 20;
	float orbit21 = 0.5f;
	float radius21 = 0.1f;
	//Sun
	drawSphere(0.5, vec3(0.95, 0.95, 0.15)); //태양 크기, 색
	//Planet1
	glPushMatrix(); //위성 추가
	glRotatef(theta1, 0, 1, 0); //위성 회전축
	//Planet
	glPushMatrix(); //초록위성
	glTranslatef(0, 0, orbit1); //생성할 축
	drawSphere(radius1, vec3(0.15, 0.95, 0.15));
	glPopMatrix();
	//Support

	drawCylinder(radius1 / 4, orbit1, vec3(0.95, 0.95, 0.95)); //공전축
	glPopMatrix();
	//Orbit of Planet1
	glPushMatrix();
	glRotatef(-90, 1, 0, 0); //공전궤도
	drawDisk(orbit1 - radius1 / 4, orbit1 + radius1 / 4, vec3(0, 1, 0));
	glPopMatrix();


	//planet2
	glPushMatrix();
	glRotatef(theta2, 0, 1, 0);
	//Planet
		glPushMatrix();
		glTranslatef(0, 0, orbit2);
		drawSphere(radius2, vec3(0.15, 0.15, 0.95));
	//Satellite of the planet2
			glPushMatrix();
			glRotatef(theta21, 0, 1, 0);
	//Satellite
				glPushMatrix();
				glTranslatef(0, 0, orbit21);
				drawSphere(radius21, vec3(0.95, 0.15, 0.95));
				glPopMatrix();

	//support
			drawCylinder(radius21 / 4, orbit21, vec3(0.95, 0.95, 0.95));
			glPopMatrix();
	//Orbit of the satellite of the planet 2
			glPushMatrix();
				glRotatef(-90, 1, 0, 0);
				drawDisk(orbit21 - radius21 / 4, orbit21 + radius21 / 4, vec3(1, 0, 1));
			glPopMatrix();
		glPopMatrix();

	//support
		drawCylinder(radius2 / 4, orbit2, vec3(0.95, 0.95, 0.95));
	glPopMatrix();
	//Orbit of Planet2
		glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		drawDisk(orbit2 - radius2 / 4, orbit2 + radius2 / 4, vec3(0, 0, 1));
	glPopMatrix();


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
	drawAxes(AXIS_LENGTH , AXIS_LINE_WIDTH* dpiScaling);
	
	setupLight();

	setupMaterial();

	switch (selection)
		{
	case 1: rotationExample(); break;
	case 2: rotationPivotExample(); break;
	case 3: rotationPivotExampleGLM(); break;
	case 4: scalingExample(); break;
	case 5: scalingPivotExample(); break;
	case 6: scalingDirectionExample(); break;
	case 7: solarSystemExample(); break;
	case 8: scalingPivotExampleGLM(); break;
	case 9: scalingDirectionExampleGLM(); break;

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

