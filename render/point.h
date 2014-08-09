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
#ifndef POINT_H
#define POINT_H
#include "renderable.h"
class point: public renderable
{
    private:
        float m_x;
        float m_y;
        int   m_layer_index;

    public:
        point(float x = 0, float y = 0, int layer_index = 0);
        point& origin(float x, float y, int layer_index = 0);
        float x() const;
        point& x(float x_);
        float y() const;
        point& y(float y_);
        void render() const;
        point& layer(int layer_index);
        int layer() const;
        ~point() = default;
};

#endif // POINT_H
