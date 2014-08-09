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

constexpr float spring_factor = 0.234;
constexpr float spring_factor_mass = 0.4/700;
constexpr float spring_factor_muscle = spring_factor/5.0;
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
    static std::vector<point> old_hull;
    old_hull.clear();
    for(const auto &p : hull) {
        old_hull.push_back(p);
    }

    //movement by muscles
    for(const auto& mu : muscle) {
        const point &pa = old_hull[mu.from];
        const point &pb = old_hull[mu.to];
        point &pan = hull[mu.from];
        point &pbn = hull[mu.to];

        point m(pa.x()-pb.x(), pa.y()-pb.y());

        float l = algorithm::distance(point(0, 0), m);
        float f = (mu.length*(1+mu.active)-l)*spring_factor_muscle;

        point m1(m.x()/l*f, m.y()/l*f);
        point m2(-m1.x(), -m1.y());

        pan.x(pan.x()+m1.x());
        pan.y(pan.y()+m1.y());

        pbn.x(pbn.x()-m1.x());
        pbn.y(pbn.y()-m1.y());
    }


    //movment by hull
    for(int i = 0, j = hull.size()-1; i < hull.size(); j = i++) {
        const point &pa = old_hull[j];
        const point &pb = old_hull[i];
        point &pan = hull[j];
        point &pbn = hull[i];

        point m(pa.x()-pb.x(), pa.y()-pb.y());

        float l = algorithm::distance(point(0,0), m);
        float f = (hull_length[i]-algorithm::distance(pa, pb))*spring_factor;

        point m1(m.x()/l*f, m.y()/l*f);
        point m2(-m1.x(), -m1.y());

        pan.x(pan.x()+m1.x());
        pan.y(pan.y()+m1.y());

        pbn.x(pbn.x()+m2.x());
        pbn.y(pbn.y()+m2.y());
    }

    //movment by area
    float nmass = algorithm::area(hull);
    float fmass = (nmass-mass)*spring_factor_mass;

    //caclulate movment
    for(int i = 0, j = hull.size()-1; i < hull.size(); j = i++) {
        const point &pa = old_hull[j];
        const point &pb = old_hull[i];
        point &pan = hull[j];
        point &pbn = hull[i];

        line n = algorithm::normal(line(pa, pb));

        point pv(n.target().x()-n.origin().x(), n.target().y()-n.origin().y());

        point m(pv.x()*fmass, pv.y()*fmass);

        pan.x(pan.x()+m.x());
        pan.y(pan.y()+m.y());

        pbn.x(pbn.x()+m.x());
        pbn.y(pbn.y()+m.y());
    }

    //recenter:
    auto centroid = algorithm::centroid(hull);
    for(auto &p : hull) {
        p.x(p.x()-centroid.x());
        p.y(p.y()-centroid.y());
    }


    //caluclate global movment: (thats probably not physically correct)
    point &m = motion;
    m.x(0);
    m.y(0);
    hull_force.clear();
    for(int i = 0, j = hull.size()-1; i < hull.size(); j = i++) {
        const point &pa = old_hull[j];
        const point &pb = old_hull[i];
        const point &pan = hull[j];
        const point &pbn = hull[i];

        //const line n = algorithm::normal(line(pa, pb));
        const line nn = algorithm::normal(line(pan, pbn));
        point nv(nn.target().x()-nn.origin().x(), nn.target().y()-nn.origin().y());

        //changed area:
        static std::vector<point> poly;
        poly.clear();
        poly.push_back(pa);
        poly.push_back(pan);
        poly.push_back(pbn);
        poly.push_back(pb);

        float area = algorithm::area(poly);

        //more area -> more force
        float force = area*0.15;

        force = -force; //move against normal
        m.x(m.x()+force*nv.x());
        m.y(m.y()+force*nv.y());

        hull_force.push_back(force);
    }
    global.x(global.x()-m.x());
    global.y(global.y()-m.y());
    /*
    auto centroid_old = algorithm::centroid(old_hull);
    auto centroid_neu = algorithm::centroid(hull);

    gloabl_motion.x(gloabl_motion.x()+centroid_neu.x()-centroid_old.x());
    gloabl_motion.y(gloabl_motion.y()+centroid_neu.y()-centroid_old.y());

    global.x(gloabl_motion.x());
    global.y(gloabl_motion.y());*/
    /*point m;
    for(int i = 0; i < hull.size(); ++i) {
        const point &pa = old_hull[i];
        const point &pb = hull[i];
        m.x(m.x()+pa.x()-pb.x());
        m.y(m.y()+pa.y()-pb.y());
    }
    global.x(global.x()+m.x()/hull.size());
    global.y(global.y()+m.y()/hull.size());
    */
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
        line n = algorithm::normal(l, hull_force[i]*5);
        n.colorR(1);
        n.render();
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
