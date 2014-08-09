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
#ifndef TEXT_H
#define TEXT_H

#include "renderable.h"
#include "point.h"
#include <string>

class text: public renderable
{
    private:
        point       m_origin;
        std::string m_text;
        float       m_size;
    public:
        text(const std::string& message=std::string(), float size=1);
        text(const point &origin_, const std::string &message=std::string(), float size=1);
        text& origin(const point& origin_);
        const point& origin() const;
        text& size(float size_);
        float size() const;
        text& message(std::string& text);
        const std::string& message() const;
        ~text() = default;
        void render() const;
};

#endif // TEXT_H
