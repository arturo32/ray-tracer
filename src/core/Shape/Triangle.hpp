#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../rt3-base.hpp"
#include "shape.hpp"
#include "TriangleMesh.hpp"

namespace rt3 {

class Triangle : public Shape {
    public:
		Triangle(bool flip, const Point3i& i, std::shared_ptr<TriangleMesh> m) : 
			Shape(flip), indices(i), mesh(m) {}
		~Triangle() = default;		
		
		const Point3i indices;
		std::shared_ptr<TriangleMesh> mesh;

		bool intersect( const Ray& r, real_type& t_hit, Surfel *sf ) const;
		bool intersect_p( const Ray& r ) const;
		Bounds3f world_bounds() const;
};
}
#endif  