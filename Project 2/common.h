#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __MAC__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "readppm.h"

using namespace std;

GLfloat *transpose(int dimension, const GLfloat *matrix);
void printMatrix(int dimension, const char *message, const GLfloat *matrix);
GLfloat *matrixVectorMult(int dimension, const GLfloat *matrix, const GLfloat *vector);
GLfloat *matrixMatrixMult(int dimension, const GLfloat *matrixA, const GLfloat *matrixB);
void rotateBy(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void translateBy(GLfloat x, GLfloat y, GLfloat z);
void scaleBy(GLfloat x, GLfloat y, GLfloat z);
void loadIdentity(void);
void pushMatrix();
void popMatrix();
GLfloat *getCurrentMatrix();
void load2DMatrix(
		GLfloat m00, GLfloat m01, GLfloat m02,
		GLfloat m10, GLfloat m11, GLfloat m12,
		GLfloat m20, GLfloat m21, GLfloat m22) ;
void load3DMatrix(
		GLfloat m00, GLfloat m01, GLfloat m02, GLfloat m03,
		GLfloat m10, GLfloat m11, GLfloat m12, GLfloat m13,
		GLfloat m20, GLfloat m21, GLfloat m22, GLfloat m23,
		GLfloat m30, GLfloat m31, GLfloat m32, GLfloat m33);
void load3DMatrix(const GLfloat *matrix);

#endif
