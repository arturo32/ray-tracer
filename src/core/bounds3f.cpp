#include "bounds3f.hpp"
#include <cmath>
namespace rt3 {

	Bounds3f Bounds3f::unite(const Bounds3f &c) const {
		Point3f small = Point3f(
			std::min(p_min[0],c.p_min[0]),
			std::min(p_min[1],c.p_min[1]),
			std::min(p_min[2],c.p_min[2])
		);
		Point3f big = Point3f(
			std::max(p_max[0],c.p_max[0]),
			std::max(p_max[1],c.p_max[1]),
			std::max(p_max[2],c.p_max[2])
		);
		return Bounds3f(small, big);
	}	

	bool Bounds3f::intersect_p(Ray &ray) const {
		for (size_t i = 0; i < 3; ++i) {
			real_type inv = real_type(1.0) / ray.direction[i];
			real_type t0 = (p_min[i] - ray.origin[i]) * inv;
			real_type t1 = (p_max[i] - ray.origin[i]) * inv;
			if (inv < real_type(0.0)) {
				std::swap(t0, t1);
			}
			ray.t_min = t0 > ray.t_min ? t0 : ray.t_min;
			ray.t_max = t1 > ray.t_max ? t1 : ray.t_max;
			if (ray.t_max <= ray.t_min) {
				return false;
			}
		}
		return true;
	}
}