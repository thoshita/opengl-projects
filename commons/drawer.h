//
// Created by Turfa Auliarachman on 25/03/18.
//

#ifndef OPENGL_2_DRAWER_H
#define OPENGL_2_DRAWER_H

#include <GLFW/glfw3.h>

typedef GLfloat* Vertices;

Vertices combineVertices(Vertices a, GLint n_a, Vertices b, GLint n_b);
Vertices createCircle(GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, GLfloat radius, GLint numberOfSides);
Vertices createTriangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat r, GLfloat g, GLfloat b);


#endif //OPENGL_2_DRAWER_H
