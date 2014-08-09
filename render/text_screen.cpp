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
#include "text_screen.h"
#include "window.h"

void text_screen::render(){
    glPushMatrix();
    glLoadIdentity();
    glScalef(2.0/window::s_instance->m_width, 2.0/window::s_instance->m_height, 1);
    glTranslatef(-window::s_instance->m_width/2.0, -window::s_instance->m_height/2.0, 0);
    text::render();
    glPopMatrix();
}
