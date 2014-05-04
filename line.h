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
#ifndef LINE_H
#define LINE_H
#include "renderable.h"
#include "point.h"
#include <tuple>

class circle;
class line: public renderable
{
    private:
        point m_origin;
        point m_target;
    public:
        line(const point &origin = point(), const point &target = point());
        const point& origin() const;
        line& origin(const point &origin_);
        const point& target() const;
        line& target(const point &target_);
        void render() const;
        ~line() = default;

        std::tuple<bool, float> intersect(const line &other) const;
        std::tuple<bool, float, int> intersect(const circle& circle) const;
};

#endif // LINE_H
