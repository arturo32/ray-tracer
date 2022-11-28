#ifndef SPHERE_H
#define SPHERE_H

#include "../rt3-base.hpp"
#include "shape.hpp"

namespace rt3 {

class Sphere : public Shape {
	public:
		Sphere(bool flip, const Point3f& c, const real_type& r)
		: Shape(flip), center{c}, radius{r} {
			Point3f r3 = vec3(radius, radius, radius);
			// std::cout << "bounds sphere = min:" << center - r3 << " | max:" << center + r3 << std::endl;
			bounds = Bounds3f(center - r3, center + r3);
		}
		~Sphere() = default;		
		Bounds3f bounds;
		const Point3f center;
		const real_type radius;		
		bool intersect( Ray& r, real_type& t_hit, Surfel *sf ) const;
		bool intersect_p( Ray& r ) const;
		Bounds3f world_bounds() const {
			return bounds;
		}
};
}
#endif  