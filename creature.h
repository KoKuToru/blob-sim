#ifndef CREATURE_H
#define CREATURE_H
#include <map>
#include <vector>
#include "point.h"
class creature
{
    public:
        enum part_type: unsigned char {
            LIMB,
            SENSOR
        };

    private:
        std::vector<point> hull;
        std::vector<float> hull_length;
        std::vector<float> hull_force;

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

    public:
        creature();

        void update();
        void render() const ;
};

#endif // CREATURE_H
