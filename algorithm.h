#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <tuple>

class line;
class point;
class circle;

class algorithm
{
    private:
        algorithm();
        static float sign(float v);

    public:
        static std::tuple<bool , point> intersect(const line& a, const line& b);
        static std::tuple<float, point> distance(const line& a, const point &p);
        static float angle(const line &a, const line &b);
};

#endif // ALGORITHM_H
