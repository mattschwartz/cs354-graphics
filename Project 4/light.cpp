/******************************************************************/
/*         Lighting functions                                     */
/*                                                                */
/* Group Members: Matt schwartz                                   */
/******************************************************************/

#ifdef _WIN32
#include <windows.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <math.h>
#include "light.h"
#include "raytrace.h"

#define EPSILON 0.001

Light *lightSources[10];

//struct light_source *lightSources[10];
int numLightSources = 0;

Light::Light(Color *color, Point *position, GLfloat specular, GLfloat diffuse, GLfloat intensity) {
    this->color = color;
    this->position = position;
    this->specular = specular;
    this->diffuse = diffuse;
    this->intensity = intensity;
} // constructor

void Light::getReflectVector(Point *start, vector normal, vector *reflectedVector) {
    GLfloat n_dot_l;
    vector toLight;

    Polygon::calculateDirection(start, lightSources[0]->getPosition(), &toLight);

    Polygon::normalize(&toLight);
    Polygon::normalize(&normal);

    n_dot_l = Polygon::dot(normal, toLight);

    reflectedVector->x = 2 * n_dot_l * normal.x - toLight.x;
    reflectedVector->y = 2 * n_dot_l * normal.y - toLight.y;
    reflectedVector->z = 2 * n_dot_l * normal.z - toLight.z;
} // getReflectVector

/**
 * Calculation is returned in @reflectVector
 * @param normal
 * @param light
 * @param relectVector
 * @return 
 */
void Light::calculateReflectVector(vector normal, vector light, vector* reflectVector) {
    GLfloat n_dot_l;

    Polygon::normalize(&normal);
    Polygon::normalize(&light);

    n_dot_l = Polygon::dot(normal, light);

    reflectVector->x = 2 * n_dot_l * normal.x - light.x;
    reflectVector->y = 2 * n_dot_l * normal.y - light.y;
    reflectVector->z = 2 * n_dot_l * normal.z - light.z;
} // calculateReflectVector

Color *Light::getColor() {
    return color;
} // getColor

Point *Light::getPosition() {
    return position;
} // getPosition

GLfloat Light::getSpecular() {
    return specular;
} // getSpecular

GLfloat Light::getDiffuse() {
    return diffuse;
} // getDiffuse

GLfloat Light::getIntensity() {
    return intensity;
} // getIntensity

void addLightSource(Light *lightToAdd) {
    lightSources[numLightSources++] = lightToAdd;
} // addLightSource

/* LIGHTING CALCULATIONS */

/* shade */
/* color of point p with normal vector n and material m returned in c */

/* in is the direction of the incoming ray and d is the recusive depth */
void Light::shade(Point* p, vector* normal, Material* m, vector* in, Color* color, int d) {
    vector toLight;
    vector perfect_reflect_vector;
    vector to_viewer;
    GLfloat l_dot_n; // angle between normal and light vector
    GLfloat r_dot_v; // angle between perfect reflect and viewer vectors
    Color *total_illumination = Polygon::makeColor(0, 0, 0);
    ray r;
    Point firstHitPoint; // first intersection point
    Point p_1;

    // calculate ambient
    color->r = m->ambient;// * m->r;
    color->g = m->ambient;// * m->g;
    color->b = m->ambient;// * m->b;

    // calculate the vector from the point to the viewer
    Polygon::calculateDirection(p, getViewPoint(), &to_viewer);
    Polygon::normalize(&to_viewer);

    // go through the light sources, adding up each's benefit for the point
    for (int i = 0; i < numLightSources; i++) {
        // get the direction of the point to the light source
        Polygon::calculateDirection(p, lightSources[i]->getPosition(), &toLight);
        Polygon::normalize(&toLight);

        // determine if point is in shadow
        p_1.x = p->x + EPSILON * toLight.x;
        p_1.y = p->y + EPSILON * toLight.y;
        p_1.z = p->z + EPSILON * toLight.z;

        r.start = &p_1;
        r.dir = &toLight;
        vector tempVector;
        Material *tempMaterial;
        firstHitPoint.w = 0;
        firstHit(&r, &firstHitPoint, &tempVector, &tempMaterial);

        // object is in shadow
        if (firstHitPoint.w != 0 && !(p_1.x == firstHitPoint.x && p_1.y == firstHitPoint.y && p_1.z == firstHitPoint.z)) {
            color->r *= 0.25;
            color->g *= 0.25;
            color->b *= 0.25;
            continue;
        } // if


        l_dot_n = Polygon::dot(toLight, *normal);

        if (l_dot_n < 0) {
            l_dot_n = 0;
        } // if

        // calculate the perfect reflect vector
        Light::calculateReflectVector(*normal, toLight, &perfect_reflect_vector);
        Polygon::normalize(&perfect_reflect_vector);

        // dot product between perfect reflect vector and vector to the viewer
        r_dot_v = Polygon::dot(perfect_reflect_vector, to_viewer);

        if (r_dot_v < 0) {
            r_dot_v = 0;
        } // if

        total_illumination->r += lightSources[i]->getColor()->r * m->diffuse * l_dot_n * lightSources[i]->getDiffuse() + // diffuse component
                m->specular * pow(r_dot_v, m->shininess) * lightSources[i]->getSpecular(); // specular component
        
        total_illumination->g += lightSources[i]->getColor()->g * m->diffuse * l_dot_n * lightSources[i]->getDiffuse() + // diffuse component
                m->specular * pow(r_dot_v, m->shininess) * lightSources[i]->getSpecular(); // specular component
        
        total_illumination->b += lightSources[i]->getColor()->b * m->diffuse * l_dot_n * lightSources[i]->getDiffuse() + // diffuse component
                m->specular * pow(r_dot_v, m->shininess) * lightSources[i]->getSpecular(); // specular component
    } // for

    color->r += total_illumination->r * m->r;
    color->g += total_illumination->g * m->g;
    color->b += total_illumination->b * m->b;

    // clamp color values to 1.0
    if (color->r > 1.0) color->r = 1.0;
    if (color->g > 1.0) color->g = 1.0;
    if (color->b > 1.0) color->b = 1.0;
} // shade
