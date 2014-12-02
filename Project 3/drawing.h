#ifndef _DRAWING_H_
#define _DRAWING_H_

/* Globals declared in drawing.cpp */
extern int subdiv_v; // The user-specified subdivision level, vertical
extern int subdiv_h; // The user-specified subdivision level, horizontal

struct point {
    double x;
    double y;
    double z;
};

/* Functions implemented in drawing.cpp */
void drawSurface(void);
void draw3DModel(void);
void setup3DModel(void);
void subdivideVertical(void);
void subdivideHorizontal(bool inc);
bool drawing3D(void);
void toggle3D(void);
void toggleDisplayMode(void);
void toggleControlPoints(void);
void toggleShadingMode(void);
bool addPoint(int x, int y);
void removeAllPoints(void);
bool removeLastPoint(void);
bool removePoint(int index);

#endif	/* _DRAWING_H_ */
