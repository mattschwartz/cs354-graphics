/* ****************************************************************************
*  File name: Cylinder.h
*  Project: 4 - Raytracing
*  Author: Matt Schwartz
*************************************************************************** */

#include "Cylinder.h"

Cylinder::Cylinder(Material *material, Point *center, GLfloat radius, GLfloat height) {
    this->material = material;
    this->center = center;
    this->radius = radius;
    this->height = height;
} // constructor

Point *Cylinder::getCenter() {
    return center;
} // getCenter

GLfloat Cylinder::getRadius() {
    return radius;
} // getRadius

GLfloat Cylinder::getHeight() {
    return height;
} // getHeight

bool Cylinder::rayCylinderIntersect(ray *r, double *t) {
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

    a = v->x * v->x + v->z * v->z;
    b = 2 * (v->x * p.x + v->z * p.z);
    c = p.x * p.x + p.z * p.z - radius * radius;

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
} // rayCylinderIntersect

void Cylinder::findCylinderNormal(Point *p, vector *n) {
    n->x = (p->x - center->x) / radius;
    n->y = 0;//(p->y - center->y) / radius;
    n->z = (p->z - center->z) / radius;
    n->w = 0.0;
} // findCylinderNormal
