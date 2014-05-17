#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <tuple>
#include <vector>

class line;
class point;
class circle;

class algorithm
{
    private:
        algorithm();
        static float sign(float v);

    public:
        static std::tuple<bool , point> intersect(const line& a, const line& b, bool both=true); //! if both is false only checks if b crosses a
        static std::tuple<float, point> distance(const line& a, const point &p);
        static float side(const line &a, const point& p);
        static float angle(const line &a, const line &b);
        static point center(const std::vector<point> &poly);
        static point centroid(const std::vector<point> &poly);
        static float area(const std::vector<point> &poly);
        static line normal(const line &a);
        static float perimeter(const std::vector<point> &poly);
        static std::tuple<float, float> approximateWidthHeight(const std::vector<point> &poly);
};

#endif // ALGORITHM_H
