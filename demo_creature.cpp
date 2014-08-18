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
#include "demo_creature.h"
#include <cmath>
#include "algorithm/algorithm.h"

demo_creature::demo_creature()
{
    //BUILD CREATURE

    int points = 16;
    int size = 256;
    for(int i = 0; i < points; ++i) {
        float x = sin(-2*i*M_PI/points)*size;
        float y = cos(-2*i*M_PI/points)*size;
        hull.push_back(point(x, y));
    }

    for(int i = 0; i < points; i+=points/8) {
        muscle.push_back(Muscle{
                             i,
                             (i+points/8)%points,
                             0,
                             0});
    }

    muscle.push_back(Muscle{
                         0,
                         points/2,
                         0,
                         0});
/*    muscle.push_back(Muscle{
                         2,
                         3,
                         0,
                         0});*/

    global.x(1024);

    init();
}

void demo_creature::update() {
    //update ai:
    static int f = 0;

    for(auto &m : muscle) {
        m.active = 3*cos(f/100.0);
    }


    f += 1;

    creature::update();
}
