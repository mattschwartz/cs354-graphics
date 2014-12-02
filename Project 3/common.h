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

#include "iostream"

#define MAX_CONTROL_POINTS 30
#define MAX_SUBDIVIDED_CONTROL_POINTS MAX_CONTROL_POINTS * 64
#define MAX_3D_MODEL_POINTS MAX_SUBDIVIDED_CONTROL_POINTS * 3

using namespace std;

enum { 
    SOLID_FRAME = 0,
    WIRE_FRAME = 1,
    FACES_ONLY,
    CONTROL_POINTS,
    GOURAUD_SHADING,
    PHONG_SHADING,
    LOW_SHINY,
    HIGH_SHINY,
    THREE_D_MODE,
    TWO_D_MODE,
    
    // NYI
    AVERAGE_MODE,
    PROFILE_MODE
};

#endif
