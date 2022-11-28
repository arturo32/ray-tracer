#include "bvh.hpp"

namespace rt3 {

	BVH::BVH(std::vector<shared_ptr<Primitive>>& objs,
			size_t start, size_t end, short axis,
			std::string method, size_t leaf_size) {
		axis = axis % 3;
		std::sort(objs.begin()+start, objs.begin()+end,
		[axis](std::shared_ptr<rt3::Primitive> a, std::shared_ptr<rt3::Primitive> b) {
			return a->world_bounds().p_min[axis] > b->world_bounds().p_min[axis];
		});

		size_t diff = end-start;
		std::cout << "diff: " << diff << std::endl;
		if (diff == 1) {
			left = right = objs[start+0];
		} else if (diff == 2){
			left = objs[start+0];
			right = objs[start+1];
		} else {
			size_t mid = start+(diff/2);
			left = make_shared<BVH>(objs, start, mid, axis+1, method, leaf_size);
			right = make_shared<BVH>(objs, mid, end, axis+1, method, leaf_size);
		}
		bounds = left->world_bounds().unite(right->world_bounds());
		std::cout << "bounds = min: " << bounds.p_min << " | max: "<< bounds.p_max << std::endl;

	}	

	void BVH::intersect( Ray& r, Surfel *sf ) const {
		Ray r1 = Ray(r.origin, r.direction);
		r1.t_max = r.t_max;
		r1.t_min = r.t_min;
		if (bounds.intersect_p(r1)) {
			left->intersect(r, sf);
			right->intersect(r, sf);
		} else {
			sf->hit = false;
		}
	}

	bool BVH::intersect_p( Ray& r ) const {
		Ray r1 = Ray(r.origin, r.direction);
		r1.t_max = r.t_max;
		r1.t_min = r.t_min;
		if (bounds.intersect_p(r1)) {
			if (left->intersect_p(r)) {
				return true;
			} else {
				return right->intersect_p(r);
			}
		} else {
			return false;
		}
	}
}