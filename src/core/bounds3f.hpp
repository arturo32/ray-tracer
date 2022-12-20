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

    inline rt3::Bounds3f addPoint(const rt3::Bounds3f &b, const Point3f &p) {
		return rt3::Bounds3f(
			Point3f(std::min(b.p_min.x(), p.x()),
					std::min(b.p_min.y(), p.y()),
					std::min(b.p_min.z(), p.z())),
			Point3f(std::max(b.p_max.x(), p.x()),
					std::max(b.p_max.y(), p.y()),
					std::max(b.p_max.z(), p.z()))
		);
	}

	inline rt3::Bounds3f MultBounds(const rt3::Matrix4x4 &m, const Bounds3f &b) {
		Bounds3f ret(
			MultPoint(m, Point3f(b.p_min.x(), b.p_min.y(), b.p_min.z())),
			MultPoint(m, Point3f(b.p_max.x(), b.p_min.y(), b.p_min.z()))
		);    
		ret = addPoint(ret, MultPoint(m, Point3f(b.p_min.x(), b.p_max.y(), b.p_min.z())));
		ret = addPoint(ret, MultPoint(m, Point3f(b.p_min.x(), b.p_min.y(), b.p_max.z())));
		ret = addPoint(ret, MultPoint(m, Point3f(b.p_min.x(), b.p_max.y(), b.p_max.z())));
		ret = addPoint(ret, MultPoint(m, Point3f(b.p_max.x(), b.p_max.y(), b.p_min.z())));
		ret = addPoint(ret, MultPoint(m, Point3f(b.p_max.x(), b.p_min.y(), b.p_max.z())));
		ret = addPoint(ret, MultPoint(m, Point3f(b.p_max.x(), b.p_max.y(), b.p_max.z())));
		return ret;
	}
}

#endif // BOUNDS3f_H