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
#ifndef CREATURE_H
#define CREATURE_H
#include <map>
#include <vector>
#include "../render/point.h"
class creature
{
    public:
        enum part_type: unsigned char {
            LIMB,
            SENSOR
        };

    protected:
        std::vector<point> hull;
        std::vector<point> hull_new;

        std::vector<float> hull_length;
        std::vector<point> hull_force;

        point motion;

        struct Muscle {
            int from;
            int to;
            float length;
            float active;
        };

        std::vector<Muscle> muscle;
        float mass;

        point global;
        point gloabl_motion;

        void physicsMuscles();
        void physicsHullLength();
        void physicsHullArea();
        void physicsHullAngle();
        void physicsSelfIntersection();
        void physicsIntersection();

        void applyForces();

    public:

        void init();

        virtual void update();

        void render() const ;
};

#endif // CREATURE_H
