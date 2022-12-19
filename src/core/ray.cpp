#include "ray.hpp"

namespace rt3 {

	Ray operator*(const Matrix4x4& m, const Ray& r)  {
		real_type x = r.origin.x();
		real_type y = r.origin.y();
		real_type z = r.origin.z();
		
		Point3f n_origin = Point3f(x*m.m[0][0] + y*m.m[0][1] + z*m.m[0][2] + m.m[0][3],
								   x*m.m[1][0] + y*m.m[1][1] + z*m.m[1][2] + m.m[1][3],
								   x*m.m[2][0] + y*m.m[2][1] + z*m.m[2][2] + m.m[2][3]);

		x = r.direction.x();
		y = r.direction.y();
		z = r.direction.z();

		Vector3f n_direction = Vector3f(x*m.m[0][0] + y*m.m[0][1] + z*m.m[0][2],
										x*m.m[1][0] + y*m.m[1][1] + z*m.m[1][2],
										x*m.m[2][0] + y*m.m[2][1] + z*m.m[2][2]);

		n_direction.make_unit_vector();
		//Ray newR = Ray(r.origin, result*r.direction.length());
		
		Ray newR = Ray(n_origin, n_direction);
		newR.t_max = r.t_max;
		newR.t_min = r.t_min;

		return newR;
	}
}
