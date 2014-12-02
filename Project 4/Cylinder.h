/* ****************************************************************************
*  File name: Cylinder.h
*  Project: 4 - Raytracing
*  Author: Matt Schwartz
*************************************************************************** */

#ifndef _CYLINDER_H_
#define _CYLINDER_H_

#include "Polygon.h"

class Cylinder: public Polygon {
private:
	Point *center;
	GLfloat radius;
	GLfloat height;

public:
	Cylinder(Material *material, Point *center, GLfloat radius, GLfloat height);
	Point *getCenter();
	GLfloat getRadius();
	GLfloat getHeight();

	bool rayCylinderIntersect(ray *r, double *t);
	void findCylinderNormal(Point *p, vector *n);
};

#endif
