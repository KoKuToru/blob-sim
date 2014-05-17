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
#include <algorithm>

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

        point m_measure_start;
        point m_measure_stop;
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
                } else if (m_state == 3) {
                    m_measure_stop = m_mouse_scene;
                    m_state = 0;
                    m_status = "Press ENTER to stop edit mode";
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
            if (m_state == 3) {
                m_measure_stop = m_mouse_scene;
            }

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
                case 'm': //measure
                    m_status = "Click to stop measure";
                    m_measure_start = m_mouse_scene;
                    m_measure_stop  = m_mouse_scene;
                    m_state = 3;
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
            std::vector<point> points; //m_lines should be replaced with a list of points..
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
            line* last = &(m_lines.back());
            static vector<float> stats;
            stats.clear();
            for(line &l: m_lines) {
                points.push_back(l.origin());
                //render line
                l.render();
                //intersection test
                for(line &l2: m_lines) {
                    if (&l == &l2) continue;
                    bool res; point p;
                    tie(res, p) = algorithm::intersect(l, l2);
                    if (res) {
                        p.colorR(1);
                        circle(p, 5).render();
                    }
                }
                //render endpoints
                circle(l.origin(), 2).render();
                circle(l.target(), 2).render();
                //angle
                float alpha = algorithm::angle(*last, l)*algorithm::side(*last, l.target());
                //render
                string alpha_text = to_string(/*180-*/alpha*360/(2*M_PI));
                text txt(l.origin(), alpha_text, 0.15);
                txt.colorB(1);
                txt.render();

                //normal
                line n = algorithm::normal(l);
                n.colorR(1);
                n.render();

                /*iterate normal*/
                float dx = n.target().x() - n.origin().x();
                float dy = n.target().y() - n.origin().y();
                dx *= 200;
                dy *= 200;
                float sx = l.target().x() - l.origin().x();
                float sy = l.target().y() - l.origin().y();
                float s = sqrt(sx*sx + sy*sy);

                text(n.origin(), "len="+std::to_string(s), 0.1).render();

                last = &l;
            }
            if (best_line >= 0) {
                circle(best_point, 4).render();
            }

            point center  = algorithm::center(points);
            center.colorR(1).colorB(1);
            circle(center,7).render();
            text(center, "center" ,0.2).render();

            point centroid = algorithm::centroid(points);
            centroid.colorR(1).colorG(1);
            circle(centroid,7).render();
            text(centroid, "centroid" ,0.2).render();

            text_screen(point(10, height()-20), "Area: "+std::to_string(algorithm::area(points)), 0.1).render();

            text_screen text(m_status);
            text.origin(point(10, 10)).size(0.2).colorR(1);
            text.render();

            if (m_state == 3) {
                line(m_measure_start, m_measure_stop).render();
                circle(m_measure_start, 3).render();
                circle(m_measure_stop, 3).render();
            }

            float w = 0;
            float h = 0;
            std::tie(w, h) = algorithm::approximateWidthHeight(points);
            text = text_screen("Approximated Rect: " + std::to_string(w) + "x" + std::to_string(h));
            text.origin(point(10, 40)).size(0.2).colorR(1);
            text.render();

            float s = algorithm::distance(m_measure_start, m_measure_stop);
            text = text_screen("Measured: " + std::to_string(s));
            text.origin(point(10, 70)).size(0.2).colorR(1);
            text.render();
        }
};

int main() {
    gol main;
    main.loop();
}

