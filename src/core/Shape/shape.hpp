#ifndef SHAPE_H
#define SHAPE_H

#include "../rt3-base.hpp"
#include "../ray.hpp"


namespace rt3 {

using Bounds3f = vec2<Point3f>;

class Surfel;

class Shape {
	protected:
		bool flip_normals;
	public:
		Shape(bool flip) : flip_normals{flip}{}
		~Shape() = default;
		virtual Bounds3f world_bounds() const = 0;
		virtual bool intersect( const Ray& r, real_type& t_hit, Surfel *sf ) const = 0;
		virtual bool intersect_p( const Ray& r ) const = 0;
};

}

#endif