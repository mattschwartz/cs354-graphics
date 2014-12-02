/******************************************************************/
/*         Main raytracer file                                    */
/*                                                                */
/* Group Members: Matt Schwartz                                   */
/******************************************************************/

#ifdef _WIN32
#include <windows.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "common.h"
#include "lowlevel.h"
#include "raytrace.h"
#include "Sphere.h"
#include "light.h"
#include "Cylinder.h"
#include "readppm.h"

#define EPSILON 0.001

using namespace std;

/* local functions */
void initScene(void);
void initCamera(int, int);
void display(void);
void keyHandler(unsigned char ch, int x, int y);
void init(int, int);
void traceRay(ray*, Color*);
void drawScene(void);

/* local data */

/* the scene: so far, just one Sphere */
Sphere *s1;
Sphere *s2;
Sphere *s3;
Sphere *s4;

Cylinder *c1;
Cylinder *c2;

bool doAA = true;
int aaPasses = 10;

/* the viewing parameters: */
Point* viewpoint;
GLfloat pnear; /* distance from viewpoint to image plane */
GLfloat fovx; /* x-angle of view frustum */
int width = 500; /* width of window in pixels */
int height = 350; /* height of window in pixels */

int depth = 0;

int main(int argc, char** argv) {
	int win;

	glutInit(&argc, argv);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	win = glutCreateWindow("raytrace");
	glutSetWindow(win);
	init(width, height);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyHandler);
	glutMainLoop();
	return 0;
}

Point *getViewPoint() {
	return viewpoint;
}

void keyHandler(unsigned char ch, int x, int y) {

	switch (ch) {

	case 'a':
		cout << "Toggle anti-aliasing.  ";
		doAA = !doAA;

		if (doAA) {
			cout << "This will take some time.";
		} // if

		else {
			cout << "AA is off.";
		} // else

		cout << endl;
		break;

	case 'x':
		aaPasses++;
		cout << "Increasing number of AA rays (" << aaPasses << ")." << endl;
		break;

	case 'X':
		aaPasses--;

		if (aaPasses < 0) {
			aaPasses = 0;
		} // if

		cout << "Decreasing number of AA rays (" << aaPasses << ")." << endl;
		break;

	case 'q':
		cout << "Terminating raytrace.cpp" << endl;
		exit(0);
		break;

	default:
		break;
	} // switch

	display();
} // keyHandler

void init(int w, int h) {

	/* OpenGL setup */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	/* low-level graphics setup */
	initCanvas(w, h);

	/* raytracer setup */
	initCamera(w, h);
	initScene();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawScene(); /* draws the picture in the canvas */
	flushCanvas(); /* draw the canvas to the OpenGL window */
	glFlush();
}

void initScene() {
	Light *light = new Light(Polygon::makeColor(1, 1, 1), Polygon::makePoint(15, -15, 4), 0.6, 1, 95);
	addLightSource(light);

	light = new Light(Polygon::makeColor(1, 1, 1), Polygon::makePoint(-10, 10, -15), 0.5, 1, 95);
	addLightSource(light);

	light = new Light(Polygon::makeColor(1, 1, 1), Polygon::makePoint(-10, 0, -10), 0.6, 0.657, 95);
	addLightSource(light);

	light = new Light(Polygon::makeColor(0, 0, 1), Polygon::makePoint(0, -15, -10), 0.6, 0.657, 95);
	addLightSource(light);

	light = new Light(Polygon::makeColor(1, 0, 1), Polygon::makePoint(-0.75, -0.5, 6), 0.15, 0.657, 95);
	addLightSource(light);

	light = new Light(Polygon::makeColor(1, 1, 1), Polygon::makePoint(-0.75, 0.5, 6), 0.15, 0.657, 95);
	addLightSource(light);

	s1 = new Sphere(Polygon::makeMaterial(1, 0, 0.5, 0.15, 0.75, 0.85, 100), Polygon::makePoint(0.25, 0.65, -3), 0.18);
	s2 = new Sphere(Polygon::makeMaterial(0.2, 0.85, 0.25, 0.15, 0.75, 0.85, 100), Polygon::makePoint(0, 0, -4), 0.25);
	s3 = new Sphere(Polygon::makeMaterial(0.67, 0.67, 0.67, 0.05, 1, 1, 255), Polygon::makePoint(0.35, 0.65, -5), 0.8);
	s4 = new Sphere(Polygon::makeMaterial(0.2, 0.85, 0.95, 0.05, 0.75, 0.85, 25), Polygon::makePoint(-1, -0.5, -5), 0.25);
	c1 = new Cylinder(Polygon::makeMaterial(0.25, 0.23, 0.84, 0.15, 0.75, 0.85, 25), Polygon::makePoint(0.95, 0, -7), 0.3, 2);
	c2 = new Cylinder(Polygon::makeMaterial(0.85, 0.73, 0.04, 0.15, 0.75, 0.85, 25), Polygon::makePoint(-0.7, 0, -10), 0.25, 2);
}

void initCamera(int w, int h) {
	viewpoint = Polygon::makePoint(0.0, 0.0, 0.0);
	pnear = 1.0;
	fovx = PI / 6;
}

void drawScene() {
	int i, j;
	GLfloat imageWidth;
	/* declare data structures on stack to avoid dynamic allocation */
	Point worldPix; /* current pixel in world coordinates */
	Point direction;
	ray r;
	Color c;

	/* initialize */
	worldPix.w = 1.0;
	worldPix.z = -pnear;
	r.start = &worldPix;
	r.dir = &direction;

	imageWidth = 2 * pnear * tan(fovx / 2);

	/* trace a ray for every pixel */
	for (i = 0; i < width; i++) {
		/* Refresh the display */
		/* Comment this line out after debugging */
		//flushCanvas();

		for (j = 0; j < height; j++) {

			/* find position of pixel in world coordinates */
			/* y position = (pixel height/middle) scaled to world coords */
			worldPix.y = (j - (height / 2)) * imageWidth / width;
			/* x position = (pixel width/middle) scaled to world coords */
			worldPix.x = (i - (width / 2)) * imageWidth / width;

			/* find direction */
			/* note: direction vector is NOT NORMALIZED */
			Polygon::calculateDirection(viewpoint, &worldPix, &direction);

			/* trace the ray! */
			c.r = 0;
			c.g = 0;
			c.b = 0;

			Color aaColor = c;
			Color leftAlias = c;
			Color color3 = c;
			Color color4 = c;

			traceRay(&r, &color3, 1);

			// anti-aliasing
			if (doAA) {
				ray aaRay = r;
				ray aaRay2 = r;
				ray aaRay3 = r;

				// fire rays all around original ray and average colors
				for (int i = 0; i < aaPasses; i++) {
					traceRay(&aaRay, &color3, 1);
					aaRay.start->x -= r.start->x * 0.01;

					traceRay(&aaRay2, &color4, 1);
					aaRay2.start->y += r.start->y * 0.01;

					aaColor.r += (color3.r + color4.r) / 2;
					aaColor.g += (color3.g + color4.g) / 2;
					aaColor.b += (color3.b + color4.b) / 2;

				} // for

				// take average of all rays
				c.r = aaColor.r / aaPasses;
				c.g = aaColor.g / aaPasses;
				c.b = aaColor.b / aaPasses;

				// clamp color values
				c.r > 1 ? c.r = 1 : c.r = c.r;
				c.g > 1 ? c.g = 1 : c.g = c.g;
				c.b > 1 ? c.b = 1 : c.b = c.b;
			} else
				c = color3;


			/* write the pixel! */
			drawPixel(i, j, c.r, c.g, c.b);
		}
	}
}

/* returns the color seen by ray r in parameter c */

/* d is the recursive depth */
void traceRay(ray* r, Color* c, int d) {
	Point p; /* first intersection point */
	vector n;
	ray *reflectedRay = (ray *) malloc(sizeof (ray *));
	vector rayDirection;
	Material* m;

	if (d < 0) {
		return;
	} // if

	p.w = 0.0; /* initialize to "no intersection" */
	firstHit(r, &p, &n, &m);

	if (p.w != 0.0) {
		Light::shade(&p, &n, m, r->dir, c, d); /* do the lighting calculations */

		reflectedRay->start = &p;
		Light::getReflectVector(reflectedRay->start, n, &rayDirection);

		reflectedRay->dir = &rayDirection;

		reflectedRay->start->x += EPSILON * reflectedRay->dir->x;
		reflectedRay->start->y += EPSILON * reflectedRay->dir->y;
		reflectedRay->start->z += EPSILON * reflectedRay->dir->z;
		traceRay(reflectedRay, c, --d);

	} // if
}

/* firstHit */

/* If something is hit, returns the finite intersection point p, 
the normal vector n to the surface at that point, and the surface
material m. If no hit, returns an infinite point (p->w = 0.0) */
void firstHit(ray* r, Point* p, vector* n, Material* *m) {
	double t = 0; /* parameter value at first hit */
	bool hit = false;


	hit = s1->raySphereIntersect(r, &t);

	if (hit) {
		*m = s1->getMaterial();
		Polygon::findPointOnRay(r, t, p);
		s1->findSphereNormal(p, n);
		return;
	} // if

	hit = s2->raySphereIntersect(r, &t);

	if (hit) {
		*m = s2->getMaterial();
		Polygon::findPointOnRay(r, t, p);
		s2->findSphereNormal(p, n);
		return;
	} // if

	hit = s3->raySphereIntersect(r, &t);

	if (hit) {
		*m = s3->getMaterial();
		Polygon::findPointOnRay(r, t, p);
		s3->findSphereNormal(p, n);
		return;
	} // if

	hit = s4->raySphereIntersect(r, &t);

	if (hit) {
		*m = s4->getMaterial();
		Polygon::findPointOnRay(r, t, p);
		s4->findSphereNormal(p, n);
		return;
	} // if

	hit = c1->rayCylinderIntersect(r, &t);

	if (hit) {
		*m = c1->getMaterial();
		Polygon::findPointOnRay(r, t, p);
		c1->findCylinderNormal(p, n);
		return;
	} // if

	hit = c2->rayCylinderIntersect(r, &t);

	if (hit) {
		*m = c2->getMaterial();
		Polygon::findPointOnRay(r, t, p);
		c2->findCylinderNormal(p, n);
		return;
	} // if

	p->w = 0.0;
} // firstHit
