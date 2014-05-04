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

std::tuple<bool, float> line::intersect(const line &other) const {

    //from http://stackoverflow.com/a/1968345
    double p0_x = m_origin.x();
    double p0_y = m_origin.y();
    double p1_x = m_target.x();
    double p1_y = m_target.y();
    double p2_x = other.origin().x();
    double p2_y = other.origin().y();
    double p3_x = other.target().x();
    double p3_y = other.target().y();

    double s1_x = p1_x - p0_x;
    double s1_y = p1_y - p0_y;
    double s2_x = p3_x - p2_x;
    double s2_y = p3_y - p2_y;

    if ((-s2_x * s1_y + s1_x * s2_y) == 0) {
        return std::make_tuple(false, 0.0f);
    }

    double s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    double t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    double w = 0.1;
    if (s >= 0-w && s <= 1+w && t >= 0-w && t <= 1+w)
    {
        // Collision detected
        return std::make_tuple(true, t);
    }
    return std::make_tuple(false, 0.0f);
}

std::tuple<bool, float, int> line::intersect(const circle& circle) const {
    std::tuple<bool, float, int> best = std::make_tuple(false, 0.0f, 0);
    for(int i = 0; i < 8; ++i) {
        auto res = std::tuple_cat(intersect(circle.getLineSegment(i)), std::make_tuple(i));
        if (std::get<0>(best) == false) {
            best = res;
        } else if (std::get<1>(best) > std::get<1>(res)) {
            best = res;
        }
    }
    return best;
}
