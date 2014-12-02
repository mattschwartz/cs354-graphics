/*
 * subdiv.cpp
 * ----------
 * Contains function callbacks and bookkeeping for window management.
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
#include "drawing.h"
#include "data.h"
#include "mouse.h"

#define X_AXIS			0
#define Y_AXIS			1
#define Z_AXIS			2

/* GLOBAL VARAIBLES */

/* (storage is actually allocated here) */
int W=400;		/* window width */
int H=400;		/* window height */
int X_OFF = 10;	/* window x offset */
int Y_OFF = 10;	/* window y offset */

/* The dimensions of the viewing frustum */
GLfloat fleft   = -(W / 2);
GLfloat fright  =  W / 2;
GLfloat fbottom = -(H / 2);
GLfloat ftop    =  H / 2;
GLfloat zNear   =  (W / 2);
GLfloat zFar    = -(W / 2);
GLfloat zoomFactor = 1.0;

int shininessValue = LOW_SHINY;
int dimensionMode = THREE_D_MODE;
int extraFeatureMode = AVERAGE_MODE;

/* The current mode the mouse is in, based on what button(s) is pressed */
int mouse_mode;

/* The last position of the mouse since the last callback */
int m_last_x = 0, m_last_y = 0;

/* local function declarations */
void init(void);
void display(void);
void lightSource(bool clapOn);
void onResize(void);
void myKeyHandler(unsigned char ch, int x, int y);
void rotateCamera(double deg, int axis);
void zoomCamera(double delta);
void resetCamera(void);
void endSubdiv(int status);

int main (int argc, char **argv) {
  glutInit(&argc,argv);
  glutInitWindowSize(W, H);
  glutInitWindowPosition(X_OFF, Y_OFF);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("subdiv");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(myKeyHandler);
  glutMouseFunc(myMouseButton);
  glutMotionFunc(myMouseMotion);
  glutMainLoop();
  return 0;
}

void init() {
    // Note: these values, and how to use them came from: 
    // http://www.falloutsoftware.com/tutorials/gl/gl8.htm
    GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { -150.0f, 0.0f, 0.0f, 1.0f };
    
    float specReflection[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    glClearColor(0.0, 0.0, 0.0, 0.0);  
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(fleft, fright, fbottom, ftop, -zNear, -zFar);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glFrontFace(GL_CCW);
    glShadeModel(GL_SMOOTH); // gouraud shading

    // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
    glMateriali(GL_FRONT, GL_SHININESS, 1);
}

void display() {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*
     * See drawing.c for the definition of these routines.
     *
     * Note: Only one should be called at a time (based on the
     * display mode).
     */
    
    if (drawing3D()) {
        lightSource(true);
        draw3DModel();
    } // if
    
    else {
        lightSource(false);
        drawSurface();
    } // else

    glFlush();  /* Flush all executed OpenGL ops finish */

    onResize();

    /*
     * Since we are using double buffers, we need to call the swap
     * function every time we are done drawing.
     */
    glutSwapBuffers();
}

void lightSource(bool clapOn) {
    if (clapOn) {
        glEnable(GL_LIGHTING);
        if (shininessValue == LOW_SHINY) {
            glMateriali(GL_FRONT, GL_SHININESS, 128);
        } // if

        else if (shininessValue == HIGH_SHINY) {
            glMateriali(GL_FRONT, GL_SHININESS, 15);
        } // else
    } // if
    
    else {
        glDisable(GL_LIGHTING);
    } // else
} // lightSource

void onResize(void) {
    GLint m_viewport[4];
    
    glGetIntegerv(GL_VIEWPORT, m_viewport);
    
    if (m_viewport[2] == W || m_viewport[3] == H) {
        return;
    } // if
    
    W = m_viewport[2];
    H = m_viewport[3];
    
    fleft   = -(W / 2);
    fright  =  W / 2;
    fbottom = -(H / 2);
    ftop    =  H / 2;
    zNear   =  (W / 2);
    zFar    = -(W / 2);
    
    resetCamera();
    m_last_x = m_last_y = 0;
} // onResize

void myKeyHandler(unsigned char ch, int x, int y) {
    switch(ch) {
        case 'w':
            setup3DModel();
            break;
            
        case 'e':
            toggleDisplayMode();
            break;
            
        case 'r':
            toggleControlPoints();
            
            break;
            
        case 'a':
            subdivideVertical();
            break;
            
        case 'b':
            subdivideHorizontal(true);
            break;
            
        case 'd':
            toggleShadingMode();
            break;
            
        case 's':
            if (!drawing3D()) {
                return;
            } // if
            
            cout << "Changing shininess." << endl;
            
            shininessValue++;
            
            if (shininessValue > HIGH_SHINY) {
                shininessValue = LOW_SHINY;
            } // if
            break;
            
        case 'z':
            toggle3D();
            resetCamera();
            break;
            
        case 'p':
            if (!drawing3D()) {
                return;
            } // if
            
            cout << "[NYI] Changing to the ";
            
            extraFeatureMode++;
            
            if (extraFeatureMode > PROFILE_MODE) {
                extraFeatureMode = AVERAGE_MODE;
            } // if
            
            if (extraFeatureMode == AVERAGE_MODE) {
                cout << "average of the normal of all of the polygons";
            } // if
            
            else if (extraFeatureMode == PROFILE_MODE) {
                cout << "normal from the profile curve.";
            } // else if
            
            cout << endl;
            break;
            
        case 'c':
            resetCamera();
            cout << "Camera reset." << endl;
            break;
            
        case '=':
            zoomCamera(0.1);
            break;
            
        case 'C':
            removeAllPoints();
            resetCamera();
            cout << "Removing all points." << endl;
            break;
            
        case 'q':
                endSubdiv(0);
                break;
        default:
                /* Unrecognized keypress */
        return;
	}

	glutPostRedisplay();

	return;
}

/*
 * The rotation is specified in degrees about a certain axis of
 * the original model.
 *
 * AXIS should be either X_AXIS, Y_AXIS, or Z_AXIS.
 *
 * Positive degrees rotate in the counterclockwise direction.
 */
void rotateCamera(double deg, int axis) {
    double x, y, z;

    x = 0;
    y = 0;
    z = 0;

    if (axis == X_AXIS) {
            x = 1.0f;
    } else if (axis == Y_AXIS) {
            y = 1.0f;
    } else if (axis == Z_AXIS) {
            z = 1.0f;
    }

    glRotatef(deg, x, y, z);
}

/*
 * Changes the level of zooming by adjusting the dimenstions of the viewing
 * frustum.
 *
 * Args: delta - the change in the zoom factor.  Negative deltas cause the
 * camera to zoom in, while positive values cause the camera to zoom out.
 */
void zoomCamera(double delta) {
    zoomFactor += delta;

    if (zoomFactor <= 0.0) {
            /* The zoom factor should be positive */
            zoomFactor = 0.001;
    }
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /*
     * glFrustum must receive positive values for the near and far clip planes
     * ( arguments 5 and 6 ).
     */
    glOrtho(fleft*zoomFactor, fright*zoomFactor,
            fbottom*zoomFactor, ftop*zoomFactor,
            -zNear, -zFar);
}

/*
 * Resets the viewing frustum and moves the drawing point to the center of
 * the frustum.
 */
void resetCamera( void ) {
    init();
    zoomFactor = 1.0;
}

void myMouseButton(int button, int state, int x, int y) {
    // Mouse turns object in 3D mode
    if (drawing3D()) {
        m_last_x = x;
        m_last_y = y;
        return;
    } // if
    
    if (x > W / 2) {
        x -= W / 2;
    } // if
    else {
        x = (x - W + W / 2);
    } // else
    
    if (y < H / 2) {
        y = H / 2 - y;
    } // if
    else {
        y = -(y - H / 2);
    } // else
    
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			// Add a point, if there is room
            addPoint(x, y);
		} // if
        
        else if (button == GLUT_RIGHT_BUTTON) {
            removeLastPoint();
        } // else if
	} // if
    
    display();
}

void myMouseMotion(int x, int y) {
    if (!drawing3D()) {
        return;
    } // if
    
    double d_x, d_y;	/* The change in x and y since the last callback */

    d_x = x - m_last_x;
    d_y = y - m_last_y;

    m_last_x = x;
    m_last_y = y;

    if (mouse_mode == MOUSE_ROTATE_YX) {
        /* scaling factors */
        d_x /= 2.0;
        d_y /= 2.0;

        glRotatef(d_x, 0.0, 1.0, 0.0);	/* y-axis rotation */
        glRotatef(-d_y, 1.0, 0.0, 0.0);	/* x-axis rotation */

    } else if (mouse_mode == MOUSE_ROTATE_YZ) {
        /* scaling factors */
        d_x /= 2.0;
        d_y /= 2.0;

        glRotatef(d_x, 0.0, 1.0, 0.0);	/* y-axis rotation */
        glRotatef(-d_y, 0.0, 0.0, 1.0);	/* z-axis rotation */

    } else if (mouse_mode == MOUSE_ZOOM) {
        d_y /= 100.0;

        zoomFactor += d_y;

        if (zoomFactor <= 0.0) {
                /* The zoom factor should be positive */
                zoomFactor = 0.001;
        }

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        /*
         * glFrustum must receive positive values for the near and far
         * clip planes ( arguments 5 and 6 ).
         */
        glFrustum(fleft*zoomFactor, fright*zoomFactor,
                fbottom*zoomFactor, ftop*zoomFactor,
                -zNear, -zFar);
    }

    /* Redraw the screen */
    display();
}

void endSubdiv(int status) {
  printf("\nQuitting subdivision program.\n\n");
  fflush(stdout);

  exit(status);
}

/* end of subdiv.cpp */
