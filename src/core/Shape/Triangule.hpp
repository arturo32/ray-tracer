#ifndef TRIANGULE_H
#define TRIANGULE_H

#include "../rt3-base.hpp"
#include "shape.hpp"

namespace rt3 {

class Triangule : public Shape {
    public:
		Triangule(bool flip, const vector<Point3f>& p) : Shape(flip) {
			copy(p.begin(), p.end(), points);
		}
		~Triangule() = default;		
		Point3f points[3];
		bool intersect( const Ray& r, real_type& t_hit, Surfel *sf ) const;
		bool intersect_p( const Ray& r ) const;
		Bounds3f world_bounds() const;
};
}
#endif  