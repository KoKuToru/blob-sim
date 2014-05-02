#include "creature.h"
#include "circle.h"
#include <cmath>
#include <iostream>
creature::creature()
{
}

void creature::attach(part_type type, unsigned char root, unsigned char pos, unsigned char size) {
    if (m_parts.empty()) {
        m_parts.push_back(part{type, 0, 0, size});
        return;
    }
    if (root >= m_parts.size()) {
        //can't attach
        std::cout << "out ot range" << std::endl;
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
    m_parts.push_back(part{type, root, (unsigned char)(pos%9), size});
}

void creature::update() {
    m_body.clear();
    //build creature
    for(auto &p: m_parts) {
        point pos;
        if (p.root < m_body.size()) {
            point o = m_body[p.root].origin();
            pos = m_body[p.root].getLineSegment(p.pos).origin();
            float x = pos.x() - o.x();
            float y = pos.y() - o.y();
            float d = sqrt(x*x + y*y);
            x = o.x() + (x/d)*(d+p.len+1);
            y = o.y() + (y/d)*(d+p.len+1);
            pos.x(x).y(y);
        }
        std::cout << (void*)&p << ": " << pos.x() << ", " << pos.y() << std::endl;
        pos.colorA(1).colorR(0).colorB(1).colorG(0);
        circle tmp(pos, p.len);
        //collision check before adding !
        m_body.push_back(tmp);
    }
}

void creature::render() const {
    for(auto &p: m_body) {
        p.render();
    }
}
