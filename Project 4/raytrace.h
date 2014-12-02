#ifndef _RAYTRACE_H_
#define _RAYTRACE_H_

/******************************************************************/
/*         Raytracer declarations                                 */
/******************************************************************/

#include "Sphere.h"

/* constants */
#define TRUE 1
#define FALSE 0

#define PI 3.14159265358979323846264338327

/* functions in raytrace.cpp */
void traceRay(ray *r, Color *color, int);
Point *getViewPoint();
void firstHit(ray*,Point*,vector*,Material**);

/* global variables */
extern int width;
extern int height;

#endif	/* _RAYTRACE_H_ */
