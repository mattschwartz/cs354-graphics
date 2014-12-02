
/*
    File name:  matrix.cpp
    Project:    Project 2
    Author:     Matt Schwartz
    Date:       03.05.2013
    Description:This class holds all matrix functions needed for this project
 */
#include "common.h"
#include <stack>

stack<GLfloat *> matrix_stack;

/* Used when loadIdentity is called */
GLfloat identitymatrix[16] = { 1, 0, 0, 0,
                               0, 1, 0, 0,
                               0, 0, 1, 0,
                               0, 0, 0, 1 };

/**
 * Transpose the given matrix and returns it.
 * @param dimension
 * @param matrix
 * @return 
 */
GLfloat *transpose(int dimension, const GLfloat *matrix) {
    GLfloat *transpose = (GLfloat *)malloc((size_t)(16 * sizeof(GLfloat)));
    
    for (int r = 0; r < dimension; r++) {
        for (int c = 0; c < dimension; c++) {
            transpose[r + c * dimension] = matrix[c + r * dimension];
        } // for
    } // for
    
    return transpose;
} // transpose

/**
 * used for debug; prints a matrix to the screen for review.
 * @param dimension
 * @param message
 * @param matrix
 */
void printMatrix(int dimension, const char *message, const GLfloat *matrix) {
    cout << "---------------------------------" << endl;
    cout << message << endl;
        for (int i = 0; i < dimension; i++) {
            for (int c = 0; c < dimension; c++) {
                printf("%5.2f ", matrix[c + i * dimension]);
//                printf("%f ", matrix[c + i * dimension]);
            } // for
            cout << endl;
	} // for
} // printMatrix

/**
 * Multiplies a multi-dimensional matrix by a given vector.  Returns a flattened 
 * array.  access by calling array[r + c * dimension].
 * @param dimension this is the dimension of the matrix, not 2D or 3D.  if you want
 * to use a 3x3 matrix, dimension = 3.  this function expects the arguments to
 * follow the rules of matrix multiplication and does not guarantee any stability
 * if these rules are not followed.
 * @param matrix the matrix to use in the multiplication
 * @param vector the vector used
 */
GLfloat *matrixVectorMult(int dimension, const GLfloat *matrix, const GLfloat *vector) {
    GLfloat *product = (GLfloat *)malloc((size_t)(dimension * sizeof(GLfloat)));
    
    for (int r = 0; r < dimension; r++) {
        product[r] = 0;
        for (int c = 0; c < dimension; c++) {
            product[r] += matrix[r + c * dimension] * vector[c];
        } // for
    } // for
    
    return product;
} // matrixVectorMult

/**
 * Multiplies two matrices together.  Expects square [dimensionxdimension] matrices.
 * Returns a flattened array.  access by calling array[r + c * dimension].
 * @param matrixA the left matrix to be used
 * @param matrixB the right matrix to be used
 */
GLfloat *matrixMatrixMult(int dimension, const GLfloat *matrixA, const GLfloat *matrixB) {
    float sum;
    GLfloat *product;
    product = (GLfloat *)malloc((size_t)(dimension * dimension * sizeof(GLfloat)));

    for (int r = 0; r < dimension; r++) {
        for (int c = 0; c < dimension; c++) {

            sum = 0;
            for (int i = 0; i < dimension; i++) {
                sum += matrixA[i + r * dimension] * matrixB[c + i * dimension];
            } // for
            
            product[r + c * dimension] = sum;
        } // for
    } // for
    
    // It seems my algorithm performs the transpose.. i decided to make it easier 
    // on myself and just write a transpose function
    return transpose(dimension, product);
} // matrixMatrixMult

/* Translate a matrix by specifying the x y and z coordinates
	by which you want the matrix translated. */
void translateBy(GLfloat x, GLfloat y, GLfloat z) {
    int dimension = 4;
    GLfloat translationmatrix[] = { 1, 0, 0, x,
                                    0, 1, 0, y,
                                    0, 0, 1, z,
                                    0, 0, 0, 1 };
    GLfloat *newDisplay;

    newDisplay = matrixMatrixMult(dimension, getCurrentMatrix(), translationmatrix);
    
    load3DMatrix(newDisplay);
} // translateBy

/**
 * Produces and loads a rotation matrix*current matrix into openGL.  For as of yet
 * unknown reason(s), higher angles produces a smoother, perceptively smaller, 
 * rotations...
 * @param angle the angle by which to rotate, in degrees
 * @param x x-value of the rotation vector
 * @param y y-value of the rotation vector
 * @param z z-value of the rotation vector
 */
void rotateBy(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
    int dimension = 4;
    
    // Convert to radians
    angle = (angle * M_PI) / 180;
    
    // From http://www.opengl.org/sdk/docs/man2/xhtml/glRotate.xml
    GLfloat rotationMatrix[] = { x*x*(1-cos(angle)) + cos(angle), x*y*(1-cos(angle)) - z*sin(angle), x*z*(1-cos(angle))+y*sin(angle), 0,
                                 y*x*(1-cos(angle))+z*sin(angle), y*y*(1-cos(angle))+cos(angle), y*z*(1-cos(angle))-x*sin(angle), 0,
                                 x*z*(1-cos(angle))-y*sin(angle), y*z*(1-cos(angle))+x*sin(angle), z*z*(1-cos(angle))+cos(angle), 0,
                                 0, 0, 0, 1};
    
    GLfloat *newDisplay;
    // Old code that i may or may not use in the future.. it was prettier...
    // Does the same thing as the openGL docs code, but takes more space
//    GLfloat rotationMatrix[] = { 1, 0, 0, 0,
//                                 0, 1, 0, 0,
//                                 0, 0, 1, 0,
//                                 0, 0, 0, 1};
//
//    if (x > 0) {
//        rotationMatrix[1 + 1 * dimension] = cos(angle);
//        rotationMatrix[2 + 1 * dimension] = -sin(angle);
//        rotationMatrix[1 + 2 * dimension] = sin(angle);
//        rotationMatrix[2 + 2 * dimension] = cos(angle);
//    } // else if
//    else if (y > 0) {
//        rotationMatrix[0 + 0 * dimension] = cos(angle);
//        rotationMatrix[2 + 0 * dimension] = sin(angle);
//        rotationMatrix[0 + 2 * dimension] = -sin(angle);
//        rotationMatrix[2 + 2 * dimension] = cos(angle);
//    } // else if
//    else if (z > 0) {
//        rotationMatrix[0 + 0 * dimension] = cos(angle);
//        rotationMatrix[1 + 0 * dimension] = -sin(angle);
//        rotationMatrix[0 + 1 * dimension] = sin(angle);
//        rotationMatrix[1 + 1 * dimension] = cos(angle);
//    } // else if

    newDisplay = matrixMatrixMult(dimension, getCurrentMatrix(), rotationMatrix);
    
    load3DMatrix(newDisplay);
} // rotateBy

/**
 * Scale the MODELVIEW matrix.
 * @param x scale in x
 * @param y scale in y
 * @param z scale in z
 */
void scaleBy(GLfloat x, GLfloat y, GLfloat z) {
    int dimension = 4;
    GLfloat scaleMatrix[] = { x, 0, 0, 0,
                                 0, y, 0, 0,
                                 0, 0, z, 0,
                                 0, 0, 0, 1};
    
    GLfloat *newDisplay = (GLfloat *)malloc((size_t)(16 * sizeof(GLfloat *)));
    
    newDisplay = matrixMatrixMult(dimension, getCurrentMatrix(), scaleMatrix);
    
    load3DMatrix(newDisplay);
} // scaleBy

/**
 * Load the identity matrix to be drawn
 */
void loadIdentity(void) {
    GLfloat *identity_copy = (GLfloat *)malloc((size_t)(16 * sizeof(GLfloat *)));
    memcpy(identity_copy, identitymatrix, 16 * sizeof(GLfloat));
    matrix_stack.push(identity_copy);
} // loadIdentity

/**
 * Push the current matrix onto the stack.
 */
void pushMatrix() {
    matrix_stack.push(matrix_stack.top());
} // pushMatrx

/**
 * Use the restoreMatrix to restore the display before the push was called.
 * Does nothing if restore is null (meaning pop before push).
 */
void popMatrix() {
    matrix_stack.pop();
} // popMatrx

/**
 * Return the column-major dislpaymatrix in row-major order by transposing it 
 * before returning.
 * @return 
 */
GLfloat *getCurrentMatrix() {
    return transpose(4, matrix_stack.top());
} // loadCurrentMatrix


// deprecated
/* Takes a 2D matrix in row-major order, and loads the 3D matrix which
   does the same trasformation into the OpenGL MODELVIEW matrix, in
   column-major order. */
void load2DMatrix(
		GLfloat m00, GLfloat m01, GLfloat m02,
		GLfloat m10, GLfloat m11, GLfloat m12,
		GLfloat m20, GLfloat m21, GLfloat m22) {

    GLfloat M3D [16];  /* three dimensional matrix doing same transform */

    M3D[0] = m00;  M3D[1] = m10; M3D[2] = 0.0; M3D[3] = m20;
    M3D[4] = m01;  M3D[5] = m11; M3D[6] = 0.0; M3D[7] = m21;
    M3D[8] = 0.0;  M3D[9] = 0.0; M3D[10] = 1.0; M3D[11] = 0.0;
    M3D[12] = m02; M3D[13] = m12; M3D[14] = 0.0; M3D[15] = m22;

    // load into display matrix
//    memcpy(modelviewMatrix, M3D, sizeof(M3D));

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(M3D);
}

// deprecated - use following function instead
/* Takes a 3D matrix in row-major order, and loads the 3D matrix which
   does the same trasformation into the OpenGL MODELVIEW matrix, in
   column-major order. */
void load3DMatrix(
		GLfloat m00, GLfloat m01, GLfloat m02, GLfloat m03,
		GLfloat m10, GLfloat m11, GLfloat m12, GLfloat m13,
		GLfloat m20, GLfloat m21, GLfloat m22, GLfloat m23,
		GLfloat m30, GLfloat m31, GLfloat m32, GLfloat m33) {

    /* ADD YOUR CODE */
    GLfloat M3D [16];
    M3D[ 0] = m00;  M3D[ 1] = m10;  M3D[ 2] = m20;  M3D[ 3] = m30;
    M3D[ 4] = m01;  M3D[ 5] = m11;  M3D[ 6] = m21;  M3D[ 7] = m31;
    M3D[ 8] = m02;  M3D[ 9] = m12;  M3D[10] = m22;  M3D[11] = m32;
    M3D[12] = m03;  M3D[13] = m13;  M3D[14] = m23;  M3D[15] = m33;

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(M3D);
} // load3DMatrix

/**
 * Loads a row-major 3D matrix into the OpenGL MODELVIEW matrix in column-major 
 * order.  Instead of taking 16 floats, it takes one matrix and does all the hard
 * work for you!
 * @param matrix the row-major matrix to load
 */
void load3DMatrix(const GLfloat *matrix) {
    // Set the top of the matrix to the transpose of matrix
    matrix_stack.top() = transpose(4, matrix);
    
    // Load the top of the stack to be drawn to the screen
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(matrix_stack.top());
} // load3DMatrix