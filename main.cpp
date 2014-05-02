/**
    game-of-life
    Copyright (C) 2014  Luca Béla Palkovics <luca.bela.palkovics@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
  */
#include <iostream>
#include <GL/freeglut.h>

#include "line.h"
#include "creature.h"

using namespace std;

int win_width;
int win_height;

float view_x = 0;
float view_y = 0;
float zoom = 1.0;

void update() {
    glViewport(0, 0, win_width, win_height);
    glLoadIdentity();
    glScalef(zoom/win_width, zoom/win_height, 1);
    glTranslatef(view_x, view_y, 0);
}

void resize(int width, int height) {
    win_width = width;
    win_height = height;
    update();
}

int m_x = -1;
int m_y = -1;
void mouse(int button, int state, int x, int y) {
    // Wheel reports as button 3(scroll up) and button 4(scroll down)
    if ((button == 3) || (button == 4)) {
        if (state == GLUT_UP) return; // Disregard redundant GLUT_UP events
        if (button == 3) {
            zoom *= 1.1;
        } else {
            zoom /= 1.1;
        }
        update();
    } else if (button == 0) {
        if (state == GLUT_DOWN) {
            m_x = x;
            m_y = y;
        } else if (state == GLUT_UP) {
            m_x = -1;
            m_y = -1;
        }
    }
}

void mouseMotion(int x, int y) {
    if (m_x != -1 && m_y != -1) {
        view_x = view_x + (x - m_x)*2/zoom;
        view_y = view_y + (m_y - y)*2/zoom;
        m_x = x;
        m_y = y;
        update();
    }
}

void mouseWheel(int button, int dir, int x, int y) {
    //not working anymore in freeglut ?
    if (dir > 0) {
        zoom *= 1.1;
    } else {
        zoom /= 1.1;
    }
    update();
}

void render(void) {
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static creature test;
    static bool init = true;
    if (init) {
        init = false;
        test.attach(creature::LIMB, 0, 0, 50);
        test.attach(creature::LIMB, 0, 4, 40);
        test.attach(creature::LIMB, 1, 4, 30);
        test.attach(creature::LIMB, 2, 3, 15);
        test.attach(creature::LIMB, 2, 5, 15);
        for(int j = 0; j < 5; ++j)
        for(int i = 0; i < 9; ++i) {
            test.attach(creature::SENSOR, j, i, 50/(1+j));
        }
        test.update();
    }
    test.render();

    glDisable( GL_DEPTH_TEST ) ; // also disable the depth test so renders on top

    glRasterPos2f( 0,0 );
    const char buf[] = "Oh hello";
    const char * p = buf ;
    glPushMatrix();
    glScalef(0.2,0.2,1); //font size
    glLineWidth(50);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    do glutStrokeCharacter( GLUT_STROKE_ROMAN, *p ); while( *(++p) ) ; //GLUT_STROKE_MONO_ROMAN fix width
    glPopMatrix();
    p = buf ;
    glPushMatrix();
    glScalef(0.2,0.2,1); //font size
    glLineWidth(2);
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    do glutStrokeCharacter( GLUT_STROKE_ROMAN, *p ); while( *(++p) ) ; //GLUT_STROKE_MONO_ROMAN fix width
    glPopMatrix();
    glEnable( GL_DEPTH_TEST ) ; // Turn depth testing back on

    glutSwapBuffers();
    glutPostRedisplay();
}


int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitContextVersion(1, 4);
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow("game-of-life");
    glutReshapeFunc(resize);
    glutDisplayFunc(render);
    glutMouseFunc(mouse);
    glutMouseWheelFunc(mouseWheel);
    glutMotionFunc(mouseMotion);
    glutMainLoop();
    return 0;
}

