/* ****************************************************************************
*  File name: light.h
*  Project: 4 - Raytracing
*  Author: Matt Schwartz
 *************************************************************************** */

#ifndef _LIGHT_H_
#define	_LIGHT_H_

#include "Polygon.h"

class Light {
private:
	Color *color;
	Point *position;
	GLfloat specular;
	GLfloat diffuse;
	GLfloat intensity; // what is this even used for..

public:
	Light(Color *color, Point *position, GLfloat specular, GLfloat diffuse, GLfloat intensity);
	static void getReflectVector(Point *point, vector normal, vector *reflectedVector);
	static void calculateReflectVector(vector normal, vector light, vector *relectVector);
	Color *getColor();
	Point *getPosition();
	GLfloat getSpecular();
	GLfloat getDiffuse();
	GLfloat getIntensity();
	static void shade(Point*,vector*,Material*,vector*,Color*,int);
};

void addLightSource(Light *light);

#endif	/* LIGHT_H */
