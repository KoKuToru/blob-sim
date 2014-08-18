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
#include "creature.h"
#include "../render/circle.h"
#include <cmath>
#include <iostream>
#include "../algorithm/algorithm.h"

constexpr float spring_factor = 0.1;
constexpr float spring_factor_mass = 0.004;
constexpr float spring_factor_muscle = 0.02;
constexpr float friction = 0.01;

void creature::init() {
    mass = algorithm::area(hull);

    auto p_last = hull.back();
    for(auto p : hull) {
        hull_length.push_back(algorithm::distance(p_last, p)/2.0);
        p_last = p;
    }

    for(auto &mu : muscle) {
        mu.length = algorithm::distance(hull.at(mu.from), hull.at(mu.to))/1.5;
    }
}

/*
 * not thread safe
 * */
void creature::update() {
   physicsHullLength();
   physicsHullArea();
   physicsHullAngle();
   physicsMuscles();

   physicsSelfIntersection();
   physicsIntersection();

   applyForces();
}

void creature::render() const {
    static std::vector<point> render_hull;
    render_hull.clear();
    for(auto p : hull) {
        p.x(p.x()+global.x());
        p.y(p.y()+global.y());
        render_hull.push_back(p);
    }

    /*auto p_last = render_hull.back();
    for(auto p : render_hull) {
        circle(p, 8).render();
        line(p_last, p).render();
        p_last = p;
    }*/

    for(int i = 0, j = hull.size()-1; i < hull.size(); j = i++) {
        circle(render_hull[i], 8).render();
        line l(render_hull[j], render_hull[i]);
        l.render();
        //line n = algorithm::normal(l, hull_force[i]*5);
        //n.colorR(1);
        //n.render();
    }

    for(auto m : muscle) {
        auto p1 = render_hull[m.from];
        auto p2 = render_hull[m.to];
        auto l = line(p1, p2);
        l.colorG(1.0);
        l.render();
    }

    auto centroid = algorithm::centroid(render_hull);
    auto c = circle(centroid, 16);
    c.colorR(1);
    c.render();

    line(global, point(global.x()+motion.x()*5, global.y()+motion.y()*5)).render();
}

void creature::physicsHullLength() {
    hull_force.resize(hull.size());
    for(int i = 0, j = hull.size()-1; i < hull.size(); j = i++) {
        float len = algorithm::distance(hull[i], hull[j]);
        point m(hull[j].x() - hull[i].x(), hull[j].y() - hull[i].y());
        m.x(m.x() / len);
        m.y(m.y() / len);
        float lend = hull_length[i] - len; //delta-mm
        float force = lend * spring_factor; //F, spring_factor=N/mm
        m.x(m.x() * force);
        m.y(m.y() * force);
        hull_force[j].x(hull_force[j].x() + m.x());
        hull_force[j].y(hull_force[j].y() + m.y());
        hull_force[i].x(hull_force[i].x() - m.x());
        hull_force[i].y(hull_force[i].y() - m.y());
    }
}

void creature::physicsHullArea() {
    hull_force.resize(hull.size());
    float nmass = algorithm::area(hull);
    float len = 0;
    for(int i = 0, j = hull.size()-1; i < hull.size(); j = i++) {
        len += algorithm::distance(hull[i], hull[j]);
    }
    float nmass_each_line = (nmass-mass)/len;
    for(int i = 0, j = hull.size()-1; i < hull.size(); j = i++) {
        float slen = algorithm::distance(hull[i], hull[j]);
        float f = nmass_each_line * slen * spring_factor_mass;
        line n = algorithm::normal(line(hull[j], hull[i]));
        point m(n.target().x()-n.origin().x(), n.target().y()-n.origin().y());
        m.x(m.x() * f);
        m.y(m.y() * f);
        hull_force[j].x(hull_force[j].x() + m.x());
        hull_force[j].y(hull_force[j].y() + m.y());
        hull_force[i].x(hull_force[i].x() + m.x());
        hull_force[i].y(hull_force[i].y() + m.y());
    }
}

void creature::physicsHullAngle() {
    //nothing for now
}

void creature::physicsSelfIntersection() {
    //nothing for now
}

void creature::physicsIntersection() {
    //nothing for now
}

void creature::physicsMuscles() {
    for(auto &mu : muscle) {
        int i = mu.from;
        int j = mu.to;
        float len = algorithm::distance(hull[i], hull[j]);
        point m(hull[j].x() - hull[i].x(), hull[j].y() - hull[i].y());
        m.x(m.x() / len);
        m.y(m.y() / len);
        float lend = mu.length*(1+mu.active) - len; //delta-mm
        float force = lend * spring_factor_muscle; //F, spring_factor=N/mm
        m.x(m.x() * force);
        m.y(m.y() * force);
        hull_force[j].x(hull_force[j].x() + m.x());
        hull_force[j].y(hull_force[j].y() + m.y());
        hull_force[i].x(hull_force[i].x() - m.x());
        hull_force[i].y(hull_force[i].y() - m.y());
    }
}

void creature::applyForces() {
    hull_new.resize(hull.size());
    //1. simplyfied friction
    for(int i = 0; i < hull.size(); ++i) {
        hull_force[i].x(hull_force[i].x() * (1-friction));
        hull_force[i].y(hull_force[i].y() * (1-friction));
    }
    //2. apply to hull_new
    for(int i = 0; i < hull.size(); ++i) {
        hull_new[i].x(hull[i].x() + hull_force[i].x()/100.0); //thats not very right.. F!=delta mm
        hull_new[i].y(hull[i].y() + hull_force[i].y()/100.0); //hull_force should be acceleration values ?
    }
    //3. recenter
    point centroid = algorithm::centroid(hull);
    point centroid_n = algorithm::centroid(hull_new);
    for(int i = 0; i < hull.size(); ++i) {
        hull_new[i].x(hull_new[i].x() - centroid_n.x() + centroid.x());
        hull_new[i].y(hull_new[i].y() - centroid_n.y() + centroid.y());
    }
    //4. calculate movment .. hmm ?
    std::swap(hull, hull_new);
}
