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
#include "renderable.h"

renderable::renderable():
    m_color_r(),
    m_color_g(),
    m_color_b(),
    m_color_a()
{
}

float renderable::colorR() const {
    return m_color_r;
}

float renderable::colorG() const {
    return m_color_g;
}

float renderable::colorB() const {
    return m_color_b;
}

float renderable::colorA() const {
    return m_color_a;
}

renderable& renderable::colorR(float color_r) {
    m_color_r = color_r;
    return *this;
}

renderable& renderable::colorG(float color_g) {
    m_color_g = color_g;
    return *this;
}

renderable& renderable::colorB(float color_b) {
    m_color_b = color_b;
    return *this;
}

renderable& renderable::colorA(float color_a) {
    m_color_a = color_a;
    return *this;
}
