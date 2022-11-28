#ifndef BVH_H
#define BVH_H

#include "rt3-base.hpp"
#include "rt3.hpp"
#include <memory>

namespace rt3 {
	class BVH : public AggregatePrimitive {
        public:
        Bounds3f bounds;
        std::shared_ptr<Primitive> left = nullptr;
        std::shared_ptr<Primitive> right = nullptr;

        // methods

        BVH(std::vector<shared_ptr<Primitive>>& objs,
           size_t start, size_t end, short axis, 
           std::string method, size_t leaf_size);
		
        Bounds3f world_bounds() {
            return bounds;
        }

		void intersect( Ray& r, Surfel *sf ) const;

		bool intersect_p( Ray& r ) const;
    };
}

#endif // BVH_H