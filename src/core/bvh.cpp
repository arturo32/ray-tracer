#include "bvh.hpp"

namespace rt3 {

	void att_surfel(Surfel *sfa, Surfel sfb) {
		sfa->hit = sfb.hit;
		sfa->n = sfb.n;
		sfa->p = sfb.p;
		sfa->primitive = sfb.primitive;
		sfa->time = sfb.time;
		sfa->uv = sfb.uv;
		sfa->wo = sfb.wo;
	}

	BVH::BVH(std::vector<shared_ptr<Primitive>>& objs,
			size_t start, size_t end, short axis,
			std::string method, size_t leaf_size) {
		axis = axis % 3;
		std::sort(objs.begin()+start, objs.begin()+end,
		[axis](std::shared_ptr<rt3::Primitive> a, std::shared_ptr<rt3::Primitive> b) {
			return a->world_bounds().p_min[axis] > b->world_bounds().p_min[axis];
		});

		size_t diff = end-start;
		// std::cout << "diff: " << diff << std::endl;
		if (diff == 1) {
			left = right = objs[start];
		} else if (diff == 2){
			left = objs[start];
			right = objs[start+1];
		} else {
			size_t mid = start+(diff/2);
			left = make_shared<BVH>(objs, start, mid, axis+1, method, leaf_size);
			right = make_shared<BVH>(objs, mid, end, axis+1, method, leaf_size);
		}
		bounds = left->world_bounds().unite(right->world_bounds());
		// std::cout << "bounds = min: " << bounds.p_min << " | max: "<< bounds.p_max << std::endl;
	}	

	void BVH::intersect( Ray& r, Surfel *sf ) const {
		if (bounds.intersect_p(r)) {
			Ray rl = Ray(r.origin, r.direction);
			rl.t_max = r.t_max;
			rl.t_min = r.t_min;
			Surfel sfl;
			left->intersect(rl, &sfl);
			Ray rr = Ray(r.origin, r.direction);
			rr.t_max = r.t_max;
			rr.t_min = r.t_min;
			Surfel sfr;
			right->intersect(rr, &sfr);
			if(sfl.hit && sfr.hit) {
				if (rl.t_max < rr.t_max ) {
					r.t_max = rl.t_max;
					att_surfel(sf, sfl);
				} else {
					r.t_max = rr.t_max;
					att_surfel(sf, sfr);
				}
			} else if (sfl.hit) {
				r.t_max = rl.t_max;
				att_surfel(sf, sfl);
			} else if (sfr.hit) {
				r.t_max = rr.t_max;
				att_surfel(sf, sfr);
			} else {
				sf->hit = false;
			}
		} else {
			sf->hit = false;
		}
	}

	bool BVH::intersect_p(const Ray& r ) const {
		if (bounds.intersect_p(r)) {
			return left->intersect_p(r) || right->intersect_p(r);
		} else {
			return false;
		}
	}
}