#include "creature.h"
#include "circle.h"
#include <cmath>
#include <iostream>
#include "algorithm.h"

constexpr float spring_factor = 0.234;
constexpr float spring_factor_mass = 0.4/700;
constexpr float spring_factor_muscle = spring_factor;

creature::creature()
{
    //for now static
    int points = 16;
    int size = 256;
    for(int i = 0; i < points; ++i) {
        float x = sin(-2*i*M_PI/points)*size;
        float y = cos(-2*i*M_PI/points)*size;
        hull.push_back(point(x, y));
    }
    auto p_last = hull.back();
    for(auto p : hull) {
        hull_length.push_back(algorithm::distance(p_last, p)/2.0);
        p_last = p;
    }

    mass = algorithm::area(hull);

    muscle.push_back(Muscle{
                         0,
                         points/2,
                         algorithm::distance(hull[0], hull[points/2])/2.0f,
                         0});
    /*muscle.push_back(Muscle{
                         points/4,
                         points/2+points/4,
                         algorithm::distance(hull[points/4], hull[points/2+points/4])/2.0f,
                         0});*/
}

int mod(int v, int s) {
    while(v < 0) v += s;
    return v % s;
}

/*
 * not thread safe
 * */
void creature::update() {
    static std::vector<float> force;
    force.clear();
    force.reserve(hull_length.size());

    //update muscles
    for(auto m : muscle) {
        point &pa = hull[m.from];
        point &pb = hull[m.to];

        point mo(pa.x()-pb.x(), pa.y()-pb.y());

        float l = algorithm::distance(point(0, 0), mo);

        //this update is not good !!
        float f = (m.length*(1+m.active)-l)*spring_factor_muscle;

        point m1(mo.x()/l*f, mo.y()/l*f);
        point m2(-m1.x(), -m1.y());

        pa.x(pa.x()+m1.x());
        pa.y(pa.y()+m1.y());

        pb.x(pb.x()-m1.x());
        pb.y(pb.y()-m1.y());
    }

    //calculate force
    auto p_last = hull.back();
    for(auto p : hull) {
        force.push_back(algorithm::distance(p_last, p));
        p_last = p;
    }

    for(int i = 0; i < hull_length.size(); ++i) {
        force[i] = -(hull_length[i]-force[i])*spring_factor;
    }

    static std::vector<std::pair<point, point>> movment;
    movment.clear();
    movment.resize(hull.size());

    //calculate point movment:
    for(int i = 0, j = hull.size()-1; i < hull.size(); j = i++) {
        point &pa = hull[j];
        point &pb = hull[i];

        point m(pa.x()-pb.x(), pa.y()-pb.y());

        //normalize
        float l = algorithm::distance(point(0,0), m);
        float f = force[i];

        point m1(m.x()/l*f, m.y()/l*f);
        point m2(-m1.x(), -m1.y());

        movment[i].first = m1;
        movment[j].second = m2;
    }

    //update points
    for(int i = 0; i < hull.size(); ++i) {
        point &p = hull[i];
        const point &m1 = movment[i].first;
        const point &m2 = movment[i].second;

        p.x(p.x()+m1.x()+m2.x());
        p.y(p.y()+m1.y()+m2.y());
    }

    //force by area
    float nmass = algorithm::area(hull);
    float fmass = (nmass-mass)*spring_factor_mass;

    //caclulate movment
    for(int i = 0, j = hull.size()-1; i < hull.size(); j = i++) {
        point &pa = hull[j];
        point &pb = hull[i];

        line n = algorithm::normal(line(pa, pb));

        point pv(n.target().x()-n.origin().x(), n.target().y()-n.origin().y());

        point m(pv.x()*fmass, pv.y()*fmass);

        movment[i].first = m;
        movment[j].second = m;
    }

    //update points
    for(int i = 0; i < hull.size(); ++i) {
        point &p = hull[i];
        const point &m1 = movment[i].first;
        const point &m2 = movment[i].second;

        p.x(p.x()+m1.x()+m2.x());
        p.y(p.y()+m1.y()+m2.y());
    }

    static int f = 0;
    muscle[0].active = cos(f++/10.0);
}

void creature::render() const {
    auto p_last = hull.back();
    for(auto p : hull) {
        circle(p, 8).render();
        line(p_last, p).render();
        p_last = p;
    }
    for(auto m : muscle) {
        auto p1 = hull[m.from];
        auto p2 = hull[m.to];
        auto l = line(p1, p2);
        l.colorG(1.0);
        l.render();
    }
}
