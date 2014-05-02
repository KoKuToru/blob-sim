#ifndef CREATURE_H
#define CREATURE_H
#include <map>
#include <vector>
#include "circle.h"
class creature
{
    public:
        enum part_type: unsigned char {
            LIMB,
            SENSOR
        };
    private:
        struct part {
                part_type type;
                unsigned char root;
                unsigned char pos;
                unsigned char len;
        };

        std::vector<part> m_parts;

        std::vector<circle> m_body;
    public:
        creature();
        void attach(part_type type, unsigned char root, unsigned char pos, unsigned char size);
        void update();
        void render() const ;
};

#endif // CREATURE_H
