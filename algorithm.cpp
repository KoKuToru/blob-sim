#include "algorithm.h"
#include "line.h"
#include "point.h"
#include <cmath>

algorithm::algorithm() {}

float algorithm::sign(float v) {
    return (v == 0)?0:((v > 0)?1:-1);
}

std::tuple<bool , point> algorithm::intersect(const line& a, const line& b, bool both) {
    //check if side crosses ..
    float b_before = algorithm::side(a, b.origin());
    float b_after  = algorithm::side(a, b.target());

    //check if line changed side
    if (b_before == b_after) {
        if (both) {
            float a_before = algorithm::side(b, a.origin());
            float a_after  = algorithm::side(b, a.target());
            if (a_before == a_after) {
                return std::make_tuple(false, point());
            }
        } else {
            return std::make_tuple(false, point());
        }
    }

    //check if where a intersects b, we know we must intesect, because of side check
    float x1_ = b.origin().x();
    float y1_ = b.origin().y();
    float x2_ = b.target().x();
    float y2_ = b.target().y();
    float x3_ = a.origin().x();
    float y3_ = a.origin().y();
    float x4_ = a.target().x();
    float y4_ = a.target().y();

    float d = (((x2_ - x1_) * (y4_ - y3_)) - (y2_ - y1_) * (x4_ - x3_));
    float r = (((y1_ - y3_) * (x4_ - x3_)) - (x1_ - x3_) * (y4_ - y3_)) / d;
    float s = (((y1_ - y3_) * (x2_ - x1_)) - (x1_ - x3_) * (y2_ - y1_)) / d;

    if (s >= 0 && s <= 1) { //distance from a.origin()
        if (r >= 0 && r <= 1) { //distance from b.orgin()
            float x = x1_ + r * (x2_ - x1_);
            float y = y1_ + r * (y2_ - y1_);
            return std::make_tuple(true, point(x, y));
        }
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

float algorithm::side(const line &a, const point& p) {
    float x1 = a.target().x() - a.origin().x();
    float y1 = a.target().y() - a.origin().y();
    float x2 = p.y() - a.origin().y();
    float y2 = p.x() - a.origin().x(); //x,y flipe => normal
    float dot_n = x1*x2 - y1*y2;
    return sign(dot_n);
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

point algorithm::center(const std::vector<point> &poly) {
    float x = 0;
    float y = 0;
    for(auto &p: poly) {
        x += p.x();
        y += p.y();
    }
    x /= poly.size();
    y /= poly.size();
    return point(x, y);
}

point algorithm::centroid(const std::vector<point> &poly) {
    float cx = 0;
    float cy = 0;
    float area2 = 0;
    for (size_t i = 0; i < poly.size(); ++i) {
        size_t i_next = (i+1)%poly.size();
        float f = poly[i].x()*poly[i_next].y() - poly[i_next].x()*poly[i].y();
        cx += (poly[i].x() + poly[i_next].x())*f;
        cy += (poly[i].y() + poly[i_next].y())*f;
        area2 += f;
    }

    //float A = algorithm::area(poly);
    float A = 0.5*area2;
    cx /= (6*A);
    cy /= (6*A);
    return point(cx, cy);
}

float algorithm::area(const std::vector<point> &poly) {
    float area2 = 0;
    for (size_t i = 0; i < poly.size(); ++i) {
        size_t i_next = (i+1)%poly.size();
        float f = poly[i].x()*poly[i_next].y() - poly[i_next].x()*poly[i].y();
        area2 += f;
    }
    return 0.5*area2;
}

line algorithm::normal(const line &a) {
    float x = a.target().x() - a.origin().x();
    float y = a.target().y() - a.origin().y();

    float s = sqrt(x*x+y*y);

    float x_start = a.origin().x() + x/2;
    float y_start = a.origin().y() + y/2;
    float x_stop  = x_start - y/s *50;
    float y_stop  = y_start + x/s *50; //x,y flipped !

    return line(point(x_start, y_start), point(x_stop, y_stop));
}

float algorithm::perimeter(const std::vector<point> &poly) {
    float res = 0;
    for(size_t i = 0; i < poly.size(); ++i) {
        size_t i_next = (i+1)%poly.size();
        const point &a = poly[i];
        const point &b = poly[i_next];
        res += algorithm::distance(a, b);
    }
    return res;
}

std::tuple<float, float> algorithm::approximateWidthHeight(const std::vector<point> &poly) {
    float A = algorithm::area(poly);
    float U = algorithm::perimeter(poly);

    /*
     Annahme das es ungefähr Rechteckig ist

     A = a*b
     U = 2*(a+b)

     Wolfram Alpha:
      Solve[(A = a*b, U = 2*(a+b)), (a,b)]

     Result:
    */
    float a = 1.0f/4.0f*(U - sqrt(U*U - 16.0f*A));
    float b = 1.0f/4.0f*(sqrt(U*U - 16.0f*A) + U);

    return std::make_tuple(a, b);
}

float algorithm::distance(const point& a, const point& b) {
    float x = b.x() - a.x();
    float y = b.y() - a.y();
    float s = sqrt(x*x+y*y);
    return s;
}
