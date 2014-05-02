#include "creature.h"
#include "circle.h"
#include <cmath>
#include <iostream>
creature::creature()
{
}

void creature::attach(part_type type, unsigned char root, unsigned char pos, unsigned char size) {
    pos = pos%8;

    if (m_parts.empty()) {
        m_parts.push_back(part{type, 0, 0xFF, size});
        return;
    }
    if (root >= m_parts.size()) {
        //can't attach
        std::cout << "out ot range" << std::endl;
        return;
    }
    if (type == SENSOR) {
        if (m_parts[root].type != LIMB) {
            //can't attach
            std::cout << "not a limb" << std::endl;
            return;
        }
    }
    if ((pos == 0) && (root != 0)) {
        //can't attach
        std::cout << "can't attach to primary pos.." << std::endl;
        return;
    }
    for(auto &p: m_parts) {
        if (p.root == root) {
            if (p.pos == pos) {
                //can't attach
                std::cout << "pos already used" << std::endl;
                return;
            }
        }
    }
    std::cout << "add part" << std::endl;
    m_parts.push_back(part{type, root, pos, size});
    build();
}

void creature::build() {
    m_body.clear();
    m_sensor.clear();
    //build creature
    for(auto &p: m_parts) {
        point pos;
        point pos2;
        int shift = 0;
        if (p.root < m_body.size()) {
            point o = m_body[p.root].origin();
            pos = m_body[p.root].getLineSegment(p.pos).origin();
            float x = pos.x() - o.x();
            float y = pos.y() - o.y();
            float d = sqrt(x*x + y*y);
            x = (x/d)*(d+p.len+0.5);
            y = (y/d)*(d+p.len+0.5);
            pos2 = pos;
            pos.x(o.x() + x).y(o.y() + y);
            if (p.pos  != 0xFF) {
                shift = m_body[p.root].shift();
            }
        }
        if (p.type == LIMB) {
            pos.colorA(1).colorR(0).colorB(1).colorG(0);
            circle tmp(pos, p.len);
            if (p.pos  != 0xFF) {
                tmp.shift(shift+p.pos+4);
            }
            //collision check before adding !
            m_body.push_back(tmp);
        } else if (p.type == SENSOR) {
            pos.colorA(1).colorR(1).colorB(0).colorG(0);
            pos2.colorA(1).colorR(0).colorB(1).colorG(0);
            line tmp(pos2, pos);
            m_sensor.push_back(tmp);
        }
    }
}

void creature::update() {
    //move parts
}

void creature::render() const {
    for(auto &p: m_body) {
        p.render();
    }
    for(auto &p: m_sensor) {
        p.render();
    }
}
