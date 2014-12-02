/* ****************************************************************************
*  File name: Polygon.h
*  Project: 4 - Raytracing
*  Author: Matt Schwartz
*************************************************************************** */

#ifndef _Polygon_H_
#define _Polygon_H_

#include "common.h"
#include <math.h>

typedef struct Point {
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;
} Point;

/* a vector is just a point */
typedef Point vector;

/* a ray is a start point and a direction */
typedef struct ray {
	Point* start;
	vector* dir;
} ray;

typedef struct Material {
	/* color */
	GLfloat r;
	GLfloat g;
	GLfloat b; 

	/* ambient reflectivity */
	GLfloat ambient;
	GLfloat diffuse;
	GLfloat specular; // surface's coefficient of specular reflection
	GLint shininess;
} Material;

typedef struct Color {
	GLfloat r;
	GLfloat g;
	GLfloat b; 
	/* these should be between 0 and 1 */
} Color;

class Polygon {
protected:
	// subclasses share these members
	Material *material;
	Color *color;

public:
	// accessor
	Color *getColor();
	Material *getMaterial();

	// generic Polygon operations
	static Color *makeColor(GLfloat r, GLfloat g, GLfloat b);
	static Point *makePoint(GLfloat x, GLfloat y, GLfloat z);
	static Material* makeMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat amb, 
				GLfloat diffuse, GLfloat specular, GLint shininess);
	static Point *copyPoint(Point *p0);
	static void freePoint(Point *p);
	static void calculateDirection(Point *p, Point *q, Point *v);
	static void normalize(vector *v);
	static GLfloat dot(vector a, vector b);
	static void findPointOnRay(ray *r, double t, Point *p);
};

#endif
