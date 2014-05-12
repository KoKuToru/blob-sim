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
#include <limits>
#include "window.h"
#include "algorithm.h"

using namespace std;

template<typename T>
class enumerate_iter: public std::iterator<std::input_iterator_tag, std::pair<int, typename T::value_type &>> {
    private:
        size_t index = 0;
        T      iter;
    public:
        enumerate_iter(size_t index, T iter): index(index), iter(iter) {}
        enumerate_iter& operator++() {
            ++iter;
            ++index;
            return *this;
        }
        enumerate_iter operator++(int) {
            enumerate_iter tmp(*this);
            operator++();
            return tmp;
        }
        bool operator==(const enumerate_iter& rhs) {
            return iter == rhs.iter;
        }
        bool operator!=(const enumerate_iter& rhs) {
            return iter != rhs.iter;
        }
        std::pair<int, typename T::value_type &> operator*() {
            return std::pair<int, typename T::value_type &>(index, *iter);
        }
};

template<typename T>
class enumerate_helper {
    private:
        enumerate_iter<T> m_begin;
        enumerate_iter<T> m_end;
    public:
        enumerate_helper(size_t index_start, T begin_, size_t index_end, T end_): m_begin(index_start, begin_), m_end(index_end, end_) {

        }
        enumerate_iter<T> begin () const
        {
            return m_begin;
        }

        enumerate_iter<T> end () const
        {
            return m_end;
        }
};

template<typename T> enumerate_helper<typename T::iterator> enumerate(T &o) {
    return enumerate_helper<typename T::iterator>(0, o.begin(), o.size(), o.end());
}

constexpr float MAX_ZOOM = 150.0f;

class gol: public window {
    private:
        float m_view_x = 0.0, m_view_y = 0.0, m_zoom = 1.0;
        int m_btn_x = -1, m_btn_y = -1;

        std::vector<line> m_lines;
        point m_mouse;
        point m_mouse_scene;

        int m_state = 0;
        int m_split_line = -1;
        std::string m_status = "Press SPACE to enter edit mode";

        point screen2scene(const point& p) {
            return point((p.x()-width()/2)*2/m_zoom-m_view_x, (p.y()-height()/2)*(-2)/m_zoom-m_view_y);
        }

        int m_best_line = -1;

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
            m_mouse = point(x, y);
            m_mouse_scene = screen2scene(m_mouse);

            if (btn == 0) {
                if (m_state == 1 && state == GLUT_DOWN) {
                    if (m_lines.empty()) {
                        m_lines.push_back(line(m_mouse_scene, m_mouse_scene));
                    } else {
                        m_lines.back().target(m_mouse_scene);
                        m_lines.push_back(line(m_mouse_scene, m_mouse_scene));
                    }
                } else if (m_state == 2) {
                    m_state = 0;
                } else {
                    if (state == GLUT_DOWN) {
                        m_btn_x = x;
                        m_btn_y = y;
                    } else if (state == GLUT_UP) {
                        m_btn_x = -1;
                        m_btn_y = -1;
                    }
                }
            }
        }
        void onMouseMotion(int x, int y) {
            m_mouse = point(x, y);
            m_mouse_scene = screen2scene(m_mouse);

            if (m_btn_x != -1 && m_btn_y != -1) {
                m_view_x = m_view_x + (x - m_btn_x)*2/m_zoom;
                m_view_y = m_view_y + (m_btn_y - y)*2/m_zoom;
                m_btn_x = x;
                m_btn_y = y;
                onResize(width(), height());
            }
            if (m_state == 1 && !m_lines.empty()) {
                m_lines.back().target(m_mouse_scene);
            }
            if (m_state == 2) {
                m_lines[m_split_line  ].target(m_mouse_scene);
                m_lines[m_split_line+1].origin(m_mouse_scene);
            }
        }
        void onKeyboard(int key) {
            std::cout << "got key: " << key << std::endl;
            switch(key) {
                case ' ': //start adding points
                    m_status = "Press ENTER to stop edit mode";
                    m_state = 1;
                    break;
                case 13: //stop adding points
                    m_state = 0;
                    m_status = "Press SPACE to enter edit mode";
                    if (!m_lines.empty()) {
                        m_lines.pop_back();
                        if (!m_lines.empty()) {
                            m_lines.push_back(line(m_lines.back().target(), m_lines.front().origin()));
                        }
                    }
                case 8: //remove last
                    if (m_state == 1) {
                        if (!m_lines.empty()) {
                            m_lines.pop_back();
                        }
                    }
                    break;
                case 's': //split
                    if (m_state == 0) {
                        if (m_best_line >= 0) {
                            m_state = 2;
                            m_split_line = m_best_line;
                            m_lines.insert(m_lines.begin()+m_best_line+1, line(m_mouse_scene, m_lines[m_split_line].target()));
                            m_lines[m_split_line].target(m_mouse_scene);
                        }
                    }
            }
        }

        void onRender() {
            //update
            int   best_line = -1;
            float best_distance = std::numeric_limits<float>::infinity();
            point best_point;
            for(auto e: enumerate(m_lines)) {
                float dis;
                point p;
                std::tie(dis, p) = algorithm::distance(e.second, m_mouse_scene);
                if (dis < best_distance) {
                    best_distance = dis;
                    best_line = e.first;
                    best_point = p;
                }
            }
            m_best_line = best_line;
            for(line &l: m_lines) {
                //render line
                l.render();
                //render endpoints
                circle(l.origin(), 2).render();
                circle(l.target(), 2).render();
            }
            if (best_line >= 0) {
                circle(best_point, 4).render();
            }

            glDisable( GL_DEPTH_TEST ) ; // also disable the depth test so renders on top

            text_screen text(m_status);
            text.origin(point(10, 10)).size(0.2).colorR(1);
            text.render();
        }
};

int main() {
    gol main;
    main.loop();
}

