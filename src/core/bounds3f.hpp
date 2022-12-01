#ifndef BOUNDS3f_H
#define BOUNDS3f_H

#include "rt3.hpp"
#include "ray.hpp"

namespace rt3 {
	class Bounds3f {
        public:
        Point3f p_min;
        Point3f p_max;

        Bounds3f() = default;
        Bounds3f(Point3f a, Point3f b): p_min{a}, p_max{b}{};

        Bounds3f unite(const Bounds3f &other) const;
        bool intersect_p(const Ray &ray) const;
    };

    inline std::ostream& operator<<(std::ostream &os, const Bounds3f &t) {
        os << t.p_min << " | " << t.p_max;
        return os;
    }
}

#endif // BOUNDS3f_H