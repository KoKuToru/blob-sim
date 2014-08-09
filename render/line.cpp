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
#include "line.h"
#include "circle.h"
#include <GL/freeglut.h>
#include <cmath>

line::line(const point &origin, const point &target):
    m_origin(origin),
    m_target(target)
{
}

const point& line::origin() const {
    return m_origin;
}

line& line::origin(const point &origin_) {
    m_origin = origin_;
    return *this;
}

const point& line::target() const {
    return m_target;
}

line& line::target(const point &target_) {
    m_target = target_;
    return *this;
}


void line::render() const {
    glBegin(GL_LINES);
    glColor4f(m_origin.colorR(), m_origin.colorG(), m_origin.colorB(), m_origin.colorA());
    glVertex3f(m_origin.x(), m_origin.y(), m_origin.layer());
    glColor4f(m_target.colorR(), m_target.colorG(), m_target.colorB(), m_target.colorA());
    glVertex3f(m_target.x(), m_target.y(), m_target.layer());
    glEnd();
}
