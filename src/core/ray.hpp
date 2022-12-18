#ifndef RAY_H
#define RAY_H

#include "rt3.hpp"
#include "Matrix4x4.hpp"

#include "../ext/vec3.hpp"

namespace rt3 {
using Point3f = vec3<real_type>;
using Vector3f = vec3<real_type>;
	class Ray {
		public:
			Ray (const Point3f& o, const Vector3f& d ) : origin{o}, direction{d} {/*empty*/}
			const Point3f origin; //!< origin
			const Vector3f direction; //!< direction
			real_type t_max = INFINITY;
			real_type t_min = 0;
			// O que precisa ser normalizado exatamente?
			// Point3f normalize() const { return ???; }
			Point3f operator()(real_type t) const { return origin + direction * t; }

			friend Ray operator*(const Matrix4x4& m, const Ray& r)  {
				Vector3f old_direction = Vector3f(r.direction);
				//old_direction.make_unit_vector();
				real_type x = old_direction.x();
				real_type y = old_direction.y();
				real_type z = old_direction.z();
				
				
				Vector3f result = Vector3f(x*m.m[0][0] + y*m.m[0][1] + z*m.m[0][2] + m.m[0][3],
											x*m.m[1][0] + y*m.m[1][1] + z*m.m[1][2] + m.m[1][3],
											x*m.m[2][0] + y*m.m[2][1] + z*m.m[2][2] + m.m[2][3]);

				//result.make_unit_vector();
				//Ray newR = Ray(r.origin, result*r.direction.length());
				
				Ray newR = Ray(r.origin, result);
				newR.t_max = r.t_max;
				newR.t_min = r.t_min;

				return newR;
			}
			
			
	};
}
#endif // RAY_H
