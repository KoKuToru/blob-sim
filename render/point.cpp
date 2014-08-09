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
#include "point.h"
#include <GL/freeglut.h>

point::point(float x, float y, int layer_index):
    m_x(x),
    m_y(y),
    m_layer_index(layer_index)
{
}

point& point::origin(float x, float y, int layer_index) {
    m_x = x;
    m_y = y;
    m_layer_index = layer_index;
    return *this;
}

void point::render() const {
    glBegin(GL_POINTS);
    glColor4f(m_color_r, m_color_g, m_color_b, m_color_a);
    glVertex2f(m_x, m_y);
    glEnd();
}

float point::x() const {
    return m_x;
}

point& point::x(float x_) {
    m_x = x_;
    return *this;
}

float point::y() const {
    return m_y;
}

point& point::y(float y_) {
    m_y = y_;
    return *this;
}

point& point::layer(int layer_index) {
    m_layer_index = layer_index;
    return *this;
}

int point::layer() const {
    return m_layer_index;
}
