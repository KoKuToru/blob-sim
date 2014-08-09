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
#include "gol.h"
#include <iostream>
#include "demo_creature.h"

point gol::screen2scene(const point& p) {
    return point((p.x()-width()/2)*2/m_zoom-m_view_x, (p.y()-height()/2)*(-2)/m_zoom-m_view_y);
}

gol::gol(): window("game-of-life") {}

void gol::onResize(int w, int h) {
    glViewport(0, 0, w, h);
    glLoadIdentity();
    glScalef(m_zoom/w, m_zoom/h, 1);
    glTranslatef(m_view_x, m_view_y, 0);
}

void gol::onScroll(int dir) {
    if (dir > 0) {
        m_zoom *= 1.1;
    } else {
        m_zoom /= 1.1;
    }
    m_zoom = std::min(MAX_ZOOM, m_zoom);
    onResize(width(), height());
}

void gol::onMouse(int btn, int state, int x, int y) {
    m_mouse = point(x, y);
    m_mouse_scene = screen2scene(m_mouse);

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

void gol::onMouseMotion(int x, int y) {
    m_mouse = point(x, y);
    m_mouse_scene = screen2scene(m_mouse);

    if (m_btn_x != -1 && m_btn_y != -1) {
        m_view_x = m_view_x + (x - m_btn_x)*2/m_zoom;
        m_view_y = m_view_y + (m_btn_y - y)*2/m_zoom;
        m_btn_x = x;
        m_btn_y = y;
        onResize(width(), height());
    }
}

void gol::onKeyboard(int key) {
    std::cout << "got key: " << key << std::endl;
}

void gol::onRender() {
    static demo_creature test;
    for(int i = 0; i < 10; ++i) test.update();
    test.render();
}
