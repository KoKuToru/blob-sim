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
#ifndef WINDOW_H
#define WINDOW_H

#include <string>

class window
{
    private:
        friend class text_screen; //not so optimal
        int m_width;
        int m_height;
        static window* s_instance;

    public:
        window(const std::string &title, int w=640, int h=480);
        virtual ~window() = default;
        virtual void onRender() = 0;
        virtual void onResize(int w, int h) = 0;
        virtual void onScroll(int dir) = 0;
        virtual void onMouse(int btn, int state, int x, int y) = 0;
        virtual void onMouseMotion(int x, int y) = 0;
        virtual void onKeyboard(int key) = 0;
        void loop();

        int width() const;
        int height() const;
};

#endif // WINDOW_H
