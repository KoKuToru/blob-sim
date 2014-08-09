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
#include "circle.h"
#include "line.h"

circle::circle(const point &origin, float r):
    m_origin(origin),
    m_radius(r),
    m_shift(0)
{
}

circle& circle::origin(const point &origin){
    m_origin = origin;
    return *this;
}

const point& circle::origin() const {
    return m_origin;
}

circle& circle::radius(float r) {
    m_radius = r;
    return *this;
}

float circle::radius() const {
    return m_radius;
}

line circle::getLineSegment(int i) const {
    i += m_shift;
    i %= 8;

    //9 segments render
    constexpr float st[] = {
        0, //sin(0)
        0.70710678118, //sin(PI/4)
        1,
        0.70710678118,
        0,
        -0.70710678118,
        -1,
        -0.70710678118,
        0
    };

    constexpr float ct[] = {
        1,
        0.70710678118,
        0,
        -0.70710678118,
        -1,
        -0.70710678118,
        0,
        0.70710678118,
        1
    };

    auto a = point(m_origin.x() + st[i+0]*m_radius,m_origin.y() + ct[i+0]*m_radius);
    auto b = point(m_origin.x() + st[i+1]*m_radius,m_origin.y() + ct[i+1]*m_radius);
    a.colorR(m_origin.colorR()).colorG(m_origin.colorG()).colorB(m_origin.colorB()).colorA(m_origin.colorA());
    b.colorR(m_origin.colorR()).colorG(m_origin.colorG()).colorB(m_origin.colorB()).colorA(m_origin.colorA());

    auto m = line(a, b);

    return m;
}

void circle::render() const {
    for(int i = 0; i < 8; ++i) {
        getLineSegment(i).render();
    }
}

void circle::shift(int i) {
    m_shift = i;
}

int circle::shift() const {
    return m_shift;
}
