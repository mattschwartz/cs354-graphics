/*
 * drawing.cpp
 * -----------
 * Contains the drawing routines and related helper functions for the
 * subdivision surface
 *
 * Group Members: Matt Schwartz
 */

#ifdef _WIN32
#include <windows.h>
#endif
#include <cmath>
#include <assert.h>
#include <string.h>

#include "common.h"
#include "drawing.h"
#include "data.h"

/* Globals */
int subdiv_v = 0; // The user-specified subdivision level, vertical
int subdiv_h = 0; // The user-specified subdivision level, horizontal

int numControlPoints = 0; // current number of points
int numSub_vPoints = 0;
int numSub_hPoints = 0;
int displayMode = SOLID_FRAME;
int shadingMode = GOURAUD_SHADING;

bool draw3D = false;
bool showControlPoints = false;
bool setup3D = false;

float pointColor[] = {1.0f, 1.0f, 1.0f}; 
float lineColor[] = {0.05f, 0.25f, 1.0f};
float axisColor[] = {0.75f, 0.15f, 0.20f};

struct point controlPoints[MAX_CONTROL_POINTS + (MAX_CONTROL_POINTS - 1)];
struct point subdividableControlPoints[MAX_SUBDIVIDED_CONTROL_POINTS];
struct point threeDModelVertices[MAX_3D_MODEL_POINTS];
struct point sub_hVertices[MAX_3D_MODEL_POINTS * MAX_3D_MODEL_POINTS];

void drawAxis(void);
void drawControlPoints(void);
struct point crossAndNormal(struct point vec1, struct point vec2);
void fillVertices(void);
void extrapolateEndPoints(void);
struct point reflect(struct point p1, struct point p2);
void printPoint(struct point p);

/**
 * Draws the 2D sketch
 */
void drawSurface(void) {
    drawAxis();
    drawControlPoints();
    
    // Draw a line connecting the control points in a shade of blue
    glColor3fv(lineColor);
    glBegin(GL_LINES);
    for (int i = 0; i < numControlPoints; i++) {
        if (i > 0) {
            glVertex3f(controlPoints[i - 1].x, controlPoints[i - 1].y, 0);
        } // if
        
        else {
            glVertex3f(controlPoints[i].x, controlPoints[i].y, 0);
        } // else
        
        glVertex3f(controlPoints[i].x, controlPoints[i].y, 0);
    } // for
    glEnd();
    
} // drawSurface

/**
 * Subdivide the user-supplied control points.
 */
void subdivideVertical(void) {
    struct point tempPointArray[numSub_vPoints + (numSub_vPoints - 1)];
    
    if (!draw3D) {
        return;
    } // if
    
    subdiv_v++;
    
    if (subdiv_v > 6) {
        cout << "Cannot apply vertical subdivision: reached max depth of 6." << endl;
        subdiv_v = 6;
        return;
    } // if
    
    cout << "Applying vertical subdivision: depth of " << subdiv_v << endl;
    
    // Even rule
    // updated point = 1/8 * (p(i-1) + 6*p(i) + p(i+1))
    for (int i = 1; i < numSub_vPoints - 1; i++) {
        tempPointArray[2*i].x = (subdividableControlPoints[i - 1].x / 8) + 
                (3*subdividableControlPoints[i].x / 4) + 
                (subdividableControlPoints[i + 1].x / 8);
        tempPointArray[2*i].y = (subdividableControlPoints[i - 1].y / 8) + 
                (3*subdividableControlPoints[i].y / 4) + 
                (subdividableControlPoints[i + 1].y / 8);
        tempPointArray[2*i].z = 0;
    } // for
    
    // Odd rule
    // new point = 1/8 * (4*p(i) + 4*p(i+1))
    for (int i = 0; i < numSub_vPoints - 1; i++) {
        tempPointArray[2*i + 1].x = (subdividableControlPoints[i].x / 2) + 
                (subdividableControlPoints[i + 1].x / 2);
        tempPointArray[2*i + 1].y = (subdividableControlPoints[i].y / 2) + 
                (subdividableControlPoints[i + 1].y / 2);
        tempPointArray[2*i + 1].z = 0;
    } // for
    
    numSub_vPoints = numSub_vPoints + (numSub_vPoints - 1);
    
    for (int i = 1; i < numSub_vPoints-1; i++) {
        subdividableControlPoints[i].x = tempPointArray[i].x;
        subdividableControlPoints[i].y = tempPointArray[i].y;
        subdividableControlPoints[i].z = 0;
    } // for
    
    extrapolateEndPoints();
    
    // Add vertices to the drawable 3D model
    fillVertices();
} // subdivideVertical

/**
 * Subdivide the 3D model horizontally.  Considers each control point as a 
 * closed triangle-loop of control points, made from rotating the point by
 * 120 degrees twice.
 * 
 * The subdivision works, although the end points are off, so the end result is
 * peculiar and has a very sharp, tapered end on one side and a smooth end on 
 * the other.
 */
void subdivideHorizontal(bool inc) {
    struct point subdivTemp[6000];
    struct point startPoint;
    struct point midPoint;
    struct point endPoint;
    
    if (!draw3D) {
        return;
    } // if
    
    if (inc) {
        subdiv_h++;
    } // if
    
    fillVertices();
    
    if (subdiv_h > 6) {
        cout << "Cannot apply horizontal subdivision: reached max depth of 6." << endl;
        subdiv_h = 6;
        return;
    } // if
    
    if (inc) {
        cout << "Applying horizontal subdivision: depth of " << subdiv_h << endl;
    } // if
    
    for (int i = 0; i < numSub_vPoints; i++) {
        startPoint = threeDModelVertices[i];
        midPoint = threeDModelVertices[numSub_vPoints + i];
        endPoint = threeDModelVertices[numSub_vPoints * 2 + i];
        
        subdivTemp[0] = startPoint;
        subdivTemp[1] = midPoint;
        subdivTemp[2] = endPoint;
        subdivTemp[3] = startPoint;
        
        numSub_hPoints = 4;
        
        // Fill temp array 
        for (int j = 0; j < subdiv_h; j++) {
            struct point temp[numSub_hPoints + (numSub_hPoints - 1)];
            // Even rule
            // updated point = 1/8 * (p(i-1) + 6*p(i) + p(i+1))
            for (int k = 1; k < numSub_hPoints - 1; k++) {
                temp[2*k].x = (subdivTemp[k - 1].x / 8) + (3*subdivTemp[k].x / 4) + (subdivTemp[k + 1].x / 8);
                temp[2*k].y = subdivTemp[k].y;
                temp[2*k].z = (subdivTemp[k - 1].z / 8) + (3*subdivTemp[k].z / 4) + (subdivTemp[k + 1].z / 8);
            } // for
    
            // Odd rule
            // new point = 1/8 * (4*p(i) + 4*p(i+1))
            for (int k = 0; k < numSub_hPoints - 1; k++) {
                temp[2*k + 1].x = (subdivTemp[k].x / 2) + (subdivTemp[k + 1].x / 2);
                temp[2*k + 1].y = subdivTemp[k].y;
                temp[2*k + 1].z = (subdivTemp[k].z / 2) + (subdivTemp[k + 1].z / 2);
            } // for
    
            numSub_hPoints = numSub_hPoints + (numSub_hPoints - 1);
            for (int k = 0; k < numSub_hPoints - 1; k++) {
                subdivTemp[k].x = temp[k].x;
                subdivTemp[k].y = temp[k].y;
                subdivTemp[k].z = temp[k].z;
            } // for
            
            // close the loop
            subdivTemp[0] = startPoint;
            subdivTemp[numSub_hPoints - 1] = startPoint;
        } // for
        
        // Fill array with new points for the current control point
        for (int k = 0; k < numSub_hPoints; k++) {
            sub_hVertices[k + i * numSub_hPoints].x = subdivTemp[k].x;
            sub_hVertices[k + i * numSub_hPoints].y = subdivTemp[k].y;
            sub_hVertices[k + i * numSub_hPoints].z = subdivTemp[k].z;
        } // for
        
    } // for
} // subdivideHorizontal

/**
 * 
 * @return true if the object model is being drawn or the 2D sketch
 */
bool drawing3D(void) {
    return draw3D;
} // drawing3D

/**
 * Called when the user has finished entering in his or her control points and
 * wishes to "rotate the silhouette".  Will only be called once per sketch.
 * Repeated calls to this function will do nothing.  At least 5 control
 * points must be entered before the user can view the 3D model.
 */
void setup3DModel(void) {
    if (setup3D) {
        return;
    } // if
    
    toggle3D();
    
    if (!draw3D) {
        return;
    } // if
    
    setup3D = true;
    
    cout << "Rotating silhouette." << endl;
    // Copy control point data into subdividable array
    for (int i = 0; i < numControlPoints; i++) {
        subdividableControlPoints[i].x = controlPoints[i].x;
        subdividableControlPoints[i].y = controlPoints[i].y;
        subdividableControlPoints[i].z = controlPoints[i].z;
    } // for
    
    numSub_vPoints = numControlPoints;
    numSub_hPoints = 3;
    
    // Adjust end points to fall on axis of rotation
    extrapolateEndPoints();
    
    // Add vertices to the drawable 3D model
    fillVertices();
} // setup3DModel

/**
 * Allows the user to switch between the model and the sketch he or she entered
 * that produced the model.  The user cannot enter or remove control points in
 * sketch mode after a 3D model has already been drawn.
 */
void toggle3D(void) {
    if (numControlPoints < 5) {
        cout << "Too few control points.  Please enter " << 
                (5 - numControlPoints) << " more." << endl;
        draw3D = false;
        return;
    } // if
    
    draw3D = !draw3D;
    
    if (draw3D) {
        // Copy control point data into subdividable array
        for (int i = 0; i < numControlPoints; i++) {
            subdividableControlPoints[i].x = controlPoints[i].x;
            subdividableControlPoints[i].y = controlPoints[i].y;
            subdividableControlPoints[i].z = controlPoints[i].z;
        } // for

        numSub_vPoints = numControlPoints;
        numSub_hPoints = 3;

        // Adjust end points to fall on axis of rotation
        extrapolateEndPoints();

        // Add vertices to the drawable 3D model
        fillVertices();
        cout << "Drawing 3D model." << endl;
    } // if
    
    else {
        numSub_vPoints = 0;
        displayMode = SOLID_FRAME;
        shadingMode = GOURAUD_SHADING;
        subdiv_v = 0;
        subdiv_h = 0;
        draw3D = false;
        showControlPoints = false;
        setup3D = false;
        cout << "Entering 2D sketch mode." << endl;
    } // else
} // create3D

/**
 * Internal function that draws the axis of rotation for the user to guide his
 * or her control points about.
 */
void drawAxis(void) {
    glColor3fv(axisColor);
    glBegin(GL_LINES);
    glVertex3f(0, -400, 0);
    glVertex3f(0, 400, 0);
    glEnd();
} // drawAxis

/**
 * Draws the 3D model to the screen, including the subdivision.  Determines if 
 * the user wanted a wireframe or solid model to be drawn as well as whether or
 * not the user wanted to view control points (only in wireframe mode).
 * 
 * Limitations: vertical and horizontal subdivision are exclusive.
 */
void draw3DModel(void) {
    int drawMode = GL_LINE_LOOP;
    int index;
    int index2;
        
    glDisable(GL_LIGHTING);
    subdivideHorizontal(false);
        
    if (showControlPoints) {
        drawControlPoints();
        return;
    } // if
    
    glColor3fv(lineColor);
    
    if (displayMode == SOLID_FRAME) {
        glEnable(GL_LIGHTING);
        drawMode = GL_POLYGON;
    } // if
    
    else if (displayMode == WIRE_FRAME) {
        drawMode = GL_LINE_LOOP;
    } // else if
    
    else {
        drawMode = GL_LINE_LOOP;
    } // else
    
//    if (subdiv_h > 0) {
        for (int i = 0; i < numSub_hPoints - 1; i++) {
            for (int j = 0; j < numSub_vPoints; j++) {
                index = (j % numSub_vPoints) * numSub_hPoints + i;
                index2 = ((j + 1) % (numSub_vPoints)) * numSub_hPoints + i;
                
                struct point bot_right = sub_hVertices[index];
                struct point bot_left = sub_hVertices[index + 1];
                
                struct point top_right = sub_hVertices[index2];
                struct point top_left = sub_hVertices[index2 + 1];
                
                glBegin(drawMode);
                
//                if (shadingMode == PHONG_SHADING) {
                    struct point normal = crossAndNormal(bot_right, top_right);

                    glNormal3f(normal.x, normal.y, normal.z);
//                } // if
//                
//                else {
//                    glNormal3f(0.0, 0.0, 1.0);
//                } // else
                
                glVertex3f(bot_right.x, bot_right.y, bot_right.z);
                glVertex3f(top_right.x, top_right.y, top_right.z);
                
                glVertex3f(top_left.x, top_left.y, top_left.z);
                glVertex3f(bot_left.x, bot_left.y, bot_left.z);
                glEnd();
            } // for
        } // for
//    } // if
    
//    else {
//        for (int j = 0; j < 3; j++) {
//            for (int i = 0; i < numSub_vPoints - 1; i++) {
//                index = i + (j % 3) * numSub_vPoints;
//                index2 = i + ((j + 1) % 3) * numSub_vPoints;
//                
//                glBegin(drawMode);
//                
//                if (shadingMode == PHONG_SHADING) {
//                    struct point normal = crossAndNormal(threeDModelVertices[index], threeDModelVertices[index + 1]);
//
//                    glNormal3f(normal.x, normal.y, normal.z);
//                } // if
//                
//                else {
//                    glNormal3f(0.0, 0.0, 1.0);
//                } // else
//                
//                glVertex3f(threeDModelVertices[index].x, 
//                        threeDModelVertices[index].y, threeDModelVertices[index].z);
//                glVertex3f(threeDModelVertices[index + 1].x, 
//                        threeDModelVertices[index + 1].y, threeDModelVertices[index + 1].z);
//
//                glVertex3f(threeDModelVertices[index2 + 1].x, 
//                        threeDModelVertices[index2 + 1].y, threeDModelVertices[index2 + 1].z);
//                glVertex3f(threeDModelVertices[index2].x, 
//                        threeDModelVertices[index2].y, threeDModelVertices[index2].z);
//                glEnd();
//            } // for
//        } // for
//    } // else
} // draw3DModel

struct point crossAndNormal(struct point vec1, struct point vec2) {
    struct point normal;
    double magnitude;
    
    normal.x = vec1.y * vec2.z - vec1.z * vec2.y;
    normal.y = vec1.z * vec2.x - vec1.x * vec2.z;
    normal.z = vec1.x * vec2.y - vec1.y * vec2.x;
    
    magnitude = sqrt( (normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z) );
    
    normal.x /= magnitude;
    normal.y /= magnitude;
    normal.z /= magnitude;
    
    return normal;
} // crossAndMultiply

/**
 * Draw control points in 2D or 3D, including all subdivided points.
 */
void drawControlPoints(void) {
    glPointSize(5);
    glColor3fv(pointColor);
    
    if (draw3D) {
//        if (subdiv_h > 0) {
            glBegin(GL_POINTS);
            
            for (int i = 0; i < numSub_vPoints; i++) {
                for (int j = 0; j < numSub_hPoints; j++) {
                    glVertex3f(sub_hVertices[j + i * numSub_hPoints].x, 
                            sub_hVertices[j + i * numSub_hPoints].y, 
                            sub_hVertices[j + i * numSub_hPoints].z);
                } // for
            } // for
            
            glEnd();
//        } // if
        
//        else {
//            glBegin(GL_POINTS);
//            
//            for (int i = 0; i < numSub_vPoints * 3; i++) {
//                glVertex3f(threeDModelVertices[i].x, threeDModelVertices[i].y, 
//                        threeDModelVertices[i].z);
//            } // for
//        
//            glEnd();
//        } // else
        
        
        return;
    } // if
    
    glBegin(GL_POINTS);
    
    for (int i = 0; i < numControlPoints; i++) {
        glVertex3f(controlPoints[i].x, controlPoints[i].y, 0);
    } // for
    
    glEnd();
} // drawControlPoints

/**
 * Rotates the given control points about the axis of rotation 120 degrees twice
 * and records the new locations which causes the 2D sketch to appear in 3D.
 */
void fillVertices(void) {
    double theta = 2.0944; // ≈120 degrees
    
    for (int i = 0; i < numSub_vPoints; i++) {
        threeDModelVertices[i].x = subdividableControlPoints[i].x;
        threeDModelVertices[i].y = subdividableControlPoints[i].y;
        threeDModelVertices[i].z = subdividableControlPoints[i].z;
    } // for
    
    // Compute control points 120 degrees about y axis from original points
    for (int i = numSub_vPoints, j = 0; i < (numSub_vPoints * 2); i++, j++) {
        threeDModelVertices[i].x = threeDModelVertices[j].x * cos(theta) + 
                threeDModelVertices[j].z * sin(theta);
        threeDModelVertices[i].y = threeDModelVertices[j].y;
        threeDModelVertices[i].z = threeDModelVertices[j].x * -sin(theta) + 
                threeDModelVertices[j].z * cos(theta);
    } // for
    
    // Compute control points -120 degrees about y axis from original points
    for (int i = (numSub_vPoints * 2), j = 0; i < (numSub_vPoints * 3); i++, j++) {
        threeDModelVertices[i].x = threeDModelVertices[j].x * cos(-theta) + 
                threeDModelVertices[j].z * sin(-theta);
        threeDModelVertices[i].y = threeDModelVertices[j].y;
        threeDModelVertices[i].z = threeDModelVertices[j].x * -sin(-theta) + 
                threeDModelVertices[j].z * cos(-theta);
    } // for
} // fillVertices

/**
 * Estimates where the end points of the given control points would be if the
 * user had put them exactly on the axis of rotation.
 */
void extrapolateEndPoints(void) {
    struct point startPoint = {0, 0, 0};
    struct point endPoint = {0, 0, 0};
    
    startPoint = reflect(controlPoints[0], controlPoints[1]);
    endPoint = reflect(controlPoints[numControlPoints - 1], controlPoints[numControlPoints - 2]);
    
    startPoint.x = 0;
    endPoint.x = 0;
    
    subdividableControlPoints[0] = startPoint;
    subdividableControlPoints[numSub_vPoints - 1] = endPoint;
} // extrapolateEndPoints

/**
 * 
 * @param p a point
 * @return true if @p exists as one of the user-supplied control points
 */
bool pointExists(struct point p) {
    for (int i = 0; i <= numControlPoints; i++) {
        if (controlPoints[i].x == p.x && controlPoints[i].y == p.y) {
            return true;
        } // if
    } // for
    
    return false;
} // pointExists

/**
 * Toggle between solid and wire frame display modes
 */
void toggleDisplayMode(void) {
    if (!draw3D) {
        return;
    } // if
    
    cout << "Setting display mode to ";

    displayMode++;

    if (displayMode > WIRE_FRAME) {
        displayMode = SOLID_FRAME;
    } // if

    if (displayMode == SOLID_FRAME) {
        cout << "solid";
    } // if

    else if (displayMode == WIRE_FRAME) {
        cout << "wireframe";
    } // else if
    
    cout << "." << endl;
    
    showControlPoints = false;
} // toggleDisplayMode

/**
 * Toggle between displaying control points and hiding them
 */
void toggleControlPoints(void) {
    if (!draw3D) {
        return;
    } // if
    
    showControlPoints = !showControlPoints;
    
    if (showControlPoints) {
        cout << "Displaying control points." << endl;
    } // if
    
    else {
        cout << "Not displaying control points." << endl;
    } // else
} // setShowControlPoints

/**
 * Toggle between Gouraud and Phong shading methods
 */
void toggleShadingMode(void) {
    if (!draw3D) {
        return;
    } // if
    
    cout << "Changing shading mode to ";

    shadingMode++;

    if (shadingMode > PHONG_SHADING) {
        shadingMode = GOURAUD_SHADING;
    } // if

    if (shadingMode == GOURAUD_SHADING) {
        cout << "Gouraud";
    } // if

    else if (shadingMode == PHONG_SHADING) {
        cout << "Phong";
    } // else if

    cout << " shading." << endl;
} // toggleShadingMode

bool addPoint(int x, int y) {
    struct point p = {x, y};
    
    if (pointExists(p)) {
        cout << "You can't enter a point on top of another point." << endl;
        return false;
    } // if
    
    if ( (numControlPoints + 1) > 30) {
        cout << "You have entered in too many points already (" << 
                numControlPoints << ")." << endl;
        return false;
    } // if
    
    if (abs(p.x) <= 5) {
        // snap!
        p.x = 0;
    } // if
    
    cout << "Adding point #" << numControlPoints << ": [" << p.x << ", " 
            << p.y << "]." << endl;
    
    controlPoints[numControlPoints++] = p;
    
    return true;
} // addPoint

/**
 * Removes all points from the 2D sketch and allows the user to re-enter another
 * sketch without having to restart the program.
 */
void removeAllPoints(void) {
    numControlPoints = 0;
    numSub_vPoints = 0;
    displayMode = SOLID_FRAME;
    shadingMode = GOURAUD_SHADING;
    subdiv_v = 0;
    subdiv_h = 0;
    draw3D = false;
    showControlPoints = false;
    setup3D = false;
} // removeAllPoints

/**
 * Takes away the last point entered by the user.
 * @return true if a point was removed and false if no points exist
 */
bool removeLastPoint(void) {
    if (numControlPoints <= 0) {
        cout << "No point to remove." << endl;
        return false;
    } // if
    
    if (numControlPoints <= 5) {
        draw3D = false;
    } // if
    
    return removePoint(numControlPoints);
} // removeLastPoint

/**
 * Not used outside of drawing.cpp
 * @param index the index of the control point to remove
 * @return true if a point was removed
 */
bool removePoint(int index) {
    if (index > numControlPoints) {
        return false;
    } // if
    
    for (int i = index; i < numControlPoints - 1; i++) {
        controlPoints[i] = controlPoints[i + 1];
    } // for
    
    cout << "Removing point #" << numControlPoints - 1 << ": " << 
            controlPoints[numControlPoints-1].x << ", " << 
            controlPoints[numControlPoints-1].y << "]." << endl;
    
    numControlPoints--;
    return true;
} // removePoint

/**
 * Reflects a point based on 2 supplied points, needed to calculate slope and
 * position.  Used for vertical subdivision
 * @param p1 a point
 * @param p2 another point
 * @return the reflected point
 */
struct point reflect(struct point p1, struct point p2) {
    double slope;
    double b;
    double deltaX;
    struct point reflectedPoint;
    
    slope = (p2.y - p1.y) / (p2.x - p1.x);
    b = p1.y - slope * p1.x;
    deltaX = p2.x - p1.x;
    
    reflectedPoint.x = p1.x - deltaX;
    reflectedPoint.y = reflectedPoint.x * slope + b;
    reflectedPoint.z = 0;
    
    return reflectedPoint;
} // reflect

/**
 * Used for debugging.  Prints out @p to std out
 * @param p the point to print
 */
void printPoint(struct point p) {
    cout << "[" << p.x << ", " << p.y << ", " << p.z << "]" << endl; 
} // printPoint

/* end of drawing.cpp */
