/*
 * plant.cpp
 * ---------
 * Contains function callbacks and bookkeeping used by glut.  Starter code
 * for Project 2.
 *
 * Group Members: Matt Schwartz
 */

#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "common.h"
#include "drawplant.h"

enum {
    ORTHO_PROJECTION,
    PERSP_PROJECTION
};

/* GLOBAL VARAIBLES */
/* (storage is actually allocated here) */
int W=800;		/* window width */
int H=600;		/* window height */
int X_OFF = 10;	/* window x offset */
int Y_OFF = 10;	/* window y offset */

int depth = 1; // depth of the l system
int m_last_x = 0; // used for mouse control

int projectionMode = ORTHO_PROJECTION; // swap between two projections
int season = SUMMER, tree = SPRUCE;
int branchAngle = 25;

// switching projctions rotation fix
float cwRotate = -10, ccwRotate = 10;

/* local function declarations */
void display(void);
void init(void);
void setProjection(int projection);
void keyHandler(unsigned char ch, int x, int y);
void mouseHandler(int x, int y);

int main (int argc, char** argv) {
    seedRandomColor();
    glutInit(&argc,argv);
    glutInitWindowSize(W, H);
    glutInitWindowPosition(X_OFF, Y_OFF);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("plant");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyHandler);
    glutMotionFunc(mouseHandler);
    glutMainLoop();
    return 0;
} // main

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);  
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    loadIdentity();
    glOrtho(-40.0, 40.0, -40.0, 40.0, -100.0, 100.0);
}

void setProjection(int projection) {
    float fixScale = 0.025; // fixes projection scale
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    loadIdentity();
    
    if (projection == PERSP_PROJECTION) {
        scaleBy(fixScale, fixScale, fixScale);
        ccwRotate = -ccwRotate;
        cwRotate = -cwRotate;
        glFrustum(-40.0, 40.0, -40.0, 40.0, -100.0, 100.0);
    } else {
        ccwRotate = -ccwRotate;
        cwRotate = -cwRotate;
        glOrtho(-40.0, 40.0, -40.0, 40.0, -100.0, 100.0);
    } // if-else
    glMatrixMode(GL_MODELVIEW);
} // setProjection


void display() {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* See drawplant.c for the definition of this routine */
    drawTree(depth);

    glFlush();  /* Flush all executed OpenGL ops finish */

    /*
     * Since we are using double buffers, we need to call the swap
     * function every time we are done drawing.
     */
    glutSwapBuffers();
}

void keyHandler(unsigned char ch, int x, int y) {
	switch(ch) {
        case '/': 
            cout << "Rotating about y axis counter-clockwise" << endl;
            rotateBy(ccwRotate, 0, 1, 0);
            break;

        case '?': 
            cout << "Rotating about y axis clockwise" << endl;
            rotateBy(cwRotate, 0, 1, 0);
            break;

        case 'l' : 
            cout << "Shifting to the left" << endl;
            translateBy(-1, 0, 0);
            break;

        case 'r' : 
            cout << "Shifting to the right" << endl;
            translateBy(1, 0, 0);
            break;

        case 'u': 
            cout << "Shifting up" << endl;
            translateBy(0, 1, 0);
            break;
                
        case 'v':
            cout << "Changing season: ";
            season++;
            if (season > SPRING) {
                season = SUMMER;
            }
            setSeason(season);
            break;
            
        case 'V':
            cout << "Changing season: ";
            season--;
            if (season < SUMMER) {
                season = SPRING;
            }
            setSeason(season);
            break;
            
        case 't':
            cout << "Changing bark: ";
            tree++;
            if (tree > REDWOOD) {
                tree = SPRUCE;
            }
            setTree(tree);
            break;
            
        case 'T':
            cout << "Changing bark: ";
            tree--;
            if (tree < SPRUCE) {
                tree = REDWOOD;
            }
            setTree(tree);
            break;
                
        case '.':
            branchAngle += 5;
            
            if (branchAngle > 35) {
                branchAngle = 35;
            } // if
            
            cout << "Increasing branching angle: " << branchAngle << endl;
            setBranchAngle(branchAngle);
            break;
            
        case ',':
            branchAngle -= 5;
            
            if (branchAngle < 0) {
                branchAngle = 0;
            } // if
            
            cout << "Decreasing branching angle: " << branchAngle << endl;
            setBranchAngle(branchAngle);
            break;
            
        case 'd': 
            cout << "Shifting down" << endl;
            translateBy(0, -1, 0);
            break;

        case 'a': 
            depth++;
            cout << "Increasing depth of L-system.  New depth: " << depth << endl;
            break;

        case 's':
            depth--;
            if (depth < 1) {
                cout << "Depth of 1 is the set minimum." << endl;
                    depth = 1;
            } else {
                cout << "Decreasing depth of L-system.  New depth: " << depth << endl;
            } // if-else

            break;
                
        case 'p':
            cout << "Changing to ";
            if (projectionMode == ORTHO_PROJECTION) {
                cout << "perspective";
                projectionMode = PERSP_PROJECTION;
            } else {
                cout << "orthogonal";
                projectionMode = ORTHO_PROJECTION;
            } // if-else
            
            cout << " projection" << endl;
            setProjection(projectionMode);
            break;
                
        case 'q':
            cout << "Exiting program";
            fflush(stdout);
            exit(0);
            break;

        default:return;
    } // switch

    display();
} // keyHandler

void mouseHandler(int x, int y) {
    // Clockwise rotation
    if (x < m_last_x) {
        cout << "Rotating about y axis clockwise" << endl;
        rotateBy(cwRotate, 0, 1, 0);
    } else {
        cout << "Rotating about y axis counter-clockwise" << endl;
        rotateBy(ccwRotate, 0, 1, 0);
    } // if-else
    
    m_last_x = x;
    display();
} // mouseHandler

/* end of plant.c */
