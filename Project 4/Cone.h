#ifndef _CONE_H_
#define _CONE_H_

#include "Polygon.h"

class Cone: public Polygon {
private:
	Point *center;
	GLfloat height;
	GLfloat radius;

public:
	Cone(Material *material, Point *center, GLfloat radius);

	bool rayConeIntersect(ray *r, double *t);
	void findConeNormal(Point *p, vector *n);
};

#endif
