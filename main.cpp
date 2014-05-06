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

#include "text_screen.h"
#include <cmath>
#include "window.h"

using namespace std;

constexpr float MAX_ZOOM = 150.0f;

class gol: public window {
    private:
        float m_view_x = 0.0, m_view_y = 0.0, m_zoom = 1.0;
        int m_btn_x = -1, m_btn_y = -1;

    public:
        gol(): window("game-of-life") {}

        void onResize(int w, int h) {
            glViewport(0, 0, w, h);
            glLoadIdentity();
            glScalef(m_zoom/w, m_zoom/h, 1);
            glTranslatef(m_view_x, m_view_y, 0);
        }
        void onScroll(int dir) {
            if (dir > 0) {
                m_zoom *= 1.1;
            } else {
                m_zoom /= 1.1;
            }
            m_zoom = min(MAX_ZOOM, m_zoom);
            onResize(width(), height());
        }
        void onMouse(int btn, int state, int x, int y) {
            if (btn == 0) {
                if (state == GLUT_DOWN) {
                    m_btn_x = x;
                    m_btn_y = y;
                } else if (state == GLUT_UP) {
                    m_btn_x = -1;
                    m_btn_y = -1;
                }
            }
        }
        void onMouseMotion(int x, int y) {
            if (m_btn_x != -1 && m_btn_y != -1) {
                m_view_x = m_view_x + (x - m_btn_x)*2/m_zoom;
                m_view_y = m_view_y + (m_btn_y - y)*2/m_zoom;
                m_btn_x = x;
                m_btn_y = y;
                onResize(width(), height());
            }
        }

        void onRender() {
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

            static text_screen ts("test");
            static text t("test");
            ts.origin(point(0,-0.8)).size(0.001).colorR(1);
            t.origin(point(0,0)).size(0.2).colorB(1);
            ts.render();
            t.render();
        }
};

int main() {
    gol main;
    main.loop();
}

