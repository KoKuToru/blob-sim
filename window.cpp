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
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow("game-of-life");
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
