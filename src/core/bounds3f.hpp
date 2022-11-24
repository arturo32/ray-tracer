#ifndef BOUNDS3f_H
#define BOUNDS3f_H

#include "rt3-base.hpp"
#include "rt3.hpp"

namespace rt3 {
	class Bounds3f {
        public:
        Point3f p_min;
        Point3f p_max;

        Bounds3f(Point3f a, Point3f b): p_min{a}, p_max{b}{};

        Bounds3f unite(const Bounds3f &other) const;
        bool intersect_p(const Ray &ray, float &hit1, float &hit2);
    };
}

#endif // BOUNDS3f_H