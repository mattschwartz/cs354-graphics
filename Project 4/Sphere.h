/* ****************************************************************************
*  File name: Sphere.h
*  Project: 4 - Raytracing
*  Author: Matt Schwartz
*************************************************************************** */

#ifndef _Sphere_H_
#define _Sphere_H_

#include "Polygon.h"

class Sphere : public Polygon {
private:
	Point* center; /* center */
	GLfloat radius; /* radius */
	GLubyte *texMap; // texture map

public:
	// constructors
	Sphere(Material *material, Point *center, GLfloat radius);
	GLfloat getRadius();
	void setTextureMap(GLubyte *pixels);
	Color *getMappedColorAt(Point *point);
	bool raySphereIntersect(ray *r, double *t);
	void findSphereNormal(Point* p, vector* n);
};

#endif
