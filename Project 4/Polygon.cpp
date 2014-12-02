/* ****************************************************************************
*  File name: Polygon.cpp
*  Project: 4 - Raytracing
*  Author: Matt Schwartz
*************************************************************************** */

#ifdef _WIN32
#include <windows.h>
#endif

#include "Polygon.h"

Color *Polygon::getColor() {
    return color;
} // getColor

Material *Polygon::getMaterial() {
    return material;
} // getMaterial

Color *Polygon::makeColor(GLfloat r, GLfloat g, GLfloat b) {
    Color *co;
    co = (Color *) malloc(sizeof (Color));

    co->r = r;
    co->g = g;
    co->b = b;

    return co;
} // makeColor

Point *Polygon::makePoint(GLfloat x, GLfloat y, GLfloat z) {
    Point* p;
    /* allocate memory */
    p = (Point*) malloc(sizeof (Point));
    /* put stuff in it */
    p->x = x;
    p->y = y;
    p->z = z;
    p->w = 1.0;
    return (p);
} // makePoint

Material* Polygon::makeMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat amb, GLfloat diffuse, GLfloat specular, GLint shininess) {
    Material* m;

    /* allocate memory */
    m = (Material*) malloc(sizeof (Material));
    /* put stuff in it */
    m->r = r;
    m->g = g;
    m->b = b;
    m->ambient = amb;
    m->diffuse = diffuse;
    m->specular = specular;
    m->shininess = shininess;
    return (m);
}

Point *Polygon::copyPoint(Point *p0) {
    Point* p;
    /* allocate memory */
    p = (Point*) malloc(sizeof (Point));

    p->x = p0->x;
    p->y = p0->y;
    p->z = p0->z;
    p->w = p0->w; /* copies over vector or point status */
    return (p);
} // copyPoint

void Polygon::freePoint(Point *p) {
    if (p != NULL) {
        free(p);
    }
} // freePoint

void Polygon::calculateDirection(Point *p, Point *q, Point *v) {
    v->x = q->x - p->x;
    v->y = q->y - p->y;
    v->z = q->z - p->z;
    /* a direction is a point at infinity */
    v->w = 0.0;

    /* NOTE: v is not unit length currently, but probably should be */
} // calculateDirection

void Polygon::normalize(vector *v) {
    /* PUT YOUR CODE HERE */
    GLfloat magnitude;

    magnitude = sqrt(pow(v->x, 2) + pow(v->y, 2) + pow(v->z, 2));

    v->x /= magnitude;
    v->y /= magnitude;
    v->z /= magnitude;
} // normalize

// performs the dot product between vectors a and b

GLfloat Polygon::dot(vector a, vector b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
} // dot

void Polygon::findPointOnRay(ray *r, double t, Point *p) {
    p->x = r->start->x + t * r->dir->x;
    p->y = r->start->y + t * r->dir->y;
    p->z = r->start->z + t * r->dir->z;
    p->w = 1.0;
} // findPointOnRay
