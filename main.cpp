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
#include "circle.h"

using namespace std;

void resize(int width, int height)
{
    glViewport(0, 0, width, height);
    glLoadIdentity();
    glScalef(1.0/width, 1.0/height, 1);
}

void render(void)
{
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto sl  = point(-100,-100);
    sl.colorR(1);
    line a(sl, point(100, 100));

    circle cr(sl, 100);
    cr.render();


    a.render();

    //TEXT
    glMatrixMode( GL_PROJECTION ) ;
    glPushMatrix() ; // save
    glLoadIdentity();// and clear
    glMatrixMode( GL_MODELVIEW ) ;
    glPushMatrix() ;
    glLoadIdentity() ;

    glDisable( GL_DEPTH_TEST ) ; // also disable the depth test so renders on top

    glRasterPos2f( 0,0 ) ; // center of screen. (-1,0) is center left.
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    const char buf[] = "Oh hello";
    const char * p = buf ;
    do glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, *p ); while( *(++p) ) ;

    glEnable( GL_DEPTH_TEST ) ; // Turn depth testing back on

    glMatrixMode( GL_PROJECTION ) ;
    glPopMatrix() ; // revert back to the matrix I had before.
    glMatrixMode( GL_MODELVIEW ) ;
    glPopMatrix() ;

    glutSwapBuffers();
    glutPostRedisplay();
}


int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(1, 4);
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow("game-of-life");
    glutReshapeFunc(resize);
    glutDisplayFunc(render);
    glutMainLoop();
    return 0;
}

