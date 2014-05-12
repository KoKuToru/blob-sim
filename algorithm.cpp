#include "algorithm.h"
#include "line.h"
#include "point.h"
#include <cmath>

algorithm::algorithm() {}

std::tuple<bool , point> algorithm::intersect(const line& a, const line& b) {
    //from http://stackoverflow.com/a/1968345
    double p0_x = a.origin().x();
    double p0_y = a.origin().y();
    double p1_x = a.target().x();
    double p1_y = a.target().y();
    double p2_x = b.origin().x();
    double p2_y = b.origin().y();
    double p3_x = b.target().x();
    double p3_y = b.target().y();

    double s1_x = p1_x - p0_x;
    double s1_y = p1_y - p0_y;
    double s2_x = p3_x - p2_x;
    double s2_y = p3_y - p2_y;

    if ((-s2_x * s1_y + s1_x * s2_y) == 0) {
        return std::make_tuple(false, point());
    }

    double s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    double t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    double w = 0.1;
    if (s >= 0-w && s <= 1+w && t >= 0-w && t <= 1+w)
    {
        // Collision detected
        return std::make_tuple(true, point(p0_x+t*s1_x, p0_y+t*s1_y)); //t or s ? need to be checked
    }
    return std::make_tuple(false, point());
}

std::tuple<float, point> algorithm::distance(const line& a, const point &p) {
    float A = p.x() - a.origin().x();
    float B = p.y() - a.origin().y();
    float C = a.target().x() - a.origin().x();
    float D = a.target().y() - a.origin().y();

    float dot = A * C + B * D;
    float len_sq = C * C + D * D;
    float param = dot / len_sq;

    float xx, yy;

    if (param < 0 || (a.origin().x() == a.target().x() && a.origin().y() == a.origin().x())) {
        xx = a.origin().x();
        yy = a.origin().y();
    }
    else if (param > 1) {
        xx = a.target().x();
        yy = a.target().y();
    }
    else {
        xx = a.origin().x() + param * C;
        yy = a.origin().y() + param * D;
    }

    float dx = p.x() - xx;
    float dy = p.y() - yy;
    return std::make_tuple(sqrt(dx * dx + dy * dy), point(xx, yy));
}

float algorithm::angle(const line &a, const line &b) {
    float x1 = a.target().x() - a.origin().x();
    float y1 = a.target().y() - a.origin().y();
    float s1 = sqrt(x1*x1+y1*y1);
    x1 /= s1;
    y1 /= s1;
    float x2 = b.target().x() - b.origin().x();
    float y2 = b.target().y() - b.origin().y();
    float s2 = sqrt(x2*x2+y2*y2);
    x2 /= s2;
    y2 /= s2;
    float dot = x1*x2 + y1*y2;
    return acos(dot);
}
