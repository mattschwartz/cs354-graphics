#include "Cone.h"

Cone::Cone(Material *material, Point *center, GLfloat radius) {
	this->material = material;
	this->center = center;
	this->radius = radius;
} // constructor

bool Cone::rayConeIntersect(ray *r, double *t) {
    Point p; /* start of transformed ray */
    double a, b, c; /* coefficients of quadratic equation */
    double D; /* discriminant */
    Point* v;

    /* transform ray so that Sphere center is at origin */
    /* don't use matrix, just translate! */
    p.x = r->start->x - center->x;
    p.y = r->start->y - center->y;
    p.z = r->start->z - center->z;
    v = r->dir; /* point to direction vector */

    a = v->x * v->x + v->y * v->y + v->z * v->z;
    b = 2 * (v->x * p.x + v->y * p.y + v->z * p.z);
    c = p.x * p.x + p.y * p.y + p.z * p.z - radius * radius;

    D = b * b - 4 * a * c;

    if (D < 0) { /* no intersection */
        return (false);
    } else {
        D = sqrt(D);
        /* First check the root with the lower value of t: */
        /* this one, since D is positive */
        *t = (-b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
        /* ignore roots which are less than zero (behind viewpoint) */
        if (*t < 0) {
            *t = (-b + D) / (2 * a);
        }
        if (*t < 0) {
            return (false);
        } else return (true);
    }
} // rayConeIntersect

void Cone::findConeNormal(Point *p, vector *n) {
} // findConeNormal