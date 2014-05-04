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
#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <GL/freeglut.h>

class renderable
{
    protected:
        float m_color_r;
        float m_color_g;
        float m_color_b;
        float m_color_a;
    public:
        renderable();
        virtual ~renderable() = default;
        virtual void render() const = 0;
        float colorR() const;
        float colorG() const;
        float colorB() const;
        float colorA() const;
        renderable& colorR(float color_r);
        renderable& colorG(float color_g);
        renderable& colorB(float color_b);
        renderable& colorA(float color_a);
};

#endif // RENDERABLE_H
