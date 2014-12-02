/*
 * drawplant.cpp
 * -------------
 * Contains the drawing routine and related helper functions for the
 * L-system fractal plant.  Starter code for Project 2.
 *
 * Group Members: Matt Schwartz
 */

#ifdef _WIN32
#include <windows.h>
#endif
#include <assert.h>

#include "common.h"
#include "drawplant.h"
#include "iostream"

void centerBranch(int depth);
void leftBranch(int depth);
void rightBranch(int depth);
void turnLeft(void);
void turnRight(void);
void terminatingLeaf(void);
void drawLeaf(void);
void drawFruit(void);
void drawBranch(int direction, int depth);
void drawTree(int depth);
void initialize(void);

// Changes during seasons
double leavesRedWeight = 0.45, leavesGreenWeight = 1.0, leavesBlueWeight = 0.05;
// Different colored tree bark
double treeRed = 0.54, treeGreen = 0.27, treeBlue = 0.07;

int randomSeed; // used for randomizing leaves

int drawLeaves = TRUE; // false during the winter
int scaleFactor = 1; // used for scaling deeper branches

int angle = 25;

// Center of tree
void centerBranch(int depth) {
    int terminatingBranch = 0;
    if (depth == 0) {
        translateBy(0, 5 * (1.0 / scaleFactor), 0);
        terminatingLeaf();
        return;
    } // if
    
    if (depth == 1) {
        terminatingBranch = TERM;
    } // if
    
    translateBy(0, 6 * (depth * 1.0 / scaleFactor), 0);
    drawBranch(CENTER, depth);
    
    // Left branch
    pushMatrix();
    turnLeft();
    translateBy(0, 6 * (depth * 1.0 / scaleFactor), 0);
    drawBranch(LEFT | terminatingBranch, depth);
    leftBranch(depth - 1);
    popMatrix();
    
    // 3D left branch
    pushMatrix();
    rotateBy(90, 0, 1, 0);
    rotateBy(15, 0, 0, 1);
    translateBy(1, 0, 0);
    turnLeft();
    translateBy(0, 6 * (depth * 1.0 / scaleFactor), 0);
    drawBranch(LEFT | terminatingBranch, depth);
    leftBranch(depth - 1);
    popMatrix();
    
    translateBy(0, 6 * (depth * 1.0 / scaleFactor), 0);
    drawBranch(CENTER, depth);
    
    // Right branch
    pushMatrix();
    turnRight();
    translateBy(0, 6 * (depth * 1.0 / scaleFactor), 0);
    drawBranch(RIGHT | terminatingBranch, depth);
    rightBranch(depth - 1);
    popMatrix();
    
    // 3D right branch
    pushMatrix();
    rotateBy(90, 0, 1, 0);
    rotateBy(-15, 0, 0, 1);
    translateBy(-1, 0, 0);
    turnRight();
    translateBy(0, 6 * (depth * 1.0 / scaleFactor), 0);
    drawBranch(RIGHT | terminatingBranch, depth);
    rightBranch(depth - 1);
    popMatrix();
    
    translateBy(0, 6 * (depth * 1.0 / scaleFactor), 0);
    drawBranch(CENTER | TERM, depth);
    centerBranch(depth - 1);
} // centerBranch

// Left half of tree
void leftBranch(int depth) {
    int terminatingBranch = 0;
    
    if (depth == 0) {
        translateBy(0, 5 * (1.0 / scaleFactor), 0);
        terminatingLeaf();
        return;
    } // if
    
    if (depth == 1) {
        terminatingBranch = TERM;
    }
    
    translateBy(0, 6 * (1.0 / scaleFactor), 0);
    drawBranch(CENTER, depth);
    
    // Continue left branch
    pushMatrix();
    turnLeft();
    translateBy(0, 6 * (depth * 1.0 / scaleFactor), 0);
    drawBranch(LEFT | terminatingBranch, depth);
    leftBranch(depth - 1);
    popMatrix();
    
    // 3D left branch
    pushMatrix();
    rotateBy(90, 0, 1, 0);
    rotateBy(15, 0, 0, 1);
    translateBy(1, 0, 0);
    turnLeft();
    translateBy(0, 6 * (depth * 1.0 / scaleFactor), 0);
    drawBranch(LEFT | terminatingBranch, depth);
    leftBranch(depth - 1);
    popMatrix();
    
    translateBy(0, 6 * (depth * 1.0 / scaleFactor), 0);
    drawBranch(CENTER, depth);
    
    // Right branch
    pushMatrix();
    turnRight();
    translateBy(0, 6 * (depth * 1.0 / scaleFactor), 0);
    drawBranch(RIGHT | TERM, depth);

    translateBy(0, 5 * (depth * 1.0 / scaleFactor), 0);
    terminatingLeaf();
    
    popMatrix();
    
    translateBy(0, 6 * (depth * 1.0 / scaleFactor), 0);
    drawBranch(CENTER | TERM, depth);
    leftBranch(depth - 1);
} // leftBranch

// Right half of tree
void rightBranch(int depth) {
    int terminatingBranch = 0;
    if (depth == 0) {
        translateBy(0, 5 * (1.0 / scaleFactor), 0);
        terminatingLeaf();
        return;
    } // if
    
    if (depth == 1) {
        terminatingBranch = TERM;
    } // if
    
    translateBy(0, 6 * (depth * 1.0 / scaleFactor), 0);
    drawBranch(CENTER, depth);
    
    // Left branch
    pushMatrix();
    turnLeft();
    translateBy(0, 6 * (depth * 1.0 / scaleFactor), 0);
    drawBranch(LEFT | TERM, depth);
    translateBy(0, 5 * (depth * 1.0 / scaleFactor), 0);
    terminatingLeaf();
    popMatrix();
    
    translateBy(0, 6 * (depth * 1.0 / scaleFactor), 0);
    drawBranch(CENTER, depth);
    
    // Right branch
    pushMatrix();
    turnRight();
    translateBy(0, 6 * (depth * 1.0 / scaleFactor), 0);
    drawBranch(RIGHT | terminatingBranch, depth);
    rightBranch(depth - 1);
    popMatrix();
    
    // 3D right branch
    pushMatrix();
    rotateBy(90, 0, 1, 0);
    rotateBy(-15, 0, 0, 1);
    translateBy(-1, 0, 0);
    turnRight();
    translateBy(0, 6 * (depth * 1.0 / scaleFactor), 0);
    drawBranch(RIGHT | terminatingBranch, depth);
    rightBranch(depth - 1);
    popMatrix();
    
    translateBy(0, 6 * (depth * 1.0 / scaleFactor), 0);
    drawBranch(CENTER | TERM, depth);
    rightBranch(depth - 1);
} // rightBranch

void setBranchAngle(int ang) {
    angle = ang;
} // setAngle

/**
 * Rotates the camera by an angle to the left.  Consolidating this into a 
 * function makes the l-system more readable
 */
void turnLeft() {
    rotateBy(angle, 0, 0, 1);
} // turnLeft

/**
 * Rotates the camera by an angle to the right.  Consolidating this into a 
 * function makes the l-system more readable
 */
void turnRight() {
    rotateBy(-angle, 0, 0, 1);
} // turnRight

// Draws 3 leaves around the end of a branch.. once upon a time
void terminatingLeaf(void) {
    if (!drawLeaves) {
        return;
    } // if
    
    if ( (rand() % 100)  < 2) {
        drawFruit();
    } else {
        pushMatrix();
        rotateBy( (rand() % 90), 0, 0, 1);
        rotateBy( -(rand() % 10), 0, 1, 0);
        drawLeaf();
        popMatrix();
    } // if-else
} // terminatingLeaf

// Draw a leaf of 0.25 thickness to the screen
void drawLeaf(void) {
    /* ADD YOUR CODE to make the 2D leaf a 3D extrusion */
    // RANDOM leaf-looking colors
    double green = (rand() % 100) / 100.0;
    double red = (rand() % 100) / 100.0;
    double blue = (rand() % 100) / 100.0;
    
    GLfloat vertices[] = {0.0, 0.0, -0.125,
                          1.0, 0.7, -0.125,
                          1.3, 1.8, -0.125,
                          1.0, 2.8, -0.125,
                          0.0, 4.0, -0.125,
                         -1.0, 2.8, -0.125,
                         -1.3, 1.8, -0.125,
                         -1.0, 0.7, -0.125,
    
                          // Top of leaf
                          0.0, 0.0, 0.125,
                          1.0, 0.7, 0.125,
                          1.3, 1.8, 0.125,
                          1.0, 2.8, 0.125,
                          0.0, 4.0, 0.125,
                         -1.0, 2.8, 0.125,
                         -1.3, 1.8, 0.125,
                         -1.0, 0.7, 0.125 };
    
    GLuint indices[] = { 0,  1,  2,  3,  4,  5,  6,  7, // bottom
                         8,  9, 10, 11, 12, 13, 14, 15, // top
                         0,  1,  9,  8,
                         1,  2, 10,  9,
                         2,  3, 11, 10,
                         3,  4, 12, 11,
                         4,  5, 13, 12,
                         5,  6, 14, 13,
                         6,  7, 15, 14,
                         7,  0,  8, 15 };
    
    glColor3f(red * leavesRedWeight, green * leavesGreenWeight, blue * leavesBlueWeight); 
    
    int i;
    int v1, v2, v3;
    
    glBegin(GL_POLYGON);
    for (i = 0; i < 48; i++) {
        v1 = indices[i]*3;
        v2 = indices[i]*3 + 1;
        v3 = indices[i]*3 + 2;
        
        glVertex3f(vertices[v1], vertices[v2], vertices[v3]);
    } // for
    glEnd();
} // drawLeaf

// Draw an apple!  An apple's a fruit!
void drawFruit(void) {
    translateBy(0, -2.5, 0);
    glColor3f(1.0, 0.2, 0.04); 
    glutSolidSphere(1.5, 30, 30);
} // drawFruit

/**
 * Draws a branch.  The more deep you go into the l-system, the smaller the 
 * branch becomes.
 * NOTE:  This was completed before I realized a simpler way to do it was creating
 * two arrays: vertices and indices.  But it worked, and more work is more work,
 * so I left it as is.
 * @param direction left, right, center, or term as defined in the enum inside
 * of drawplant.h.  LEFT and RIGHT make it so the branch will appear to connect
 * to the rest of the tree easier.  TERM will cause the top of the branch
 * to taper slightly.  
 * @param depth used for scaling the branch
 */
void drawBranch(int direction, int depth) {
    glColor3f(treeRed, treeGreen, treeBlue);
    
    /* ADD YOUR CODE to make the 2D branch a 3D extrusion */
    GLfloat lowerLeftBack[3]      = {-1.0, 0.0, -1.0};
    GLfloat lowerRightBack[3]     = { 1.0, 0.0, -1.0};
    GLfloat upperLeftBack[3]      = {-1.0, 6.0, -1.0};
    GLfloat upperRightBack[3]     = { 1.0, 6.0, -1.0};
    
    GLfloat lowerLeftFront[3]     = {-1.0, 0.0,  1.0};
    GLfloat lowerRightFront[3]    = { 1.0, 0.0,  1.0};
    GLfloat upperLeftFront[3]     = {-1.0, 6.0,  1.0};
    GLfloat upperRightFront[3]    = { 1.0, 6.0,  1.0};
    
    if (direction & RIGHT) {
        lowerRightBack[1] = -6.0;
        lowerLeftBack[1] = -3.0;
        
        lowerRightFront[1] = -6.0;
        lowerLeftFront[1] = -3.0;
    } // if
    else if (direction & LEFT) {
        lowerRightBack[1] = -3.0;
        lowerLeftBack[1] = -6.0;
        
        lowerRightFront[1] = -3.0;
        lowerLeftFront[1] = -6.0;
    } // else if
    
    if (direction & TERM) {
        upperRightBack[0] = 0.25;
        upperLeftBack[0] = -0.25;
        
        upperRightFront[0] = 0.25;
        upperLeftFront[0] = -0.25;
    } // if
    
    pushMatrix();
    scaleBy((depth * 1.0 / scaleFactor), depth*1.0 / scaleFactor, depth*1.0 / scaleFactor);
    
    glBegin(GL_POLYGON);
    
    // Draw back face
    glVertex3fv(lowerRightBack);
    glVertex3fv(upperRightBack);
    glVertex3fv(upperLeftBack);
    glVertex3fv(lowerLeftBack);
    
    // Draw front face
    glVertex3fv(lowerRightFront);
    glVertex3fv(upperRightFront);
    glVertex3fv(upperLeftFront);
    glVertex3fv(lowerLeftFront);
    
    // Draw left face
    glVertex3fv(lowerLeftFront);
    glVertex3fv(upperLeftFront);
    glVertex3fv(upperLeftBack);
    glVertex3fv(lowerLeftBack);
    
    // Draw top face
    glVertex3fv(upperRightFront);
    glVertex3fv(upperRightBack);
    glVertex3fv(upperLeftBack);
    glVertex3fv(upperLeftFront);
    
    // Draw right face
    glVertex3fv(lowerRightBack);
    glVertex3fv(upperRightBack);
    glVertex3fv(upperRightFront);
    glVertex3fv(lowerRightFront);
    
    // Draw bottom face
    glVertex3fv(lowerRightFront);
    glVertex3fv(lowerRightBack);
    glVertex3fv(lowerLeftBack);
    glVertex3fv(lowerLeftFront);
    
    // Draws a peak on the branch if the branch is a terminating branch
    if (direction & TERM) {
        // Draw back face
        glVertex3fv(upperRightBack);
        glVertex3fv(lowerLeftBack);
        glVertex3f(0.0, 7.0, -0.5);
    
        // Draw front face
        glVertex3fv(upperRightFront);
        glVertex3fv(lowerLeftFront);
        glVertex3f(0.0, 7.0, 0.5);
        
        // Draw left face
        glVertex3fv(upperLeftFront);
        glVertex3f(0.0, 7.0, 0.5);
        glVertex3f(0.0, 7.0, -0.5);
        glVertex3fv(upperLeftBack);
        
        // draw right face
        glVertex3fv(upperRightFront);
        glVertex3f(0.0, 7.0, 0.5);
        glVertex3f(0.0, 7.0, -0.5);
        glVertex3fv(upperRightBack);
    } // if
    
    popMatrix();
    glEnd();
    
} // drawBranch

/*
 * Draws the plant.
 *
 * ADD YOUR CODE and modify the function to take an L-system depth and
 * any other necessary arguments.
 */
void drawTree(int depth) {    
    srand(randomSeed);
    
    glDepthMask(GL_FALSE); // disable z buffer before drawing ppm file
    int w;
    int h;
    
    GLubyte *pixels = readPPMfile("mountains.ppm", &w, &h);
    
    glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glDepthMask(GL_TRUE); // reenable z buffer
    
    scaleFactor = depth;
    pushMatrix();
    initialize();
    centerBranch(depth);
    popMatrix();
} // drawPlant

// Change the season which changes the leaves
void setSeason(int season) {
    seedRandomColor();
    
    switch (season) {
        case SUMMER:
            cout << "summer";
            leavesRedWeight = 0.45;
            leavesGreenWeight = 1.0;
            leavesBlueWeight = 0.05;
            drawLeaves = TRUE;
            break;
            
        case AUTUMN:
            cout << "autumn";
            leavesRedWeight = 0.90;
            leavesGreenWeight = 0.58;
            leavesBlueWeight = 0.05;
            drawLeaves = TRUE;
            break;
            
        case WINTER: 
            cout << "winter";
            drawLeaves = FALSE;
            break;
            
        case SPRING:
            cout << "spring";
            leavesRedWeight = 0.25;
            leavesGreenWeight = 1.0;
            leavesBlueWeight = 0.05;
            drawLeaves = TRUE;
            break;
            
        default:
            return;
    } // switch
    cout << endl;
} // setWeather

/**
 * Only changes the color of the tree bark.  In a perfect word, this would have
 * also changed the L-System to represent the types of trees being drawn.
 * @param tree
 */
void setTree(int tree) {
    switch (tree) {
        case SPRUCE:
            cout << "spruce";
            treeRed = 0.324;
            treeGreen = 0.2538;
            treeBlue = 0.0588;
            break;
            
        case YEW:
            cout << "yew";
            treeRed = 0.216;
            treeGreen = 0.2214;
            treeBlue = 0.0693;
            break;
    
        case WILLOW:
            cout << "willow";
            treeRed = 0.1296;
            treeGreen = 0.162;
            treeBlue = 0.0434;
            break;
    
        case REDWOOD:
            cout << "redwood";
            treeRed = 0.3456;
            treeGreen = 0.0108;
            treeBlue = 0.0469;
            break;
            
        default:
            return;

    } // switch
    cout << endl;
} // setTree

// Seeds the random generator so that the leaves will be drawn with random colors
// but the same random colors each time before the seed changes
void seedRandomColor(void) {
    randomSeed = (int)time(0);
} // seedRandomColor

/**
 *  Before calling the recursive L-system, draws the tree trunk to the screen.
 *  It's supposed to resemble a hexagonal cylinder (which is more realistic
 *  than a box!)
 */
void initialize(void) {
    // Draw the trunk
    
    GLfloat vertices[] = { 1.0, 0.0,  -2.0,
                           2.0, 0.0,  0.0,
                           1.0, 0.0,  2.0,
                          -1.0, 0.0,  2.0,
                          -2.0, 0.0,  0.0,
                          -1.0, 0.0, -2.0,
    
                          // top vertices
                           1.0, 18.0,  -1.0,
                           1.0, 18.0,  0.0,
                           1.0, 18.0,  1.0,
                          -1.0, 18.0,  1.0,
                          -1.0, 18.0,  0.0,
                          -1.0, 18.0, -1.0 };
    
    GLuint indices[] = {  0,  1,  2,  3,  4,  5, // top
                          6,  7,  8,  9, 10, 11,// bottom
                          1,  7,  6,  0,
                          2,  8,  7,  1,
                          3,  9,  8,  2,
                          4, 10,  9,  3,
                          5, 11, 10,  4,
                          0,  6, 11,  5 };
    
    translateBy(0, -39, 0);
    
    int i;
    int v1, v2, v3;
    
    glColor3f(treeRed, treeGreen, treeBlue); 
    glBegin(GL_QUADS);
    
    for (i = 0; i < 36; i++) {
        v1 = indices[i] * 3;
        v2 = indices[i] * 3 + 1;
        v3 = indices[i] * 3 + 2;
        
        glVertex3f(vertices[v1], vertices[v2], vertices[v3]);
    } // for
    
    glEnd();
    
    translateBy(0, 12, 0);
} // initialize
/* end of drawplant.c */
