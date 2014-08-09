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
#include "window.h"
#include <GL/freeglut.h>
#include <functional>

window* window::s_instance = nullptr;

window::window(const std::string& title, int w, int h):
    m_width(w),
    m_height(h)
{
    if (s_instance != nullptr) {
        throw std::string("Can be only used once !");
    }
    s_instance = this;
    int argc = 1;
    const char* argv[] = {"dummy"};
    glutInit(&argc, (char**)argv);
    glutInitContextVersion(1, 4);
    glutInitWindowSize(w, h);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
    glutCreateWindow(title.c_str());
    glutReshapeFunc([](int w, int h){
        window::s_instance->m_width = w;
        window::s_instance->m_height = h;
        window::s_instance->onResize(w, h);
    });
    glutDisplayFunc([](){
        glClearColor(1,1,1,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        window::s_instance->onRender();
        glutSwapBuffers();
        glutPostRedisplay();
    });
    glutMouseFunc([](int button, int state, int x, int y) {
        if ((button == 3) || (button == 4)) {
            if (state == GLUT_UP) {
                return;
            }
            window::s_instance->onScroll((button == 3)?1:-1);
        } else {
            window::s_instance->onMouse(button, state, x, y);
        }
    });
    glutMotionFunc([](int x, int y) {
        window::s_instance->onMouseMotion(x, y);
    });
    glutPassiveMotionFunc([](int x, int y) {
        window::s_instance->onMouseMotion(x, y);
    });
    glutKeyboardFunc([](unsigned char key, int x, int y) {
        (void)x; (void)y;
        window::s_instance->onKeyboard(key);
    });
}

void window::loop() {
    glutMainLoop();
}

int window::width() const {
    return m_width;
}

int window::height() const {
    return m_height;
}
