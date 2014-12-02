/* ****************************************************************************
*  File name: Sphere.cpp
*  Project: 4 - Raytracing
*  Author: Matt Schwartz
*************************************************************************** */

#include "Sphere.h"

Sphere::Sphere(Material *material, Point *center, GLfloat radius) {
    this->material = material;
    this->center = center;
    this->radius = radius;
} // constructr

GLfloat Sphere::getRadius() {
	return radius;
}

void Sphere::setTextureMap(GLubyte *pixels) {
    texMap = pixels;
} // setTextureMap

Color *Sphere::getMappedColorAt(Point *point) {
    
    return NULL;
} // getMappedColorAt

/* Returns true if the ray interesects some point on the Sphere */
bool Sphere::raySphereIntersect(ray *r, double *t) {
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
        *t = (-b - D) / (2 * a);
        /* ignore roots which are less than zero (behind viewpoint) */
        if (*t < 0) {
            *t = (-b + D) / (2 * a);
        }
        if (*t < 0) {
            return (false);
        } else return (true);
    }
} // raySphereIntersect

/* normal vector of s at p is returned in n */

/* note: dividing by radius normalizes */
void Sphere::findSphereNormal(Point* p, vector* n) {
    n->x = (p->x - center->x) / radius;
    n->y = (p->y - center->y) / radius;
    n->z = (p->z - center->z) / radius;
    n->w = 0.0;
} // findSphereNormal
