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
#ifndef GOL_H
#define GOL_H

#include "render/window.h"
#include "render/point.h"
#include <vector>

constexpr float MAX_ZOOM = 150.0f;

class gol: public window {
    private:
        float m_view_x = 0.0, m_view_y = 0.0, m_zoom = 1.0;
        int m_btn_x = -1, m_btn_y = -1;

        point m_mouse;
        point m_mouse_scene;

        int m_state = 0;

        point screen2scene(const point& p);

    public:
        gol();

        void onResize(int w, int h);
        void onScroll(int dir);
        void onMouse(int btn, int state, int x, int y);
        void onMouseMotion(int x, int y);
        void onKeyboard(int key);
        void onRender();
};

#endif // GOL_H
