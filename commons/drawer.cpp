//
// Created by Turfa Auliarachman on 25/03/18.
//

#include <math.h>
#include "drawer.h"

void combineVertices(Vertices &a, GLint &n_a, Vertices b, GLint n_b, vector<int> &start_of_section) {
    a = combineVertices(a, n_a, b, n_b);
    start_of_section.push_back(n_a);
    n_a += n_b;
}

Vertices combineVertices(Vertices a, GLint n_a, Vertices b, GLint n_b) {
    auto new_arr = new GLfloat[(n_a + n_b) * 5];

    for(int i = 0; i < n_a; i++) {
        new_arr[i*5] = a[i*5];
        new_arr[i*5 + 1] = a[i*5 + 1];
        new_arr[i*5 + 2] = a[i*5 + 2];
        new_arr[i*5 + 3] = a[i*5 + 3];
        new_arr[i*5 + 4] = a[i*5 + 4];
    }

    for(int i = 0; i < n_b; i++) {
        new_arr[(n_a + i)*5] = b[i*5];
        new_arr[(n_a + i)*5 + 1] = b[i*5 + 1];
        new_arr[(n_a + i)*5 + 2] = b[i*5 + 2];
        new_arr[(n_a + i)*5 + 3] = b[i*5 + 3];
        new_arr[(n_a + i)*5 + 4] = b[i*5 + 4];
    }

    delete a;
    delete b;

    return new_arr;
}

Vertices createCircle(GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, GLfloat radius, GLint numberOfSides) {
    int numberOfVertices = numberOfSides + 2;

    GLfloat twicePi = static_cast<GLfloat>(2.0f * M_PI);

    GLfloat circleVerticesX[numberOfVertices];
    GLfloat circleVerticesY[numberOfVertices];
    GLfloat circleVerticesR[numberOfVertices];
    GLfloat circleVerticesG[numberOfVertices];
    GLfloat circleVerticesB[numberOfVertices];

    circleVerticesX[0] = x;
    circleVerticesY[0] = y;
    circleVerticesR[0] = r;
    circleVerticesG[0] = g;
    circleVerticesB[0] = b;

    for ( int i = 1; i < numberOfVertices; i++ ) {
        circleVerticesX[i] = x + ( radius * cos( i *  twicePi / numberOfSides ) );
        circleVerticesY[i] = y + ( radius * sin( i * twicePi / numberOfSides ) );
        circleVerticesR[i] = r;
        circleVerticesG[i] = g;
        circleVerticesB[i] = b;
    }

    auto allCircleVertices = new GLfloat[numberOfVertices * 5];

    for ( int i = 0; i < numberOfVertices; i++ )
    {
        allCircleVertices[i * 5] = circleVerticesX[i];
        allCircleVertices[( i * 5 ) + 1] = circleVerticesY[i];
        allCircleVertices[( i * 5 ) + 2] = circleVerticesR[i];
        allCircleVertices[( i * 5 ) + 3] = circleVerticesG[i];
        allCircleVertices[( i * 5 ) + 4] = circleVerticesB[i];
    }

    return allCircleVertices;
}

Vertices createTriangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat r, GLfloat g,
                        GLfloat b) {
    GLfloat * triangle = new GLfloat[15];
    triangle[0] = x1;
    triangle[1] = y1;

    triangle[5] = x2;
    triangle[6] = y2;

    triangle[10] = x3;
    triangle[11] = y3;

    triangle[2] = triangle[7] = triangle[12] = r;
    triangle[3] = triangle[8] = triangle[13] = g;
    triangle[4] = triangle[9] = triangle[14] = b;

    return triangle;
}
