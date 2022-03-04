#include <windows.h>
#include <stdlib.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glut.h>
#include <math.h>
#include "Vector3D.h"
#include "Point3D.h"
#include "camera.h"

#define PI 3.141592653

#define SCREEN_WIDTH	700
#define SCREEN_HEIGHT	700
#define FPS	30

#define MAX_PARTICLES 1000
#define WCX		640
#define WCY		480
#define RAIN	0

float bjump_1 = 0, gjump_1 = 0, bdrop_1 = 0, gdrop_1 = 0;
float angle = -30, angle0 = -30, angle1 = -30, angle2 = -30;
float inc = 1.0, inc0 = 1, inc1 = 1, inc2 = 1;
float bx_position = 0;
float cloudmove = -5;
float gx_position = 4.5;
float xmotherp = 7;
float z_position = 0, zz_position = 0;
float zf_position = 0;
float zff_position = 0;
float turn = 0, turnz = 0, turnm = 0;
float xx_position = 0;
float xf_position = 0;
float xz_position = 0;
float xbj_position = 0;
int action = 1;
float xmothers = 0, zmother = 0;
float turngirl = 0, zgirl = 0, xgirl = 0;
float turnboy = 0, zboy = 0, xboy = 0;
void MyTimerFunc(int value);//TIMER FUNCTION
Camera cam;


////////////////////////////////ALI///////////////////////////////
//Background back;
double rotx;

float anglex = -30;
float incx = 1.0;
float xyx_position = 4;
float _anglex = -30;
float xmotherpx = 1;
float xrx_position = -6;

float slowdown = 2.0;
float velocity = 0.0;
float zoom = 3.0;
float pan = 0.0;
float tilt = 0.0;
float hailsize = 0.1;

int loop;
int fall;

//floor colors
float r = 0.0;
float g = 1.0;
float b = 0.0;
float ground_points[21][21][3];
float ground_colors[21][21][4];
float accum = -10.0;

typedef struct {
	// Life
	bool alive;	// is the particle alive?
	float life;	// particle lifespan
	float fade; // decay
	// color
	float red;
	float green;
	float blue;
	// Position/direction
	float xpos;
	float ypos;
	float zpos;
	// Velocity/Direction, only goes down in y dir
	float vel;
	// Gravity
	float gravity;
}particles;

// Paticle System
particles par_sys[MAX_PARTICLES];


// Initialize/Reset Particles - give them their attributes
void initParticles(int i) {
	par_sys[i].alive = true;
	par_sys[i].life = 1.0;
	par_sys[i].fade = float(rand() % 100) / 1000.0f + 0.003f;

	par_sys[i].xpos = (float)(rand() % 21) - 10;
	par_sys[i].ypos = 10.0;
	par_sys[i].zpos = (float)(rand() % 21) - 10;

	par_sys[i].red = 0.5;
	par_sys[i].green = 0.5;
	par_sys[i].blue = 1.0;

	par_sys[i].vel = velocity;
	par_sys[i].gravity = -10;//-0.8;

}

void init() {
	int x, z;

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);

	// Ground Verticies
	  // Ground Colors
	for (z = 0; z < 50; z++) {
		for (x = 0; x < 50; x++) {
			ground_points[x][z][0] = x - 15.0;
			ground_points[x][z][1] = accum;
			ground_points[x][z][2] = z - 15.0;

			ground_colors[z][x][0] = r; // red value
			ground_colors[z][x][1] = g; // green value
			ground_colors[z][x][2] = b; // blue value
			ground_colors[z][x][3] = 0.0; // acummulation factor
		}
	}

	// Initialize particles
	for (loop = 0; loop < MAX_PARTICLES; loop++) {
		initParticles(loop);
	}
}

// For Rain
void drawRain() {
	float x, y, z;
	for (loop = 0; loop < MAX_PARTICLES; loop = loop + 2) {
		if (par_sys[loop].alive == true) {
			x = par_sys[loop].xpos;
			y = par_sys[loop].ypos;
			z = par_sys[loop].zpos + zoom;

			// Draw particles
			glColor3f(5, 10, 10);
			glBegin(GL_LINES);
			glVertex3f(x, y, z);
			glVertex3f(x, y + 0.5, z);
			glEnd();

			// Update values
			//Move
			// Adjust slowdown for speed!
			par_sys[loop].ypos += par_sys[loop].vel / (slowdown * 700);
			par_sys[loop].vel += par_sys[loop].gravity;
			// Decay
			par_sys[loop].life -= par_sys[loop].fade;

			if (par_sys[loop].ypos <= -10) {
				par_sys[loop].life = -1.0;
			}
			//Revive
			if (par_sys[loop].life < 0.0) {
				initParticles(loop);
			}
		}
	}
}


//create BuildingShape branch
void BuildingWall() {
	glBegin(GL_QUADS);
	glColor3ub(50, 100, 255);
	glVertex3f(3.0, 2.0, 0.0);
	glVertex3f(1.7, 2.0, 0.0);
	glVertex3f(1.7, -2.0, 0.0);
	glVertex3f(3.0, -2.0, 0.0);
	glEnd();
}


//create BuildingShape 
void BuildingShape() {
	//front
	glPushMatrix();
	glTranslated(-2.8, 1.0, -1);
	BuildingWall();
	glPopMatrix();

	//back
	glPushMatrix();
	glTranslated(-2.8, 1.0, -3.5);
	BuildingWall();
	glPopMatrix();

	//right
	glPushMatrix();
	glTranslated(0.0, 1.0, 0.0);
	glRotated(90, 0.0, 1.0, 0.0);
	BuildingWall();
	glPopMatrix();

	//left
	glPushMatrix();
	glTranslated(-0.3, 1.0, 0.8);
	glRotated(-250, 0.0, 1.0, 0.0);
	BuildingWall();
	glPopMatrix();
}

//place BuildingShape 
void BuildingShapeLocation1() {
	glPushMatrix();
	glScaled(1, 1.2, 0.5);
	glTranslated(-1.4, 0, -1);
	glColor3ub(50, 205, 50);
	BuildingShape();

	glPopMatrix();
}

void BuildingShapeLocation2() {
	glPushMatrix();
	glScaled(1, 2, 0.5);
	glTranslated(1, 0, -1);
	glColor3ub(50, 205, 50);
	BuildingShape();

	glPopMatrix();
}

void BuildingShapeLocation3() {
	glPushMatrix();
	glScaled(2, 1, 0.5);
	glTranslated(-1.5, 0, -1);
	glColor3ub(50, 205, 50);
	BuildingShape();

	glPopMatrix();
}

void BuildingShapeLocation4() {
	glPushMatrix();
	glScaled(2, 2, 0.5);
	glTranslated(-1.5, 0, -10);
	glColor3ub(50, 205, 50);
	BuildingShape();

	glPopMatrix();
}

void BuildingShapeLocation5() {
	glPushMatrix();
	glScaled(1, 1.8, 0.5);
	glTranslated(2.5, 0, -1);
	glColor3ub(50, 205, 50);
	BuildingShape();
	glPopMatrix();
}

void BuildingShapeLocation6() {
	glPushMatrix();
	glScaled(1.5, 2, 0.5);
	glTranslated(1, 0, -10);
	glColor3ub(50, 205, 50);
	BuildingShape();

	glPopMatrix();
}

void BuildingShapeLocation7() {
	glPushMatrix();
	glScaled(2, 1.5, 0.5);
	glTranslated(-2.3, 0, -6);
	glColor3ub(50, 205, 50);
	BuildingShape();

	glPopMatrix();
}

//background
void squareBack() {
	glBegin(GL_QUADS);
	glColor3ub(60, 79, 79);
	glVertex3f(30, 5, 0.0);
	glVertex3f(-15, 1.0, 0.0);
	glVertex3f(-10, -6, 0.0);
	glVertex3f(10.0, -4, 0.0);
	glEnd();

}

//create land
void Cland() {
	glPushMatrix();
	glTranslated(1, 0, 2);
	glRotated(90, 1, 0, 0);
	squareBack();
	glPopMatrix();

	/*glPushMatrix();
	glTranslated(1, 1, -4.1);
	glRotated(-180, 1, 0, 0);
	squareBack();
	glPopMatrix();*/
}

////////////////////////////////CREATE PLANE AND CLOUD SCENE 4//////////////////
void square() {
	glBegin(GL_QUADS);
	glVertex3f(1.0, 1.0, 0.0);
	glVertex3f(-1.0, 1.0, 0.0);
	glVertex3f(-1.0, -1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glEnd();
}

void greengrass() {
	glBegin(GL_QUADS);
	glVertex3f(6.3, 5.0, 0.0);//right downward
	glVertex3f(-6.3, 5.0, 0.0);//left downward
	glVertex3f(-6.3, -5.0, 0.0);//left upper
	glVertex3f(6.3, -5.0, 0.0);//right upper
	glEnd();
}

void skycolor() {
	glBegin(GL_QUADS);
	glVertex3f(7.0, 11.0, 0.0);//right downward
	glVertex3f(-7.0, 11.0, 0.0);//left downward
	glVertex3f(-7.0, -11.0, 0.0);//left upper
	glVertex3f(7.0, -11.0, 0.0);//right upper
	glEnd();
}

void rectangle() {
	glBegin(GL_QUADS);
	glVertex3f(2.0, 0.4, 0.0);
	glVertex3f(-2.0, 0.4, 0.0);
	glVertex3f(-2.0, -0.4, 0.0);
	glVertex3f(2.0, -0.4, 0.0);
	glEnd();
}

void recwing() {
	glBegin(GL_QUADS);
	glVertex3f(1.0, 0.1, 0.0);
	glVertex3f(-1.0, 0.1, 0.0);
	glVertex3f(-1.0, -0.1, 0.0);
	glVertex3f(1.0, -0.1, 0.0);
	glEnd();
}

void updownwing() {
	glBegin(GL_QUADS);
	glVertex3f(1.0, 0.2, 0.0);
	glVertex3f(-1.0, 0.2, 0.0);
	glVertex3f(-1.0, -0.2, 0.0);
	glVertex3f(1.0, -0.2, 0.0);
	glEnd();
}

void wing_test() {
	glPushMatrix();//front left side
	glTranslated(0, 0, 0.2);
	recwing();
	glPopMatrix();

	glPushMatrix();//back right side
	glRotated(180.0, 0.0, 1.0, 0.0);
	glTranslated(0, 0, 0.2);
	recwing();
	glPopMatrix();

	glPushMatrix(); //low part
	glRotated(-90.0, 1.0, 0.0, 0.0);
	glTranslated(0, 0, 0.1);
	updownwing();
	glPopMatrix();

	glPushMatrix();//upper part
	glRotated(90.0, 1.0, 0.0, 0.0);
	glTranslated(0, 0, 0.1);
	updownwing();
	glPopMatrix();
}

void smallsquare() {
	glBegin(GL_QUADS);
	glVertex3f(0.4, 0.4, 0.0);
	glVertex3f(-0.4, 0.4, 0.0);
	glVertex3f(-0.4, -0.4, 0.0);
	glVertex3f(0.4, -0.4, 0.0);
	glEnd();
}

void windows1() {
	glBegin(GL_QUADS);
	glVertex3f(0.15, 0.15, 0.0);
	glVertex3f(-0.15, 0.15, 0.0);
	glVertex3f(-0.15, -0.15, 0.0);
	glVertex3f(0.15, -0.15, 0.0);
	glEnd();
}

void frontwindows1() {
	glBegin(GL_QUADS);
	glVertex3f(0.3, 0.15, 0.0);
	glVertex3f(-0.3, 0.15, 0.0);
	glVertex3f(-0.3, -0.15, 0.0);
	glVertex3f(0.3, -0.15, 0.0);
	glEnd();
}

void cloud() {
	glPushMatrix();
	glColor3d(1.0, 1.0, 1.0);
	glRotated(20.0, 0.0, 1.0, 0.0);
	glTranslated(1.0, 0.0, 0.0);
	glutSolidSphere(0.4, 50, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3d(1.0, 1.0, 1.0);
	glRotated(20.0, 0.0, 1.0, 0.0);
	glTranslated(1.2, 0.0, 0.0);
	glutSolidSphere(0.4, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3d(1.0, 1.0, 1.0);
	glRotated(20.0, 0.0, 1.0, 0.0);
	glTranslated(1.3, 0.2, 0.0);
	glutSolidSphere(0.4, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glRotated(20.0, 0.0, 1.0, 0.0);
	glColor3d(1.0, 1.0, 1.0);
	glTranslated(1.0, 0.3, 0.0);
	glutSolidSphere(0.4, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glRotated(20.0, 0.0, 1.0, 0.0);
	glColor3d(1.0, 1.0, 1.0);
	glTranslated(1.0, 0.3, 0.2);
	glutSolidSphere(0.4, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glRotated(20.0, 0.0, 1.0, 0.0);
	glColor3d(1.0, 1.0, 1.0);
	glTranslated(1.0, 0.3, -0.2);
	glutSolidSphere(0.4, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glRotated(20.0, 0.0, 1.0, 0.0);
	glColor3d(1.0, 1.0, 1.0);
	glTranslated(0.8, 0.1, -0.2);
	glutSolidSphere(0.4, 100, 100);
	glPopMatrix();
}

void rectangletailfront() {
	glBegin(GL_QUADS);
	glVertex3f(3.0, 0.4, 0.0);
	glVertex3f(-2.0, 0.4, 0.0);
	glVertex3f(-2.0, -0.4, 0.0);
	glVertex3f(2.0, -0.4, 0.0);
	glEnd();
}

void rectangletailback() {
	glBegin(GL_QUADS);
	glVertex3f(2.0, 0.4, 0.0);
	glVertex3f(-3.0, 0.4, 0.0);
	glVertex3f(-2.0, -0.4, 0.0);
	glVertex3f(2.0, -0.4, 0.0);
	glEnd();
}

void ground() {
	glPushMatrix();
	glColor3ub(0, 154, 23);
	glScaled(2, 2, 2);
	glRotated(90.0, 1.0, 0.0, 0.0);
	glTranslated(0.0, 0.0, 1.0);
	greengrass();
	glPopMatrix();

	glPushMatrix();//back right side
	glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
	glScaled(2, 2, 2);
	glRotated(180.0, 0.0, 1.0, 0.0);
	glTranslated(0.1, 4, 5);
	greengrass();
	glPopMatrix();

	glPushMatrix();//back left side
	glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
	glScaled(2, 2, 2);
	glRotated(270.0, 0.0, 1.0, 0.0);
	glTranslated(-1, 4, 6);
	greengrass();
	glPopMatrix();
}

void planebody() {
	glPushMatrix();//front left side
	glColor4f(1.0f, 1.0f, 0.0f, 0.0f);
	glTranslated(0, 0, 0.4); //(x(left right),y(up down),z(location))
	rectangletailfront();
	glPopMatrix();

	glPushMatrix();//front right side
	glColor3d(1.0, 0.0, 0.0);
	glRotated(90.0, 0.0, 1.0, 0.0);
	glTranslated(0, 0, 2.0);
	smallsquare();
	glPopMatrix();

	glPushMatrix();//back right side
	glColor4f(1.0f, 1.0f, 0.0f, 0.0f);
	glRotated(180.0, 0.0, 1.0, 0.0);
	glTranslated(0, 0, 0.4);
	rectangletailback();
	glPopMatrix();

	glPushMatrix();//back left side
	glColor3d(1.0, 0.0, 1.0);
	glRotated(270.0, 0.0, 1.0, 0.0);
	glTranslated(0, 0, 2.0);
	smallsquare();
	glPopMatrix();

	glPushMatrix(); //low part
	glColor3d(0.0, 1.0, 1.0);
	glRotated(-90.0, 1.0, 0.0, 0.0);
	glTranslated(0, 0, 0.4);
	rectangle();
	glPopMatrix();

	glPushMatrix();//upper part
	glColor3d(0.1f, 0.1f, 0.1f);
	glRotated(90.0, 1.0, 0.0, 0.0);
	glTranslated(0, 0, 0.4);
	rectangle();
	glPopMatrix();

	glPushMatrix();//wing
	glColor3d(1.0, 0.0, 0.0);
	glRotated(270.0, .0, 1.0, .0);
	glTranslated(1.3, -0.2, 1.0);
	wing_test();
	glPopMatrix();

	glPushMatrix();//wing
	glColor3d(1.0, 0.0, 0.0);
	glRotated(90.0, 0.0, 1.0, 0.0);
	glTranslated(1.3, -0.2, -1.0);
	wing_test();
	glPopMatrix();

	//windows//baru
	glPushMatrix();//front left side
	glColor3d(1.0, 1.0, 1.7);
	glTranslated(1.8, 0.1, 0.45); //(x(left right),y(up down),z(location))
	windows1();
	glPopMatrix();

	glPushMatrix();//front left side
	glColor3d(1.0, 1.0, 1.7);
	glTranslated(1.2, 0.1, 0.45); //(x(left right),y(up down),z(location))
	windows1();
	glPopMatrix();

	glPushMatrix();//front left side
	glColor3d(1.0, 1.0, 1.7);
	glTranslated(0, 0.1, 0.45); //(x(left right),y(up down),z(location))
	windows1();
	glPopMatrix();

	glPushMatrix();//front left side
	glColor3d(1.0, 1.0, 1.7);
	glTranslated(0.6, 0.1, 0.45); //(x(left right),y(up down),z(location))
	windows1();
	glPopMatrix();

	glPushMatrix();//front left side
	glColor3d(1.0, 1.0, 1.7);
	glTranslated(-0.6, 0.1, 0.45); //(x(left right),y(up down),z(location))
	windows1();
	glPopMatrix();

	glPushMatrix();//front left side
	glColor3d(1.0, 1.0, 1.7);
	glTranslated(-1.2, 0.1, 0.45); //(x(left right),y(up down),z(location))
	windows1();
	glPopMatrix();

	glPushMatrix();//front left side
	glColor3d(1.0, 1.0, 1.7);
	glTranslated(-1.7, 0.1, 0.45); //(x(left right),y(up down),z(location))
	windows1();
	glPopMatrix();

	glPushMatrix();//head windows
	glColor3d(1.0, 1.0, 1.);
	glRotated(270.0, 0.0, 1.0, 0.0);
	glTranslated(0, 0, 2.02);
	frontwindows1();
	glPopMatrix();
}

void planemove() {
	glPushMatrix();
	glScaled(0.5, 0.5, 0.5);
	glTranslated(2.5, 0.5, 2);
	planebody();
	glPopMatrix();
}

float planex = 0, planey = 0;

void planemove1() {
	glPushMatrix();
	glTranslated(planex, planey, 0);
	planemove();
	glPopMatrix();
}

///////////////////////////////////////////CREATE HUMAN//////////////////////////
float handmove = -30;
float incre = 1.0;

//hand
void squareLeaf() {
	glBegin(GL_QUADS);
	glVertex3f(1.0, 1.0, 0.0);
	glVertex3f(-0.5, 1.0, 0.0);
	glVertex3f(-0.5, -0.5, 0.0);
	glVertex3f(1.0, -0.5, 0.0);
	glEnd();
}

//create leaf object
void leaf() {

	//glColor3ub(50, 205, 50);
	//front
	glPushMatrix();
	glTranslated(-1.0, 1.0, -0.7);
	squareLeaf();
	glPopMatrix();

	//top
	glPushMatrix();
	glTranslated(-1, 2, -1.2);
	glRotated(-90, 1.0, 0.0, 0.0);
	squareLeaf();
	glPopMatrix();

	//bottom
	glPushMatrix();
	glTranslated(-1, 0.5, -1.2);
	glRotated(-90, 1.0, 0.0, 0.0);
	squareLeaf();
	glPopMatrix();

	//back
	glPushMatrix();
	glTranslated(-1.0, 1.0, -2.2);
	squareLeaf();
	glPopMatrix();

	//right
	glPushMatrix();
	glTranslated(0.0, 1.0, -1.2);
	glRotated(90, 0.0, 1.0, 0.0);
	squareLeaf();
	glPopMatrix();

	//left
	glPushMatrix();
	glTranslated(-1.5, 1.0, -1.2);
	glRotated(-270, 0.0, 1.0, 0.0);
	squareLeaf();
	glPopMatrix();
}

//Umbrella
void DrawUmbrellaStick() {
	GLUquadricObj* pObj;
	pObj = gluNewQuadric();
	gluQuadricNormals(pObj, GLU_SMOOTH);

	glPushMatrix();
	glColor3ub(244, 164, 96);
	glRotatef(270, 1, 0, 0);
	gluCylinder(pObj, 0.02, 0.01, 0.5, 27, 25);
	glPopMatrix();
}

void DrawUmbrellaTop() {
	GLUquadricObj* pObj;
	pObj = gluNewQuadric();
	gluQuadricNormals(pObj, GLU_SMOOTH);

	glPushMatrix();
	glColor3ub(244, 164, 96);
	glRotatef(270, 1, 0, 0);
	gluCylinder(pObj, 0.5, 0.02, 0.5, 27, 25);
	glPopMatrix();
}

void wavehand2() {
	//left hand
	glPushMatrix();
	glColor3ub(36, 255, 255);
	glScaled(0.10, 0.4, 0.10);
	glTranslated(-5, 1.0, 30);
	glRotated(50, 1, 0, 0);
	leaf();
	glPopMatrix();

	//right hand
	glPushMatrix();
	glColor3ub(36, 255, 255);
	glScaled(0.10, 0.4, 0.10);
	glTranslated(6.5, 3.5, 24);
	glRotated(180, 0, 0, 1);
	glRotated(0, 0, 0, 1);
	leaf();
	glPopMatrix();

	//left leg
	glPushMatrix();
	glColor3ub(255, 148, 201);
	glScaled(0.10, 0.5, 0.10);
	glTranslated(4.5, 2, 28.5);
	glRotated(180, 0, 0, 1);
	leaf();
	glPopMatrix();

	//right leg
	glPushMatrix();
	glColor3ub(255, 148, 201);
	glScaled(0.10, 0.5, 0.10);
	glTranslated(4.5, 2, 25.5);
	glRotated(180, 0, 0, 1);
	leaf();
	glPopMatrix();

}

void humanPart2() {
	//head
	glPushMatrix();
	glColor3ub(87, 87, 87);
	glScaled(0.3, 0.3, 0.3);
	glTranslated(2.6, 3.3, 10);
	leaf();
	glPopMatrix();

	//body
	glPushMatrix();
	glColor3ub(255, 26, 140);
	glScaled(0.2, 0.4, 0.3);
	glTranslated(3.5, 1, 10);
	leaf();
	glPopMatrix();

	wavehand2();
}

void boy2() {
	glPushMatrix();
	glScaled(0.5, 0.5, 0.5);
	glTranslated(2.5, 0.5, 1.2);
	humanPart2();
	glPopMatrix();
}

void ManUmbrella() {
	glPushMatrix();
	glRotated(-270, 0.0, 1.0, 0.0);
	glTranslated(-3.5, 0, -1.5);
	boy2();
	glPopMatrix();
}

//head
void head() {
	glPushMatrix();
	glScaled(0.3, 0.3, 0.3);
	glTranslated(2.6, 3.3, 10);
	leaf();
	glPopMatrix();


}

void headwomen() {
	//hair
	glPushMatrix();
	glColor3ub(0, 0, 0);
	glScaled(0.3, 0.05, 0.35);
	glTranslated(2.6, 31.5, 8.7);
	leaf();
	glPopMatrix();

	glPushMatrix();
	glColor3ub(0, 0, 0);
	glScaled(0.3, 0.2, 0.05);
	glTranslated(2.4, 6.2, 58.5);
	leaf();
	glPopMatrix();

	glPushMatrix();
	glColor3ub(0, 0, 0);
	glScaled(0.3, 0.2, 0.05);
	glTranslated(2.4, 6.2, 47.7);
	leaf();
	glPopMatrix();

	glPushMatrix();
	glColor3ub(0, 0, 0);
	glScaled(0.05, 0.4, 0.3);
	glTranslated(6.8, 2, 9.8);
	leaf();
	glPopMatrix();
}

//body
void body() {
	glPushMatrix();
	glScaled(0.2, 0.4, 0.3);
	glTranslated(3.5, 1, 10);
	leaf();
	glPopMatrix();
}

void boyhand(float angle) {
	//left hand
	glPushMatrix();
	glColor3ub(0, 191, 255);
	glScaled(0.10, 0.3, 0.10);
	glTranslated(6, 4.5, 27.2);
	glRotated(180, 0, 0, 1);
	glRotated(180, 0, 1, 0);
	glRotated(-angle, 0, 0, 1);
	leaf();
	glPopMatrix();

	glPushMatrix();
	glColor3ub(255, 228, 196);
	glScaled(0.10, 0.1, 0.10);
	glTranslated(6, 8.5, 27.2);
	glRotated(180, 0, 0, 1);
	glRotated(180, 0, 1, 0);
	glRotated(-angle, 0, 0, 1);
	leaf();
	glPopMatrix();

	//right hand
	glPushMatrix();
	glColor3ub(0, 191, 255);
	glScaled(0.10, 0.3, 0.10);
	glTranslated(5.5, 4.5, 24);
	glRotated(180, 0, 0, 1);
	glRotated(0, 0, 1, 0);
	glRotated(-angle, 0, 0, 1);
	leaf();
	glPopMatrix();

	//right hand
	glPushMatrix();
	glColor3ub(255, 228, 196);
	glScaled(0.10, 0.1, 0.10);
	glTranslated(5.5, 8, 24);
	glRotated(180, 0, 0, 1);
	glRotated(0, 0, 1, 0);
	glRotated(-angle, 0, 0, 1);
	leaf();
	glPopMatrix();
}

void boyhandst() {
	//left hand
	glPushMatrix();
	glColor3ub(0, 191, 255);
	glScaled(0.10, 0.3, 0.10);
	glTranslated(6, 4.5, 27.2);
	glRotated(180, 0, 0, 1);
	glRotated(180, 0, 1, 0);
	glRotated(0, 0, 0, 1);
	leaf();
	glPopMatrix();

	glPushMatrix();
	glColor3ub(255, 228, 196);
	glScaled(0.10, 0.1, 0.10);
	glTranslated(6, 8.5, 27.2);
	glRotated(180, 0, 0, 1);
	glRotated(180, 0, 1, 0);
	glRotated(0, 0, 0, 1);
	leaf();
	glPopMatrix();

	//right hand
	glPushMatrix();
	glColor3ub(0, 191, 255);
	glScaled(0.10, 0.3, 0.10);
	glTranslated(5.5, 4.5, 24);
	glRotated(180, 0, 0, 1);
	glRotated(0, 0, 1, 0);
	glRotated(0, 0, 0, 1);
	leaf();
	glPopMatrix();

	//right hand
	glPushMatrix();
	glColor3ub(255, 228, 196);
	glScaled(0.10, 0.1, 0.10);
	glTranslated(5.5, 8, 24);
	glRotated(180, 0, 0, 1);
	glRotated(0, 0, 1, 0);
	glRotated(0, 0, 0, 1);
	leaf();
	glPopMatrix();
}


void girlhand(float angle0) {
	//left hand
	glPushMatrix();
	glColor3ub(0, 191, 255);
	glScaled(0.10, 0.3, 0.10);
	glTranslated(6, 4.5, 27.2);
	glRotated(180, 0, 0, 1);
	glRotated(180, 0, 1, 0);
	glRotated(-angle0, 0, 0, 1);
	leaf();
	glPopMatrix();

	glPushMatrix();
	glColor3ub(255, 228, 196);
	glScaled(0.10, 0.1, 0.10);
	glTranslated(6, 8, 27.2);
	glRotated(180, 0, 0, 1);
	glRotated(180, 0, 1, 0);
	glRotated(-angle0, 0, 0, 1);
	leaf();
	glPopMatrix();

	//right hand
	glPushMatrix();
	glColor3ub(0, 191, 255);
	glScaled(0.10, 0.3, 0.10);
	glTranslated(5.5, 4.5, 24);
	glRotated(180, 0, 0, 1);
	glRotated(0, 0, 1, 0);
	glRotated(-angle0, 0, 0, 1);
	leaf();
	glPopMatrix();

	//right hand
	glPushMatrix();
	glColor3ub(255, 228, 196);
	glScaled(0.10, 0.1, 0.10);
	glTranslated(5.5, 8, 24);
	glRotated(180, 0, 0, 1);
	glRotated(0, 0, 1, 0);
	glRotated(-angle0, 0, 0, 1);
	leaf();
	glPopMatrix();
}

void girlhand1(float angle2) {
	//left hand
	glPushMatrix();
	glColor3ub(0, 191, 255);
	glScaled(0.10, 0.3, 0.10);
	glTranslated(6, 4.5, 27.2);
	glRotated(180, 0, 0, 1);
	glRotated(180, 0, 1, 0);
	glRotated(-angle0, 0, 0, 1);
	leaf();
	glPopMatrix();

	glPushMatrix();
	glColor3ub(255, 228, 196);
	glScaled(0.10, 0.1, 0.10);
	glTranslated(6, 8, 27.2);
	glRotated(180, 0, 0, 1);
	glRotated(180, 0, 1, 0);
	glRotated(-angle0, 0, 0, 1);
	leaf();
	glPopMatrix();

	//right hand
	glPushMatrix();
	glColor3ub(0, 191, 255);
	glScaled(0.10, 0.3, 0.10);
	glTranslated(5.5, 4.5, 24);
	glRotated(180, 0, 0, 1);
	glRotated(0, 0, 1, 0);
	glRotated(-angle2, 0, 0, 1);
	leaf();
	glPopMatrix();

	//right hand
	glPushMatrix();
	glColor3ub(255, 228, 196);
	glScaled(0.10, 0.1, 0.10);
	glTranslated(5.5, 8, 24);
	glRotated(180, 0, 0, 1);
	glRotated(0, 0, 1, 0);
	glRotated(-angle2, 0, 0, 1);
	leaf();
	glPopMatrix();
}

void motherhand(float angle1) {
	//left hand
	glPushMatrix();
	glColor3ub(0, 191, 255);
	glScaled(0.10, 0.3, 0.10);
	glTranslated(6, 4.5, 27.2);
	glRotated(180, 0, 0, 1);
	glRotated(180, 0, 1, 0);
	glRotated(-angle1, 0, 0, 1);
	leaf();
	glPopMatrix();

	glPushMatrix();
	glColor3ub(255, 228, 196);
	glScaled(0.10, 0.1, 0.10);
	glTranslated(6, 8, 27.2);
	glRotated(180, 0, 0, 1);
	glRotated(180, 0, 1, 0);
	glRotated(-angle1, 0, 0, 1);
	leaf();
	glPopMatrix();

	//right hand
	glPushMatrix();
	glColor3ub(0, 191, 255);
	glScaled(0.10, 0.3, 0.10);
	glTranslated(5.5, 4.5, 24);
	glRotated(180, 0, 0, 1);
	glRotated(0, 0, 1, 0);
	glRotated(-angle1, 0, 0, 1);
	leaf();
	glPopMatrix();

	//right hand
	glPushMatrix();
	glColor3ub(255, 228, 196);
	glScaled(0.10, 0.1, 0.10);
	glTranslated(5.5, 8, 24);
	glRotated(180, 0, 0, 1);
	glRotated(0, 0, 1, 0);
	glRotated(-angle1, 0, 0, 1);
	leaf();
	glPopMatrix();
}

void boyleg(float angle) {
	//left leg
	glPushMatrix();
	glScaled(0.10, 0.5, 0.10);
	glTranslated(6.5, 2, 25.5);
	glRotated(180, 0, 0, 1);
	glRotated(180, 0, 1, 0);
	glRotated(-angle, 0, 0, 1);
	leaf();
	glPopMatrix();

	//right leg
	glPushMatrix();
	glScaled(0.10, 0.5, 0.10);
	glTranslated(4.5, 2, 25.5);
	glRotated(180, 0, 0, 1);
	glRotated(-angle, 0, 0, 1);
	leaf();
	glPopMatrix();
}

void boylegst() {
	//left leg
	glPushMatrix();
	glScaled(0.10, 0.5, 0.10);
	glTranslated(6.5, 2, 25.5);
	glRotated(180, 0, 0, 1);
	glRotated(180, 0, 1, 0);
	glRotated(0, 0, 0, 1);
	leaf();
	glPopMatrix();

	//right leg
	glPushMatrix();
	glScaled(0.10, 0.5, 0.10);
	glTranslated(4.5, 2, 25.5);
	glRotated(180, 0, 0, 1);
	glRotated(0, 0, 0, 1);
	leaf();
	glPopMatrix();
}

void girlleg(float angle0) {
	//left leg
	glPushMatrix();
	glScaled(0.10, 0.5, 0.10);
	glTranslated(6.5, 2, 25.5);
	glRotated(180, 0, 0, 1);
	glRotated(180, 0, 1, 0);
	glRotated(-angle0, 0, 0, 1);
	leaf();
	glPopMatrix();

	//right leg
	glPushMatrix();
	glScaled(0.10, 0.5, 0.10);
	glTranslated(4.5, 2, 25.5);
	glRotated(180, 0, 0, 1);
	glRotated(-angle0, 0, 0, 1);
	leaf();
	glPopMatrix();
}

void girlleg1(float angle2) {
	//left leg
	glPushMatrix();
	glScaled(0.10, 0.5, 0.10);
	glTranslated(6.5, 2, 25.5);
	glRotated(180, 0, 0, 1);
	glRotated(180, 0, 1, 0);
	glRotated(-angle2, 0, 0, 1);
	leaf();
	glPopMatrix();

	//right leg
	glPushMatrix();
	glScaled(0.10, 0.5, 0.10);
	glTranslated(4.5, 2, 25.5);
	glRotated(180, 0, 0, 1);
	glRotated(-angle2, 0, 0, 1);
	leaf();
	glPopMatrix();
}

void motherleg(float angle1) {
	//left leg
	glPushMatrix();
	glScaled(0.10, 0.5, 0.10);
	glTranslated(6.5, 2, 25.5);
	glRotated(180, 0, 0, 1);
	glRotated(180, 0, 1, 0);
	glRotated(-angle1, 0, 0, 1);
	leaf();
	glPopMatrix();

	//right leg
	glPushMatrix();
	glScaled(0.10, 0.5, 0.10);
	glTranslated(4.5, 2, 25.5);
	glRotated(180, 0, 0, 1);
	glRotated(-angle1, 0, 0, 1);
	leaf();
	glPopMatrix();
}

void handR() {
	/////baru
	handmove += incre;

	if (handmove > 30) {
		handmove = 30;
		incre = -1;
	}
	if (handmove < -10) {
		handmove = -10;
		incre = +1;
	}
	////////baru

	//left hand
	glPushMatrix();
	glColor3ub(36, 255, 255);
	glScaled(0.10, 0.4, 0.10);
	glTranslated(6, 3.5, 27.2);
	glRotated(180, 0, 0, 1);
	glRotated(180, 0, 1, 0);
	glRotated(0, 0, 0, 1);//hands in the air
	leaf();
	glPopMatrix();

	//right hand
	glPushMatrix();
	glColor3ub(36, 255, 255);
	glScaled(0.10, 0.4, 0.10);
	glTranslated(6.5, 3.5, 24);
	glRotated(180, 0, 0, 1);
	glRotated(0, 0, 0, 1);
	leaf();
	glPopMatrix();
}

void handRst() {//baru
	//left hand
	glPushMatrix();
	glColor3ub(36, 255, 255);
	glScaled(0.10, 0.4, 0.10);
	glTranslated(6, 3.5, 27.2);
	glRotated(180, 0, 0, 1);
	glRotated(180, 0, 1, 0);
	glRotated(0, 0, 0, 1);//hands in the air
	leaf();
	glPopMatrix();

	//right hand
	glPushMatrix();
	glColor3ub(36, 255, 255);
	glScaled(0.10, 0.4, 0.10);
	glTranslated(6.5, 3.5, 24);
	glRotated(180, 0, 0, 1);
	glRotated(0, 0, 0, 1);
	leaf();
	glPopMatrix();

	//left leg
	glPushMatrix();
	glColor3ub(255, 148, 201);
	glScaled(0.10, 0.5, 0.10);
	glTranslated(4.5, 2, 28.5);
	glRotated(180, 0, 0, 1);
	leaf();
	glPopMatrix();

	//right leg
	glPushMatrix();
	glColor3ub(255, 148, 201);
	glScaled(0.10, 0.5, 0.10);
	glTranslated(4.5, 2, 25.5);
	glRotated(180, 0, 0, 1);
	leaf();
	glPopMatrix();
}

void wavehand() {
	//left hand
	glPushMatrix();
	glColor3ub(36, 255, 255);
	glScaled(0.10, 0.4, 0.10);
	glTranslated(6, 2.4, 30);
	glRotated(0, 0, 0, 1);//hands in the air
	leaf();
	glPopMatrix();

	//right hand
	glPushMatrix();
	glColor3ub(36, 255, 255);
	glScaled(0.10, 0.4, 0.10);
	glTranslated(6.5, 3.5, 24);
	glRotated(180, 0, 0, 1);
	glRotated(0, 0, 0, 1);
	leaf();
	glPopMatrix();

	//left leg
	glPushMatrix();
	glColor3ub(255, 148, 201);
	glScaled(0.10, 0.5, 0.10);
	glTranslated(4.5, 2, 28.5);
	glRotated(180, 0, 0, 1);
	leaf();
	glPopMatrix();

	//right leg
	glPushMatrix();
	glColor3ub(255, 148, 201);
	glScaled(0.10, 0.5, 0.10);
	glTranslated(4.5, 2, 25.5);
	glRotated(180, 0, 0, 1);
	leaf();
	glPopMatrix();

}

void eye() {
	//eye left
	glPushMatrix();
	glColor3ub(255, 255, 255);
	glScaled(0.05, 0.15, 0.08);
	glTranslated(17.5, 8.5, 32.5);
	leaf();
	glPopMatrix();

	//eye left
	glPushMatrix();
	glColor3ub(0, 0, 0);
	glScaled(0.05, 0.05, 0.05);
	glTranslated(19.5, 27.5, 52);
	leaf();
	glPopMatrix();

	//eye right
	glPushMatrix();
	glColor3ub(255, 255, 255);
	glScaled(0.05, 0.15, 0.08);
	glTranslated(17.5, 8.5, 35.5);
	leaf();
	glPopMatrix();

	//eye right
	glPushMatrix();
	glColor3ub(0, 0, 0);
	glScaled(0.05, 0.05, 0.05);
	glTranslated(19.5, 27.5, 56);
	leaf();
	glPopMatrix();
}

void humanPart() {
	head();
	body();
	wavehand();
}

void boypart() {
	glPushMatrix();
	glColor3ub(205, 133, 63);
	head();

	//hair
	glPushMatrix();
	glColor3ub(0, 0, 0);
	glScaled(0.3, 0.05, 0.35);
	glTranslated(2.6, 31.5, 8.7);
	leaf();
	glPopMatrix();

	eye();

	glColor3ub(135, 206, 250);
	body();
	glColor3ub(244, 164, 96);
	boyhand(angle);
	glColor3ub(0, 0, 205);
	boyleg(angle);
	glPopMatrix();
}

void boypartst() {
	glPushMatrix();
	glColor3ub(205, 133, 63);
	head();

	//hair
	glPushMatrix();
	glColor3ub(0, 0, 0);
	glScaled(0.3, 0.05, 0.35);
	glTranslated(2.6, 31.5, 8.7);
	leaf();
	glPopMatrix();

	eye();

	glColor3ub(135, 206, 250);
	body();
	glColor3ub(244, 164, 96);
	boyhandst();
	glColor3ub(0, 0, 205);
	boylegst();
	glPopMatrix();
}

void girlpart() {
	glPushMatrix();
	glColor3ub(255, 228, 196);
	head();

	eye();
	headwomen();
	glColor3ub(255, 105, 180);
	body();
	glColor3ub(244, 164, 96);
	girlhand(angle0);
	glColor3ub(255, 20, 147);
	girlleg(angle0);
	girlleg1(angle2);
	glPopMatrix();
}

void motherpart() {
	glPushMatrix();
	glColor3ub(245, 255, 250);
	head();
	eye();
	headwomen();
	glColor3ub(186, 85, 211);
	body();
	glColor3ub(244, 164, 96);
	motherhand(angle1);
	glColor3ub(255, 0, 0);
	motherleg(angle1);
	glPopMatrix();
}

void motherpartst() {//baru
	//head
	glPushMatrix();
	glColor3ub(87, 87, 87);
	glScaled(0.3, 0.3, 0.3);
	glTranslated(2.6, 3.3, 10);
	leaf();
	glPopMatrix();

	//body
	glPushMatrix();
	glColor3ub(255, 26, 140);
	glScaled(0.2, 0.4, 0.3);
	glTranslated(3.5, 1, 10);
	leaf();
	glPopMatrix();

	handRst();
}

//boy
void boy() {
	glPushMatrix();
	glScaled(0.5, 0.5, 0.5);
	glTranslated(2.5, 0, 1.2);
	boypart();
	glPopMatrix();
}

//girl
void girl() {
	glPushMatrix();
	glScaled(0.5, 0.5, 0.5);
	glTranslated(-4.5, 0, 2);
	girlpart();
	glPopMatrix();
}

//mother
void mother() {
	glPushMatrix();
	glScaled(0.5, 0.5, 0.5);
	glTranslated(4.0, 0.5, -3.6);
	glRotated(180, 0, 1, 0);
	motherpart();
	glPopMatrix();
}

void motherst() {
	glPushMatrix();
	glScaled(0.5, 0.5, 0.5);
	glTranslated(4.0, 0.5, -3.6);
	glRotated(180, 0, 1, 0);
	motherpartst();
	glPopMatrix();
}

void mother1() {
	glPushMatrix();
	glScaled(0.5, 0.5, 0.5);
	glTranslated(-4.5, 0, 2);
	motherpart();
	glPopMatrix();
}

//boy static
void boystatic() {
	glPushMatrix();
	glScaled(0.5, 0.5, 0.5);
	glTranslated(2.5, 0.5, 1.2);
	boypartst();
	glPopMatrix();
}

//girl static
void girlstatic() {
	glPushMatrix();
	glScaled(0.5, 0.5, 0.5);
	glTranslated(-4.5, 0.5, 2);
	girlpart();
	glPopMatrix();
}

float bjump = 0, gjump = 0, bdrop = 0, gdrop = 0;

float walkk = 0, camangle = 0, gthink = 0, glow = 0;

//boy walk
void humanMan() {
	glPushMatrix();
	glRotated(-270, 0.0, 1.0, 0.0);
	glTranslated(-3.5, 0, -1.5);
	boy();
	glPopMatrix();
}

void humanMan1() {
	glPushMatrix();
	glTranslated(bx_position, 0, 3);
	glTranslated(xbj_position, bjump_1, 0);
	glTranslated(xbj_position, bdrop_1, 0);
	glRotated(turnboy, 0, 1, 0);
	glTranslated(xboy, 0, zboy);
	boy();
	glPopMatrix();
}

//girl walk
void humanWomen() {
	glPushMatrix();
	glTranslated(2.5, -0.7, 1.5);
	glRotated(-90, 0.0, 1.0, 0.0);
	girl();
	glPopMatrix();
}



//girl walk scene1
void humanWomen1() {
	glPushMatrix();
	glTranslated(gx_position, 0, 2.5);
	glRotated(-360, 0.0, 1.0, 0.0);
	glRotated(-360, 0.0, 1.0, 0.0);
	glTranslated(xx_position, 0, zf_position);
	glTranslated(xbj_position, gthink, 0);
	glTranslated(xbj_position, glow, 0);
	glRotated(turn, 0.0, 1.0, 0.0);
	glTranslated(xf_position, 0, zff_position);
	glTranslated(xz_position, 0, z_position);
	glTranslated(xz_position, 0, zz_position);
	glRotated(turnz, 0.0, 1.0, 0.0);
	glTranslated(xbj_position, gjump_1, 0);
	glTranslated(xbj_position, gdrop_1, 0);
	glRotated(turngirl, 0, 1, 0);
	glTranslated(xgirl, 0, zgirl);
	girl();
	glPopMatrix();
}



void humanManst() {
	glPushMatrix();
	glRotated(-270, 0.0, 1.0, 0.0);
	glTranslated(-3.5, 0, -1.5);
	boystatic();
	glPopMatrix();
}

void humanManst2() {
	glPushMatrix();
	glRotated(160, 0.0, 1.0, 0.0);
	glTranslated(-3.5, 0, -1.5);
	boystatic();
	glPopMatrix();
}

//girl walk
void humanWomenst() {
	glPushMatrix();
	glTranslated(2.5, -0.7, 1.5);
	glRotated(-90, 0.0, 1.0, 0.0);
	girlstatic();
	glPopMatrix();
}

//mother walk
void humanMother() {
	glPushMatrix();
	glTranslated(4, 0, 2.2);//angle + kalau mau kasi gerak
	glRotated(-270, 0, 1, 0);
	mother();
	glPopMatrix();
}
//mother walk scene1
void humanMother1() {
	glPushMatrix();
	glScaled(2, 2, 2);
	glRotated(180, 0, 1, 0);
	glTranslated(-6.5, 0, 1);
	glTranslated(xmotherp - 2, 0, -2.5);
	glRotated(10, 1, 0, 0);
	glRotated(360, 0, 1, 0);
	glRotated(turnm, 0, 1, 0);
	glTranslated(xmothers, 0, zmother);
	mother1();
	glPopMatrix();
}

void humanMotherst() {//baru
	glPushMatrix();
	glTranslated(4, 0, 2.2);//angle + kalau mau kasi gerak
	glRotated(-270, 0, 1, 0);
	motherst();
	glPopMatrix();
}

////////////////////////////////////// 2////////////////////////////////////////////

void humanBoy() {


	glPushMatrix();
	glTranslated(xrx_position, 0.3, 0.5);
	glTranslated(xbj_position, bjump, 0);
	glTranslated(xbj_position, bdrop, 0);
	boy();
	glPopMatrix();
}

//create tree trunk dimension
void squareTrunk() {
	glBegin(GL_QUADS);
	glColor3ub(205, 133, 63);
	glVertex3f(1.0, 1.0, 0.0);
	glVertex3f(0.9, 1.0, 0.0);
	glVertex3f(0.9, -0.7, 0.0);
	glVertex3f(1.0, -0.7, 0.0);
	glEnd();
}

//rotate leaf to specify direction
void rotateLeaf() {
	glPushMatrix();
	glScaled(0.3, 0.3, 0.3);
	glTranslated(3, 5, -1);
	glRotated(-30, 0.0, 0.0, 1.0);
	glRotated(-45, 0.0, 1.0, 0.0);
	leaf();
	glPopMatrix();

	glPushMatrix();
	glScaled(0.3, 0.3, 0.3);
	glTranslated(0, 5.7, -1.3);
	glRotated(-30, 0.0, 0.0, 1.0);
	glRotated(-45, 0.0, 1.0, 0.0);
	leaf();
	glPopMatrix();

	for (int i = 0; i < 3; i++) {
		glPushMatrix();
		glScaled(0.3, 0.3, 0.3);
		glTranslated(-1 - i, 5.7 + i, -1.3);
		glRotated(-30, 0.0, 0.0, 1.0);
		glRotated(-45, 0.0, 1.0, 0.0);
		leaf();
		glPopMatrix();
	}

	glPushMatrix();
	glScaled(0.3, 0.3, 0.3);
	glTranslated(3, 8, -1);
	glRotated(-30, 0.0, 0.0, 1.0);
	glRotated(-45, 0.0, 1.0, 0.0);
	leaf();
	glPopMatrix();

	glPushMatrix();
	glScaled(0.3, 0.3, 0.3);
	glTranslated(1, 7.1, -1.6);
	glRotated(30, 0.0, 0.0, 1.0);
	glRotated(-45, 0.0, 1.0, 0.0);
	leaf();
	glPopMatrix();

	glPushMatrix();
	glScaled(0.3, 0.3, 0.3);
	glTranslated(6, 5.5, -1);
	glRotated(30, 0.0, 0.0, 1.0);
	glRotated(-45, 0.0, 1.0, 0.0);
	leaf();
	glPopMatrix();

	glPushMatrix();
	glScaled(0.3, 0.3, 0.3);
	glTranslated(6.2, 7.1, -1.6);
	glRotated(30, 0.0, 0.0, 1.0);
	glRotated(-45, 0.0, 1.0, 0.0);
	leaf();
	glPopMatrix();

	glPushMatrix();
	glScaled(0.3, 0.3, 0.3);
	glTranslated(-6.2, 5.1, 1.6);
	glRotated(30, 0.0, 0.0, 1.0);
	glRotated(-45, 0.0, 1.0, 0.0);
	leaf();
	glPopMatrix();



	for (int n = 0; n < 5; n++) {
		//left down
		glPushMatrix();
		glScaled(0.3, 0.3, 0.3);
		glTranslated(-2.5 - n, 15 - n, 1.0);
		glRotated(30, 0.0, 0.0, 1.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		leaf();
		glPopMatrix();

		glPushMatrix();
		glScaled(0.3, 0.3, 0.3);
		glTranslated(3 - n, 15 - n, 1.0);
		glRotated(30, 0.0, 0.0, 1.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		leaf();
		glPopMatrix();

		//right down
		glPushMatrix();
		glScaled(0.3, 0.3, 0.3);
		glTranslated(-2.5 + n, 15 - n, 1.0);
		glRotated(30, 0.0, 0.0, 1.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		leaf();
		glPopMatrix();

		glPushMatrix();
		glScaled(0.3, 0.3, 0.3);
		glTranslated(2 + n, 15 - n, 1.0);
		glRotated(30, 0.0, 0.0, 1.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		leaf();
		glPopMatrix();

		glPushMatrix();
		glScaled(0.3, 0.3, 0.3);
		glTranslated(2.5 + n, 15 - n, 1.0);
		glRotated(30, 0.0, 0.0, 1.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		leaf();
		glPopMatrix();

		//down
		glPushMatrix();
		glScaled(0.3, 0.3, 0.3);
		glTranslated(0, 17 - n, 1.0);
		glRotated(30, 0.0, 0.0, 1.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		leaf();
		glPopMatrix();

		//down front
		glPushMatrix();
		glScaled(0.3, 0.3, 0.3);
		glTranslated(-2.5, 15 - n, 1.0 + n);
		glRotated(30, 0.0, 0.0, 1.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		leaf();
		glPopMatrix();

		glPushMatrix();
		glScaled(0.3, 0.3, 0.3);
		glTranslated(0, 15 - n, 1.0 + n);
		glRotated(30, 0.0, 0.0, 1.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		leaf();
		glPopMatrix();

		glPushMatrix();
		glScaled(0.3, 0.3, 0.3);
		glTranslated(2, 15 - n, 1.0 + n);
		glRotated(30, 0.0, 0.0, 1.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		leaf();
		glPopMatrix();

		//down front
		glPushMatrix();
		glScaled(0.3, 0.3, 0.3);
		glTranslated(-2.5, 15 - n, 1.0 - n);
		glRotated(30, 0.0, 0.0, 1.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		leaf();
		glPopMatrix();

		glPushMatrix();
		glScaled(0.3, 0.3, 0.3);
		glTranslated(-2.5 + n, 6.1 + n, 1.0);
		glRotated(30, 0.0, 0.0, 1.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		leaf();
		glPopMatrix();

		glPushMatrix();
		glScaled(0.3, 0.3, 0.3);
		glTranslated(-2.5 + n, 8.1 + n, 1.0);
		glRotated(30, 0.0, 0.0, 1.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		leaf();
		glPopMatrix();

		glPushMatrix();
		glScaled(0.3, 0.3, 0.3);
		glTranslated(-2.5 - n, 6.1 + n, 1.0);
		glRotated(30, 0.0, 0.0, 1.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		leaf();
		glPopMatrix();

		glPushMatrix();
		glScaled(0.3, 0.3, 0.3);
		glTranslated(-2.5 - n, 8.1 + n, 1.0);
		glRotated(30, 0.0, 0.0, 1.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		leaf();
		glPopMatrix();

		glPushMatrix();
		glScaled(0.3, 0.3, 0.3);
		glTranslated(-2.5, 6.1 + n, 1.0 + n);
		glRotated(30, 0.0, 0.0, 1.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		leaf();
		glPopMatrix();

		glPushMatrix();
		glScaled(0.3, 0.3, 0.3);
		glTranslated(-2.5, 8.1 + n, 1.0 + n);
		glRotated(30, 0.0, 0.0, 1.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		leaf();
		glPopMatrix();

		glPushMatrix();
		glScaled(0.3, 0.3, 0.3);
		glTranslated(-2.5, 6.1 + n, 1.0 - n);
		glRotated(30, 0.0, 0.0, 1.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		leaf();
		glPopMatrix();

		glPushMatrix();
		glScaled(0.3, 0.3, 0.3);
		glTranslated(-2.5, 8.1 + n, 1.0 - n);
		glRotated(30, 0.0, 0.0, 1.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		leaf();
		glPopMatrix();

		glPushMatrix();
		glScaled(0.3, 0.3, 0.3);
		glTranslated(-2.5, 6.1 + n, 1.0);
		glRotated(30, 0.0, 0.0, 1.0);
		glRotated(-20, 0.0, 1.0, 0.0);
		leaf();
		glPopMatrix();
	}
}

//create tree branch
void squareT() {
	glBegin(GL_QUADS);
	glColor3ub(205, 133, 63);
	glVertex3f(1.0, 1.0, 0.0);
	glVertex3f(0.7, 1.0, 0.0);
	glVertex3f(0.7, -1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glEnd();
}

//create tree branch
void tree() {
	//front
	glPushMatrix();
	glTranslated(-1.0, 1.0, -0.7);
	squareT();
	glPopMatrix();

	//back
	glPushMatrix();
	glTranslated(-1.0, 1.0, -1);
	squareT();
	glPopMatrix();

	//right
	glPushMatrix();
	glTranslated(0.0, 1.0, 0.0);
	glRotated(90, 0.0, 1.0, 0.0);
	squareT();
	glPopMatrix();

	//left
	glPushMatrix();
	glTranslated(-0.3, 1.0, 0.0);
	glRotated(-270, 0.0, 1.0, 0.0);
	squareT();
	glPopMatrix();
}




///////////////////////CREATE TREE SCENE 3/////////////////////////////////
void trunk() {
	GLUquadricObj* pObj;
	pObj = gluNewQuadric();
	gluQuadricNormals(pObj, GLU_SMOOTH);

	glPushMatrix();
	glColor3ub(244, 164, 96);
	glRotatef(270, 1, 0, 0);
	gluCylinder(pObj, 0.2, 0.1, 1.8, 20, 25);
	glPopMatrix();
}

//rotate trunk to specific direction
void rotateTrunk() {
	glPushMatrix();
	glTranslated(-0.2, 1.3, 0.1);
	glRotated(-45, 0.0, 0.0, 1.0);
	trunk();
	glPopMatrix();

	glPushMatrix();
	glScaled(1, 0.7, 1);
	glTranslated(-0.1, 1, 0.1);
	glRotated(45, 0.0, 0.0, 1.0);
	trunk();
	glPopMatrix();

	glPushMatrix();
	glScaled(1, 0.7, 1);
	glTranslated(-0.7, 1.2, 0.1);
	trunk();
	glPopMatrix();

	glPushMatrix();
	glScaled(1, 0.4, 1);
	glTranslated(-1.6, 2.6, -1.3);
	glRotated(45, 1, 0, 0);
	trunk();
	glPopMatrix();

	glPushMatrix();
	glScaled(0.7, 1, 1);
	glTranslated(1.2, 1.3, 0.1);
	glRotated(-45, 0.0, 0.0, 1.0);
	trunk();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 2.5, 0.);
	glRotated(-90, 0.0, 0.0, 1.0);
	trunk();
	glPopMatrix();
}

void treeLocation1() {
	glPushMatrix();
	glTranslated(-1, 0, 1);
	glColor3ub(50, 205, 50);
	rotateLeaf();
	tree();

	glPopMatrix();
}

void treeLocation2() {
	glPushMatrix();
	glTranslated(-10, 0, 0.5);
	glColor3ub(50, 205, 50);
	rotateLeaf();
	tree();

	glPopMatrix();
}

void treeLocation3() {
	glPushMatrix();
	glScaled(0.5, 0.5, 0.5);
	glTranslated(-14, 0, -0.5);
	glColor3ub(50, 205, 50);
	rotateLeaf();
	tree();

	glPopMatrix();
}

void treeLocation4() {
	glPushMatrix();
	glTranslated(-4, 0, 1);
	glColor3ub(50, 205, 50);
	rotateLeaf();
	tree();

	glPopMatrix();
}

void treeLocation5() {
	glPushMatrix();
	glScaled(0.5, 0.5, 0.5);
	glTranslated(-6, 0, -0.5);
	glColor3ub(50, 205, 50);
	rotateLeaf();
	tree();

	glPopMatrix();
}

void treeLocation6() {
	glPushMatrix();
	glScaled(0.5, 0.5, 0.5);
	glTranslated(-1, 0, -0.5);
	glColor3ub(50, 205, 50);
	rotateLeaf();
	tree();

	glPopMatrix();
}

void treeLocation7() {
	glPushMatrix();
	glTranslated(-1.5, 0, -1);
	glColor3ub(50, 205, 50);
	rotateLeaf();
	tree();

	glPopMatrix();
}

//branch
void branch() {
	GLUquadricObj* pObj;
	pObj = gluNewQuadric();
	gluQuadricNormals(pObj, GLU_SMOOTH);
	glPushMatrix();
	glColor3ub(104, 70, 14);
	glTranslatef(-0.3, -0.4, 0);
	glRotatef(330, 1, 0, 0);
	gluCylinder(pObj, 0.1, 0.07, 0.5, 25, 25);
	glPopMatrix();
}

//leaf
void leaf1() {
	//daun
	glPushMatrix();
	glColor3ub(173, 255, 47);
	glScaled(0.5, 0.5, 0.5);
	glTranslatef(0, 1.0, 1.3);
	glutSolidDodecahedron();
	glPopMatrix();
}

void brancandleaf1() {
	leaf1();
}

void treesss() {
	trunk();

	glPushMatrix();
	glRotatef(0, 1, 0, 0);
	glTranslatef(0.3, 1.8, 0);
	brancandleaf1();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0.3, 0.2, 2);
	brancandleaf1();
	glPopMatrix();

	glPushMatrix();
	glRotatef(0, 1, 0, 0);
	glTranslatef(1.2, 2.2, 0);
	brancandleaf1();
	glPopMatrix();

	glPushMatrix();
	glRotatef(0, 1, 0, 0);
	glTranslatef(-0.5, 2.2, 0);
	brancandleaf1();
	glPopMatrix();

	glPushMatrix();
	glRotatef(0, 1, 0, 0);
	glTranslatef(0.3, 2.8, -0.3);
	brancandleaf1();
	glPopMatrix();
}

//SCENE 1
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//	glOrtho(-5, 5, -5, 5, 0.0001, 1000.0);
	gluPerspective(80.0, 1.0, 0.0001, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(8.0, 4.0, 8.0, 1, 1.5, 0.0, 0.0, 1.0, 0.0);

	GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //Color (0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	//Add positioned light
	GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //Color (0.5, 0.5, 0.5)
	GLfloat lightPos0[] = { 0.0f, -8.0f, 8.0f, 1.0f }; //Positioned at (4, 0, 8)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	//Add directed light
	GLfloat lightColor1[] = { 0.5f, 0.2f, 0.2f, 1.0f }; //Color (0.5, 0.2, 0.2)
	GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);


	glPushMatrix();
	humanMan1();
	humanWomen1();
	ground();
	glPopMatrix();

	glPushMatrix();
	glTranslated(2, 0, 0);
	glRotated(360.0, 0, 1, 0);
	humanMother1();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(-5, 0.0, 1.0, 0.0);
	glTranslated(cloudmove, 4.5, 1.0);
	cloud();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(-5, 0.0, 1.0, 0.0);
	glTranslated(cloudmove - 2, 5.5, 2.0);
	cloud();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(-5, 0.0, 1.0, 0.0);
	glTranslated(cloudmove + 2, 5.5, 1.5);
	cloud();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(-5, 0.0, 1.0, 0.0);
	glTranslated(cloudmove + 4, 4.0, 1.0);
	cloud();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(-5, 0.0, 1.0, 0.0);
	glTranslated(cloudmove + 5, 5.5, 2.0);
	cloud();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(-5, 0.0, 1.0, 0.0);
	glTranslated(cloudmove + 7, 5.5, 1.5);
	cloud();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(-5, 0.0, 1.0, 0.0);
	glTranslated(cloudmove + 7, 4.0, 1.0);
	cloud();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(-5, 0.0, 1.0, 0.0);
	glTranslated(cloudmove + 11, 4.0, 2.0);
	cloud();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(-5, 0.0, 1.0, 0.0);
	glTranslated(cloudmove + 9, 5.5, 1.5);
	cloud();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(-5, 0.0, 1.0, 0.0);
	glTranslated(cloudmove + 9, 4.0, 1.0);
	cloud();
	glPopMatrix();

	glPushMatrix();
	glTranslated(2, -0.3, 3);
	treesss();
	glPopMatrix();

	if (cloudmove < 5) {
		cloudmove += 0.01;
	}


	switch (action) {
	case 1:
		angle += inc;
		boyhand(angle);
		if (angle > 30)
		{
			angle = 30;
			inc = -3;
		}
		if (angle < -10)
		{
			angle = -10;
			inc = +3;
		}


		if (bx_position < 3) {
			bx_position += 0.05;
		}

		if (gx_position < 8) {
			gx_position += 0.05;
		}

		if (bx_position > 3) {
			angle = 0;
		}

	case 2:// girl stop and turn and walk to the boy
		if (gx_position > 8) {
			if (gthink < 0.2) {
				gthink += 0.03f;
			}

			if (gthink > 0.2) {
				glow = -0.2;
			}
		}

		//girl turn face the boy
		if (glow < -0.2) {
			xx_position = -4;
			zf_position = 4.6;
			turn = -180;

			//girl move near the boy
			if (xf_position < 8) {
				xf_position += 0.03f;
			}

			if (xf_position > 1) {
				xf_position = 1;
				angle0 = 0;
			}
		}
	case 3: // boy and girl talk
		if (xf_position == 1) {
			if (bjump_1 < 0.2) {
				bjump_1 += 0.03f;
			}

			if (bjump_1 > 0.2) {
				bdrop_1 = -0.2;
			}
		}

		if (gdrop_1 > bdrop_1) {
			if (gjump_1 < 0.2) {
				gjump_1 += 0.03f;
			}

			if (gjump_1 > 0.2) {
				gdrop_1 = -0.2;
			}
		}
	case 4://mother walk to front

		if (gdrop_1 < -0.2) {
			xmotherp += 0.02f;
			angle1 += inc1;
			girlhand(angle1);
			if (angle1 > 30)
			{
				angle1 = 30;
				inc1 = -3;
			}
			if (angle1 < -10)
			{
				angle1 = -10;
				inc1 = +3;
			}
		}
		if (xmotherp > 8.5) {
			xmotherp = 8.5;
			xmothers = 0.5;
			zmother = -3.8;
			turnm = 90;
			angle1 = 0;
		}

	case 5://girl meet her mother
		angle0 += inc0;
		girlhand(angle0);
		if (angle0 > 30)
		{
			angle0 = 30;
			inc0 = -3;
		}
		if (angle0 < -10)
		{
			angle0 = -10;
			inc0 = +3;
		}
		if (turnm == 90) {

			z_position = 4.5;
			xz_position = 0.2;
			turnz = -90;
		}

		if (turnz == -90) {
			zz_position += 0.05;
		}

		if (zz_position > 2.5) {
			zz_position = 2.5;
		}

		if (zz_position == 2.5) {
			xgirl = 3.5;
			zgirl = -4.5;
			turngirl = 180;
			turnboy = -270;
			xboy = -3.4;
			zboy = -0.3;
			angle0 = 0;
		}

	}
	glFlush();
	glutSwapBuffers();
}

//SCENE 3 DISPLAY1
void display3_1() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//	glOrtho(-5, 5, -5, 5, 0.0001, 1000.0);
	gluPerspective(45.0, 1.0, 0.0001, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 3.0, 7.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//gluLookAt(5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//Add ambient light
	GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //Color (0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	//Add positioned light
	GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //Color (0.5, 0.5, 0.5)
	GLfloat lightPos0[] = { 0.0f, -8.0f, 8.0f, 1.0f }; //Positioned at (4, 0, 8)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	//Add directed light
	GLfloat lightColor1[] = { 0.5f, 0.2f, 0.2f, 1.0f }; //Color (0.5, 0.2, 0.2)
	GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	glEnable(GL_COLOR_MATERIAL); // cheat!!!!
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glShadeModel(GL_SMOOTH); //Enable smooth shading


	glPushMatrix();
	glTranslated(0, bjump, 0);
	glTranslated(0, bdrop, 0);
	humanMan();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, gjump, 0);
	glTranslated(0, gdrop, 0);
	humanWomen();
	glPopMatrix();

	glPushMatrix();
	ground();
	humanMother();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.4, 0, 0);
	treesss();
	glPopMatrix();

	if (bjump < 0.2) {
		bjump += 0.03f;
	}

	if (bjump > 0.2) {
		bdrop = -0.8;
	}
	if (gdrop > bdrop) {
		if (gjump < 0.2) {
			gjump += 0.03f;
		}

		if (gjump > 0.2) {
			gdrop = -0.2;
		}
	}

	glFlush();
	glutSwapBuffers();
}

//SCENE 3 DISPLAY2
void display3_2() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//	glOrtho(-5, 5, -5, 5, 0.0001, 1000.0);
	gluPerspective(45.0, 1.0, 0.0001, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0.0, 3.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	gluLookAt(5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//Add ambient light
	GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //Color (0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	//Add positioned light
	GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //Color (0.5, 0.5, 0.5)
	GLfloat lightPos0[] = { 0.0f, -8.0f, 8.0f, 1.0f }; //Positioned at (4, 0, 8)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	//Add directed light
	GLfloat lightColor1[] = { 0.5f, 0.2f, 0.2f, 1.0f }; //Color (0.5, 0.2, 0.2)
	GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	glEnable(GL_COLOR_MATERIAL); // cheat!!!!
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glShadeModel(GL_SMOOTH); //Enable smooth shading

	glPushMatrix();
	glTranslated(0, 0, 0);
	humanManst();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0 + walkk, 0, 0);
	glRotated(90.0, 1.0, 30.0, 0.2);
	humanWomenst();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.5 + walkk, -0.7, 0);
	glRotated(90.0, 1.0, 30.0, 0.2);
	humanMother();
	glPopMatrix();

	glPushMatrix();
	ground();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-1, 0, 0);
	treesss();
	glPopMatrix();

	if (walkk < 10) {
		walkk += 0.05;
	}

	glFlush();
	glutSwapBuffers();
}


//SCENE 4 DISPLAY
void display4() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//	glOrtho(-5, 5, -5, 5, 0.0001, 1000.0);
	gluPerspective(45.0, 1.0, 0.0001, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camangle * -0.4, 1.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//Add ambient light
	GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //Color (0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	//Add positioned light
	GLfloat lightColor0[] = { 0.6f, 0.5f, 0.5f, 1.0f }; //Color (0.5, 0.5, 0.5)
	GLfloat lightPos0[] = { 0.0f, -8.0f, 8.0f, 1.0f }; //Positioned at (4, 0, 8)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	//Add directed light
	GLfloat lightColor1[] = { 1,1,1,1 }; //Color (0.5, 0.2, 0.2)
	GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	glEnable(GL_COLOR_MATERIAL); // cheat!!!!
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glShadeModel(GL_SMOOTH); //Enable smooth shading


	glPushMatrix();//cloud
	glRotated(angle + 180.0, 0.0, 1.0, 0.0);
	glTranslated(-0.9, 0.0, 1.0);
	cloud();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(angle + 180.0, 0.0, 1.0, 0.0);
	glTranslated(1, 1.9, -1.0);
	cloud();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(angle + 180.0, 0.0, 1.0, 0.0);
	glTranslated(-0.9, 2.0, -1.0);
	cloud();
	glPopMatrix();
	glPushMatrix();

	glPushMatrix();//cloud
	glRotated(angle + 180.0, 0.0, 1.0, 0.0);
	glTranslated(-1.9, -1.0, 1.8);
	cloud();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(angle + 180.0, 0.0, 1.0, 0.0);
	glTranslated(0.6, 0.6, -1.0);
	cloud();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(angle + 180.0, 0.0, 1.0, 0.0);
	glTranslated(3.0, 2.2, 1.2);
	cloud();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(angle + 180.0, 0.0, 1.0, 0.0);
	glTranslated(2.4, 0.2, 1.2);
	cloud();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(angle + 180.0, 0.0, 1.0, 0.0);
	glTranslated(-1.2, -2.7, 5.4);
	cloud();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(angle + 180.0, 0.0, 1.0, 0.0);
	glTranslated(2.2, -2.7, 1.4);
	cloud();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(angle + 180.0, 0.0, 1.0, 0.0);
	glTranslated(0.5, -2.2, 1.4);
	cloud();
	glPopMatrix();

	glPushMatrix();//skyback
	glColor3ub(153, 223, 255);
	glTranslated(-3.9, 2.6, -2.0);
	skycolor();
	glPopMatrix();

	glPushMatrix();//skyright
	glColor3ub(153, 223, 255);
	glRotated(270.0, 0.0, 1.0, 0.0);
	glTranslated(-1.9, 2.6, -2.0);
	skycolor();
	glPopMatrix();

	glPushMatrix();//skydown
	glColor3ub(153, 223, 255);
	glRotated(-90.0, 1.0, 0.0, 0.0);
	glTranslated(0.0, -20.0, -2.0);
	skycolor();
	glPopMatrix();

	planemove1();

	glPopMatrix();

	if (planex > -13) {
		planex = planex - 0.02;
		planey = planey - 0.02;
	}
	if (angle < 100) {
		angle += 1.0;
	}
	if (camangle < 100) {
		camangle += 0.1;
	}

	glFlush();
	glutSwapBuffers();
}

//scene 5 display
void display5() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 0.0001, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(4.0, 2.5, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	//Add ambient light
	GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //Color (0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	//Add positioned light
	GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //Color (0.5, 0.5, 0.5)
	GLfloat lightPos0[] = { 0.0f, -8.0f, 8.0f, 1.0f }; //Positioned at (4, 0, 8)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	//Add directed light
	GLfloat lightColor1[] = { 0.5f, 0.2f, 0.2f, 1.0f }; //Color (0.5, 0.2, 0.2)
	GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	glEnable(GL_COLOR_MATERIAL); // cheat!!!!
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glShadeModel(GL_SMOOTH); //Enable smooth shading

	glPushMatrix();
	//girlstatic();
	ground();
	glPopMatrix();

	/* main rec */
	;
	glColor3f(.5, .75, .35);
	glBegin(GL_POLYGON);
	glVertex3f(0.2, 0.1, 0.0);
	glVertex3f(0.9, 0.1, 0.0);
	glVertex3f(0.9, 0.575, 0.0);
	glVertex3f(0.2, 0.575, 0.0);
	glEnd();


	/*back rec*/

	glColor3f(.5, .75, .35);
	glBegin(GL_POLYGON);
	glVertex3f(0.2, 0.1, 0.5);
	glVertex3f(0.9, 0.1, 0.5);
	glVertex3f(0.9, 0.575, 0.5);
	glVertex3f(0.2, 0.575, 0.5);
	glEnd();


	/* left rec */

	glColor3f(.75, 0.75, .25);
	glBegin(GL_POLYGON);
	glVertex3f(0.2, 0.1, 0.5);
	glVertex3f(0.2, 0.1, 0.0);
	glVertex3f(0.2, 0.575, 0.0);
	glVertex3f(0.2, 0.575, 0.5);
	glEnd();


	/* right rec */

	glColor3f(.75, 0.75, .25);
	glBegin(GL_POLYGON);
	glVertex3f(0.9, 0.1, 0.5);
	glVertex3f(0.9, 0.1, 0.0);
	glVertex3f(0.9, 0.575, 0.0);
	glVertex3f(0.9, 0.575, 0.5);
	glEnd();


	/* left tri */

	glColor3f(.5, 0.5, .25);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.9, 0.575, 0.0);
	glVertex3f(0.9, 0.575, 0.5);
	glVertex3f(0.9, 0.8, 0.25);
	glEnd();


	/* right tri */

	glColor3f(.5, 0.5, .25);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.2, 0.575, 0.0);
	glVertex3f(0.2, 0.575, 0.5);
	glVertex3f(0.2, 0.8, 0.25);
	glEnd();


	/* roof */

	glColor3f(.55, 0.35, .2);
	glBegin(GL_POLYGON);
	glVertex3f(0.2, 0.575, 0.0);
	glVertex3f(0.9, 0.575, 0.0);
	glVertex3f(0.9, 0.8, 0.25);
	glVertex3f(0.2, 0.8, 0.25);
	glEnd();



	glPushMatrix();
	glColor3f(.55, 0.35, .2);
	glBegin(GL_POLYGON);
	glVertex3f(0.2, 0.575, 0.5);
	glVertex3f(0.9, 0.575, 0.5);
	glVertex3f(0.9, 0.8, 0.25);
	glVertex3f(0.2, 0.8, 0.25);
	glEnd();


	/* door */

	glColor3f(.15, 0.2, .3);
	glBegin(GL_POLYGON);
	glVertex3f(0.47, 0.105, 0.0);
	glVertex3f(0.65, 0.105, 0.0);
	glVertex3f(0.65, 0.46, 0.0);
	glVertex3f(0.47, 0.46, 0.0);
	glEnd();


	/* window 1 */

	glColor3f(.3, 0.2, .1);
	glBegin(GL_POLYGON);
	glVertex3f(0.72, 0.25, 0.0);
	glVertex3f(0.83, 0.25, 0.0);
	glVertex3f(0.83, 0.4, 0.0);
	glVertex3f(0.72, 0.4, 0.0);
	glEnd();


	/* window 2 */

	glColor3f(.3, 0.2, .1);
	glBegin(GL_POLYGON);
	glVertex3f(0.27, 0.25, 0.0);
	glVertex3f(0.38, 0.25, 0.0);
	glVertex3f(0.38, 0.4, 0.0);
	glVertex3f(0.27, 0.4, 0.0);
	glEnd();

	glPushMatrix();
	glTranslated(-1, 0, 0);
	treesss();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(-5, 0.0, 1.0, 0.0);
	glTranslated(cloudmove + 7, 2, 1);
	cloud();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(-5, 0.0, 1.0, 0.0);
	glTranslated(cloudmove + 3, 1, 1);
	cloud();
	glPopMatrix();


	if (cloudmove < 5) {
		cloudmove -= 0.01;
	}

	glFlush();
	glutSwapBuffers();
}


//scene 6 display
void display6() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 0.0001, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//Add ambient light
	GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //Color (0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	//Add positioned light
	GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //Color (0.5, 0.5, 0.5)
	GLfloat lightPos0[] = { 0.0f, -8.0f, 8.0f, 1.0f }; //Positioned at (4, 0, 8)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	//Add directed light
	GLfloat lightColor1[] = { 0.5f, 0.2f, 0.2f, 1.0f }; //Color (0.5, 0.2, 0.2)
	GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	glEnable(GL_COLOR_MATERIAL); // cheat!!!!
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glShadeModel(GL_SMOOTH); //Enable smooth shading

	glPushMatrix();
	ground();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, 0);
	humanManst();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(-5, 0.0, 1.0, 0.0);
	glTranslated(cloudmove, 2, 1);
	cloud();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-1, 0, 0);
	treesss();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(-5, 0.0, 1.0, 0.0);
	glTranslated(cloudmove + 1, 1, 1);
	cloud();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(-5, 0.0, 1.0, 0.0);
	glTranslated(cloudmove + 4, 2, -1);
	cloud();
	glPopMatrix();

	glPushMatrix();//cloud
	glRotated(-45, 0.0, 5.0, 0.0);
	glTranslated(cloudmove - 4, 1, 1);
	cloud();
	glPopMatrix();

	if (cloudmove < 5) {
		cloudmove += 0.01;
	}

	glFlush();
	glutSwapBuffers();
}

void display7() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	//Add ambient light
	GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //Color (0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	//Add positioned light
	GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //Color (0.5, 0.5, 0.5)
	GLfloat lightPos0[] = { 0.0f, -8.0f, 8.0f, 1.0f }; //Positioned at (4, 0, 8)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	//Add directed light
	GLfloat lightColor1[] = { 0.5f, 0.2f, 0.2f, 1.0f }; //Color (0.5, 0.2, 0.2)
	GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 0.1, 1000.0);
	cam.Set(Point3D(-2.5, 2, 9.0), Point3D(2, 4.5, -50), Vector3D(0.0, 5, 0.0));

	glPushMatrix();
	BuildingShapeLocation1();
	BuildingShapeLocation2();
	BuildingShapeLocation3();
	BuildingShapeLocation4();
	BuildingShapeLocation5();
	BuildingShapeLocation6();
	BuildingShapeLocation7();

	drawRain();
	Cland();
	glPopMatrix();
	humanBoy();

	if (cloudmove < 5) {
		cloudmove += 0.02;
	}

	glPushMatrix();
	glTranslatef(-5, 11, -30);
	glTranslated(cloudmove + 0.5, 1, 2);
	glScalef(5, 5, 5);
	cloud();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4, 19, -30);
	glTranslated(cloudmove + 1, 1, 2);
	glScalef(5, 5, 5);
	cloud();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-7, 10, -10);
	glTranslated(cloudmove + 1, 1, 2);
	glScalef(5, 5, 5);
	cloud();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15, 23, -40);
	glTranslated(cloudmove + 1, 1, 2);
	glScalef(5, 5, 5);
	cloud();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-17, 19, -30);
	glTranslated(cloudmove + 0.3, 1, 2);
	glScalef(5, 5, 5);
	cloud();
	glPopMatrix();



	switch (action) {
	case 1:
		angle += inc;

		if (angle > 30)
		{
			angle = 30;
			inc = -3;
		}
		if (angle < -10)
		{
			angle = -10;
			inc = +3;
		}


		if (xyx_position < 3) {
			xyx_position += 0.04;
		}

		if (xrx_position < 8) {
			xrx_position += 0.04;
		}

		if (xrx_position > 3) {
			angle = 0;
		}
	}
	glFlush();
	glutSwapBuffers();


}


//SCENE 8
void display8() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	//Add ambient light
	GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //Color (0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	//Add positioned light
	GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //Color (0.5, 0.5, 0.5)
	GLfloat lightPos0[] = { 0.0f, -8.0f, 8.0f, 1.0f }; //Positioned at (4, 0, 8)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	//Add directed light
	GLfloat lightColor1[] = { 0.5f, 0.2f, 0.2f, 1.0f }; //Color (0.5, 0.2, 0.2)
	GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 0.1, 1000.0);
	cam.Set(Point3D(4, 2, 5), Point3D(-100, 5, -60), Vector3D(0.0, 5, 0.0));
	float xyxz_position = -50;

	glPushMatrix();
	treeLocation1();
	treeLocation2();
	treeLocation3();
	treeLocation4();
	treeLocation5();
	treeLocation6();
	//treeLocation7();

	drawRain();
	Cland();
	glPopMatrix();
	humanBoy()
		
	switch (action) {
	case 1:
		angle += inc;

		if (angle > 30)
		{
			angle = 30;
			inc = -3;
		}
		if (angle < -10)
		{
			angle = -10;
			inc = +3;
		}


		if (xyxz_position < 7) {
			xyxz_position += 0.04;
		}

		if (xrx_position < 8) {
			xrx_position += 0.04;
		}

		if (xrx_position > 3) {
			angle = 0;
		}
	}
	glFlush();
	glutSwapBuffers();


}

//SCENE 9 BLASE
void display9() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 0.0001, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(-0.5, 1.0, 10.0, 0.0, 1.5, 0.0, 0.0, 1.0, 0.0);
	//Add ambient light
	GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	//Add positioned light
	GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat lightPos0[] = { 0.0f, -8.0f, 8.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	//Add directed light
	GLfloat lightColor1[] = { 0.5f, 0.2f, 0.2f, 1.0f }; //Color (0.5, 0.2, 0.2)
	GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	glEnable(GL_COLOR_MATERIAL); // cheat!!!!
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glShadeModel(GL_SMOOTH); //Enable smooth shading

	glPushMatrix();
	glTranslated(-1.0, 0, 3);
	humanMan();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0.7, 6 - walkk);
	humanWomen();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.9, 0.5, 3);
	DrawUmbrellaStick();
	glPopMatrix();

	glPushMatrix();
	glScaled(0.7, 0.7, 0.7);
	glTranslated(-1.1, 1.4, 7);
	DrawUmbrellaTop();
	glPopMatrix();

	glPushMatrix();
	ground();
	glPopMatrix();

	glPushMatrix();
	drawRain();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.4, 0, 0);
	treesss();
	glPopMatrix();

	if (walkk < 2) {
		walkk += 0.05;
	}

	glFlush();
	glutSwapBuffers();
}

//SCENE 10 
void display10() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 0.0001, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(-0.5, 1.0, 7.0, 0.0, 1.5, 0.0, 0.0, 1.0, 0.0);
	//Add ambient light
	GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //Color (0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	//Add positioned light
	GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //Color (0.5, 0.5, 0.5)
	GLfloat lightPos0[] = { 0.0f, -8.0f, 8.0f, 1.0f }; //Positioned at (4, 0, 8)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	//Add directed light
	GLfloat lightColor1[] = { 0.5f, 0.2f, 0.2f, 1.0f }; //Color (0.5, 0.2, 0.2)
	GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	glEnable(GL_COLOR_MATERIAL); // cheat!!!!
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glShadeModel(GL_SMOOTH); //Enable smooth shading

	glPushMatrix();
	glTranslated(-1.3, 0, 2);
	ManUmbrella();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.5 - walkk, 0.7, 3 + walkk);
	glRotatef(135.0, 0.0, 1.0, 0.0);
	humanWomen();
	glPopMatrix();

	glPushMatrix();
	glScaled(0.8, 0.8, 0.8);
	glTranslated(-0.85, 1.1, 5.5);
	DrawUmbrellaStick();
	glPopMatrix();

	glPushMatrix();
	glScaled(0.65, 0.65, 0.65);
	glTranslated(-1.0, 1.8, 7);
	DrawUmbrellaTop();
	glPopMatrix();

	glPushMatrix();
	ground();
	glPopMatrix();

	glPushMatrix();
	drawRain();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-1, 0, 0);
	treesss();
	glPopMatrix();

	if (walkk < 0.5) {
		walkk += 0.05;
	}
	glFlush();
	glutSwapBuffers();
}
void display11() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 0.0001, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//Add ambient light
	GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //Color (0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	//Add positioned light
	GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //Color (0.5, 0.5, 0.5)
	GLfloat lightPos0[] = { 0.0f, -8.0f, 8.0f, 1.0f }; //Positioned at (4, 0, 8)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	//Add directed light
	GLfloat lightColor1[] = { 0.5f, 0.2f, 0.2f, 1.0f }; //Color (0.5, 0.2, 0.2)
	GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	glEnable(GL_COLOR_MATERIAL); // cheat!!!!
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glShadeModel(GL_SMOOTH); //Enable smooth shading

	glPushMatrix();
	ground();
	glPopMatrix();

	glPushMatrix();
	drawRain();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, gjump, 0);
	glTranslated(0, gdrop, 0);
	humanManst();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, gjump, 0);
	glTranslated(0, gdrop, 0);
	humanWomen();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-1, 0, 0);
	treesss();
	glPopMatrix();

	if (bjump < 0.2) {
		bjump += 0.03f;
	}

	if (bjump > 0.2) {
		bdrop = -0.8;
	}
	if (gdrop > bdrop) {
		if (gjump < 0.2) {
			gjump += 0.03f;
		}

		if (gjump > 0.2) {
			gdrop = -0.2;
		}
	}


	glFlush();
	glutSwapBuffers();
}


void display12() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 0.0001, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//Add ambient light
	GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //Color (0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	//Add positioned light
	GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //Color (0.5, 0.5, 0.5)
	GLfloat lightPos0[] = { 0.0f, -8.0f, 8.0f, 1.0f }; //Positioned at (4, 0, 8)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	//Add directed light
	GLfloat lightColor1[] = { 0.5f, 0.2f, 0.2f, 1.0f }; //Color (0.5, 0.2, 0.2)
	GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	glEnable(GL_COLOR_MATERIAL); // cheat!!!!
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glShadeModel(GL_SMOOTH); //Enable smooth shading

	glPushMatrix();
	ground();
	glPopMatrix();

	glPushMatrix();
	glTranslated(1, 0.7, 1.4);
	humanWomen();
	glPopMatrix();

	glPushMatrix();
	glPopMatrix();

	glPushMatrix();

	glPopMatrix();

	glPushMatrix();
	glTranslated(1.5, 0.7, -0.1);
	humanMan();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-1, 0, 0);
	treesss();
	glPopMatrix();



	glFlush();
	glutSwapBuffers();
}



void idle() {
	DWORD start = GetTickCount64();
	while (GetTickCount64() - start < 1000 / FPS);

	angle += 0.1;
	if (angle > 360.0) angle = 0.0;

	glutPostRedisplay();
}

void initGL() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glPointSize(10.0);
	glLineWidth(5.0);

	// depth buffer
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1); //Enable light #1
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glShadeModel(GL_SMOOTH); //Enable smooth shading
}

int main(int argc, char** argv) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc, argv);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("Childhood Love");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutTimerFunc(1000, MyTimerFunc, 0);
	initGL();
	init();


	glutMainLoop();
}

void MyTimerFunc(int value)
{
	if (value == 0) // passed in in main
	{
		glutDisplayFunc(display);
		glutIdleFunc(display);
		glutIdleFunc(idle);
		// Change to a new display function in 2 seconds
		glutTimerFunc(20000, MyTimerFunc, 1);
	}


	else if (value == 1)
	{
		glutDisplayFunc(display3_1);
		glutIdleFunc(display3_1);
		glutIdleFunc(idle);
		// Change to a new display function in 2 seconds
		glutTimerFunc(4000, MyTimerFunc, 2);
	}
	else if (value == 2)
	{
		glutDisplayFunc(display3_2);
		glutIdleFunc(display3_2);
		glutIdleFunc(idle);
		// Change to a new display function in 2 seconds
		glutTimerFunc(4000, MyTimerFunc, 3);
	}
	else if (value == 3)
	{
		glutDisplayFunc(display4);
		glutIdleFunc(display4);
		glutIdleFunc(idle);
		//etc.  Stop calling glutTimerFunc when no longer need the timer
		glutTimerFunc(9000, MyTimerFunc, 4);
	}
	else if (value == 4)
	{
		glutDisplayFunc(display5);
		glutIdleFunc(display5);
		glutIdleFunc(idle);
		//etc.  Stop calling glutTimerFunc when no longer need the timer
		glutTimerFunc(6000, MyTimerFunc, 5);
	}
	else if (value == 5)
	{
		glutDisplayFunc(display6);
		glutIdleFunc(display6);
		glutIdleFunc(idle);
		//etc.  Stop calling glutTimerFunc when no longer need the timer
		glutTimerFunc(8000, MyTimerFunc, 6);
	}
	else if (value == 6)
	{
		glutDisplayFunc(display7);
		glutIdleFunc(display7);
		glutIdleFunc(idle);
		//etc.  Stop calling glutTimerFunc when no longer need the timer
		glutTimerFunc(8000, MyTimerFunc, 7);
	}
	else if (value == 7)
	{
		glutDisplayFunc(display8);
		glutIdleFunc(display8);
		glutIdleFunc(idle);
		//etc.  Stop calling glutTimerFunc when no longer need the timer
		glutTimerFunc(8000, MyTimerFunc, 8);
	}
	else if (value == 8)
	{
		glutDisplayFunc(display9);
		glutIdleFunc(display8);
		glutIdleFunc(idle);
		//etc.  Stop calling glutTimerFunc when no longer need the timer
		glutTimerFunc(3000, MyTimerFunc, 9);
	}
	else if (value == 9)
	{
		glutDisplayFunc(display10);
		glutIdleFunc(display8);
		glutIdleFunc(idle);
		//etc.  Stop calling glutTimerFunc when no longer need the timer
		glutTimerFunc(3000, MyTimerFunc, 10);
	}
	else if (value == 10)
	{
		glutDisplayFunc(display11);
		glutIdleFunc(display11);
		glutIdleFunc(idle);
		//etc.  Stop calling glutTimerFunc when no longer need the timer
		glutTimerFunc(3000, MyTimerFunc, 11);
	}
	else if (value == 11)
	{
		glutDisplayFunc(display12);
		glutIdleFunc(display12);
		glutIdleFunc(idle);
		//etc.  Stop calling glutTimerFunc when no longer need the timer
		glutTimerFunc(3000, MyTimerFunc, 12);
	}
	else if (value == 12)
	{
		glutDisplayFunc(display12);
		glutIdleFunc(display12);
		glutIdleFunc(idle);
		//etc.  Stop calling glutTimerFunc when no longer need the timer
		glutTimerFunc(3000, MyTimerFunc, 12);
	}
}
