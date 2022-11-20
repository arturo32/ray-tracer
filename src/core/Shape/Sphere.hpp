#ifndef SPHERE_H
#define SPHERE_H

#include "../rt3-base.hpp"
#include "shape.hpp"

namespace rt3 {

class Sphere : public Shape {
	public:
		Sphere(bool flip, const Point3f& c, const real_type& r) : Shape(flip), center{c}, radius{r} {/*empty*/}
		~Sphere() = default;		
		const Point3f center;
		const real_type radius;		
		bool intersect( const Ray& r, real_type& t_hit, Surfel *sf ) const;
		bool intersect_p( const Ray& r ) const;
		Bounds3f world_bounds() const;
};
}
#endif  