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
#ifndef CIRCLE_H
#define CIRCLE_H
#include "renderable.h"
#include "point.h"
#include "line.h"
class circle: public renderable
{
    private:
        point m_origin;
        float m_radius;
        int   m_shift;
    public:
        circle(const point &origin = point(), float r = 0);
        circle& origin(const point &origin);
        const point& origin() const;
        circle& radius(float r);
        float radius() const;
        void render() const;
        ~circle() = default;
        line getLineSegment(int i) const;
        void shift(int i);
        int shift() const;
};

#endif // CIRCLE_H
