#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../rt3-base.hpp"
#include "shape.hpp"
#include "TriangleMesh.hpp"

namespace rt3 {

class Triangle : public Shape {
    public:
		Triangle(bool flip, const Point3i& i, const Point3i& n, const Point3i& uvs, std::shared_ptr<TriangleMesh> m) : 
			Shape(flip), indices(i), normals(n), uvs(uvs), mesh(m) {}
		~Triangle() = default;		
		
		const Point3i indices;
		const Point3i normals;
		const Point3i uvs;
		std::shared_ptr<TriangleMesh> mesh;

		bool intersect( const Ray& r, real_type& t_hit, Surfel *sf ) const;
		bool intersect_p( const Ray& r ) const;
		Bounds3f world_bounds() const;
};
}
#endif  