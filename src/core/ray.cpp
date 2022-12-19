#include "ray.hpp"

namespace rt3 {

	Ray operator*(const Matrix4x4& m, const Ray& r)  {
		Ray newR = Ray(MultPoint(m, r.origin), MultVector(m, r.direction));
		newR.t_max = r.t_max;
		newR.t_min = r.t_min;
		return newR;
	}
}
