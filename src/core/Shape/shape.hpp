#ifndef SHAPE_H
#define SHAPE_H

#include "../rt3-base.hpp"
#include "../ray.hpp"
#include "../Transform.hpp"


namespace rt3 {

class Surfel;
class Bounds3f;

class Shape {
	protected:
		bool flip_normals;
	public:
		Shape(bool flip, const std::shared_ptr<const Transform >& o2w) :
			flip_normals{flip}, obj_to_world{o2w}{}
		~Shape() = default;
		virtual Bounds3f world_bounds() const = 0;
		virtual bool intersect( Ray& r, real_type& t_hit, Surfel *sf ) const = 0;
		virtual bool intersect_p(const Ray& r ) const = 0;

		// === Shape Public Data
		const std::shared_ptr<const Transform > obj_to_world; //!< Object to World transformation (scene specified).

};

}

#endif